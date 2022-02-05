/*
  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
// Reads wind messages from NMEA0183 (ESP32 UART 2 on GPIO 16) and forwards them to the N2k bus
// Version 0.2, 15.10.2021, AK-Homberger

#define ESP32_CAN_TX_PIN GPIO_NUM_5  // Set CAN TX port to 5 
#define ESP32_CAN_RX_PIN GPIO_NUM_4  // Set CAN RX port to 4

#include <Arduino.h>
#include <NMEA2000_CAN.h>  // This will automatically choose the right CAN library and create suitable NMEA2000 object
#include <NMEA2000.h>
#include <N2kMessages.h>
#include <NMEA0183.h>
#include <NMEA0183Messages.h>
#include <Preferences.h>

// Global objects/variables
tNMEA0183 NMEA0183;         // NMEA stream for NMEA0183 receiving
int NodeAddress;            // To store Last Node Address
Preferences preferences;    // Nonvolatile storage on ESP32 - To store Last Node Address


//*****************************************************************************
void setup() {
  uint8_t chipid[6];
  uint32_t id = 0;
  int i = 0;
  
  Serial.begin(115200);
  NMEA0183.Begin(&Serial2, 3, 4800); // Start NMEA0183 stream handling with 4800 baud

  esp_efuse_mac_get_default(chipid);
  for (i = 0; i < 6; i++) id += (chipid[i] << (7 * i));

  // Setup NMEA2000 system
  NMEA2000.SetProductInformation("1", // Manufacturer's Model serial code
                                 10,  // Manufacturer's product code
                                 "NMEA0183 Wind to N2k", // Manufacturer's Model ID
                                 "1.0.0.1 (2015-11-18)", // Manufacturer's Software version code
                                 "1.0.0.0 (2015-11-18)"  // Manufacturer's Model version
                                );
  // Det device information
  NMEA2000.SetDeviceInformation(id,  // Unique number. Use e.g. Serial number.
                                135, // Device function=NMEA 0183 Gateway. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20%26%20function%20codes%20v%202.00.pdf
                                25,  // Device class=Inter/Intranetwork Device. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20%26%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                               );

  preferences.begin("nvs", false);                          // Open nonvolatile storage (nvs)
  NodeAddress = preferences.getInt("LastNodeAddress", 32);  // Read stored last NodeAddress, default 32
  preferences.end();

  Serial.printf("NodeAddress=%d\n", NodeAddress);

  NMEA2000.SetMode(tNMEA2000::N2km_ListenAndNode, NodeAddress);
  NMEA2000.EnableForward(false);
  NMEA2000.Open();
}


//*****************************************************************************
void CheckSourceAddressChange() {
  int SourceAddress = NMEA2000.GetN2kSource();
  if (SourceAddress != NodeAddress) { // Save potentially changed Source Address to NVS memory
    NodeAddress = SourceAddress;      // Set new Node Address (to save only once)
    preferences.begin("nvs", false);
    preferences.putInt("LastNodeAddress", SourceAddress);
    preferences.end();
    Serial.printf("Address Change: New Address=%d\n", SourceAddress);
  }
}


//*****************************************************************************
void NMEA0183_ParseMessages() {
  tNMEA0183Msg NMEA0183Msg;  // Create message container
  
  if (!NMEA0183.GetMessage(NMEA0183Msg)) return;  // New message ?

  // We do have a new message
  if (NMEA0183Msg.IsMessageCode("MWV")) HandleMWV(NMEA0183Msg);  // Check and handle message
}


/*
enum tNMEA0183WindReference {
                            NMEA0183Wind_True=0,
                            // Apparent Wind (relative to the vessel centerline)
                            NMEA0183Wind_Apparent=1
                          };

enum tN2kWindReference {
  // Details found on page 12 of https://www.rocktheboatmarinestereo.com/specs/MSNRX200I.pdf
                            // Theoretical Wind (ground referenced, referenced to True North; calculated using COG/SOG)
                            N2kWind_True_North=0,
                            // Theoretical Wind (ground referenced, referenced to Magnetic North; calculated using COG/SOG)
                            N2kWind_Magnetic=1,
                            // Apparent Wind (relative to the vessel centerline)
                            N2kWind_Apparent=2,
                            // Deprecated - We had the typo in older version of the library
                            N2kWind_Apprent=2,
                            // Theoretical (Calculated to Centerline of the vessel, referenced to ground; calculated using COG/SOG)
                            N2kWind_True_boat=3,
                            // Theoretical (Calculated to Centerline of the vessel, referenced to water; calculated using Heading/Speed through Water)
                            N2kWind_True_water=4,
                            N2kWind_Error=6,
                            N2kWind_Unavailable=7
                          };
*/


//*****************************************************************************
void HandleMWV(const tNMEA0183Msg &NMEA0183Msg) {
  double WindAngle;
  tNMEA0183WindReference Reference;
  tN2kWindReference WindReference;
  double WindSpeed;
  tN2kMsg N2kMsg;
  
  //Serial.println("MWV Message");
  
  // Parse MWV message (WindSpeed is in m/s !!!)
  if (!NMEA0183ParseMWV_nc(NMEA0183Msg, WindAngle, Reference, WindSpeed)) return;

  // Read/Set wind reference
  if(Reference == NMEA0183Wind_True) {
    WindReference =  N2kWind_True_boat;
  } else {
    WindReference =  N2kWind_Apparent;
  }

  //Serial.printf("Angle=%4.1f°, Speeed=%3.1f kn, Reference=%d\n", WindAngle, msToKnots(WindSpeed), Reference);
  
  // Create NMEA2000 message
  SetN2kWindSpeed(N2kMsg, 0, WindSpeed, DegToRad(WindAngle), WindReference);
  // Send message
  NMEA2000.SendMsg(N2kMsg);
}


//*****************************************************************************
void loop() {
  NMEA0183_ParseMessages(); 
  NMEA2000.ParseMessages();
  CheckSourceAddressChange();
}
