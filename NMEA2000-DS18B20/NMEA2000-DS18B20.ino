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

// NMEA2000 Temperature mit DS18B20
// Version 0.1, 06.01.2021, AK-Homberger

#define ESP32_CAN_TX_PIN GPIO_NUM_5  // Set CAN TX port to 5 
#define ESP32_CAN_RX_PIN GPIO_NUM_4  // Set CAN RX port to 4

#include <Arduino.h>
#include <Preferences.h>
#include <NMEA2000_CAN.h>  // This will automatically choose right CAN library and create suitable NMEA2000 object
#include <N2kMessages.h>

// Include file for DS18B20
#include <DallasTemperature.h>

// Set time offsets
#define SlowDataUpdatePeriod 1000  // Time between CAN Messages sent
#define TempSendOffset 0

// 1-Wire connection for temperature (Dallas DS18B20) is plugged into GPIO13 on the ESP32
#define ONE_WIRE_BUS 13

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

double Temperature = 0;     // Variable to store Temperature

int NodeAddress;            // To store last Node Address
Preferences preferences;    // Nonvolatile storage on ESP32 - To store LastDeviceAddress

// Task handle for OneWire read (Core 0 on ESP32)
TaskHandle_t Task1;

// Set the information for other bus devices, which messages we support
const unsigned long TransmitMessages[] PROGMEM = {130312L, // Temperature
                                                 0
                                                 };


//*****************************************************************************
void setup() {
  uint8_t chipid[6];
  uint32_t id = 0;
  int i = 0;

  // Init USB serial port
  Serial.begin(115200);
  delay(10);

  // Start OneWire for DS18B20
    sensors.begin();
  
  // Create GetTemperature task for core 0, loop() runs on core 1
  xTaskCreatePinnedToCore(
    GetTemperature, /* Function to implement the task */
    "Task1", /* Name of the task */
    10000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &Task1,  /* Task handle. */
    0); /* Core where the task should run */
  
  // Reserve enough buffer for sending all messages. This does not work on small memory devices like Uno or Mega

  NMEA2000.SetN2kCANMsgBufSize(8);
  NMEA2000.SetN2kCANReceiveFrameBufSize(150);
  NMEA2000.SetN2kCANSendFrameBufSize(150);

  // Generate unique number from chip id
  esp_efuse_read_mac(chipid);
  for (i = 0; i < 6; i++) id += (chipid[i] << (7 * i));

  // Set product information
  NMEA2000.SetProductInformation("1", // Manufacturer's Model serial code
                                 100, // Manufacturer's product code
                                 "My Sensor Module",  // Manufacturer's Model ID
                                 "1.0.2.25 (2019-07-07)",  // Manufacturer's Software version code
                                 "1.0.2.0 (2019-07-07)" // Manufacturer's Model version
                                );
  // Set device information
  NMEA2000.SetDeviceInformation(id, // Unique number. Use e.g. Serial number.
                                130, // Device function=Temperature. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                75, // Device class=Sensor Communication Interface. See codes on  http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                               );

  // If you also want to see all traffic on the bus use N2km_ListenAndNode instead of N2km_NodeOnly below

  NMEA2000.SetForwardType(tNMEA2000::fwdt_Text); // Show in clear text. Leave uncommented for default Actisense format.

  preferences.begin("nvs", false);                          // Open nonvolatile storage (nvs)
  NodeAddress = preferences.getInt("LastNodeAddress", 34);  // Read stored last NodeAddress, default 34
  preferences.end();
  Serial.printf("NodeAddress=%d\n", NodeAddress);

  // If you also want to see all traffic on the bus use N2km_ListenAndNode instead of N2km_NodeOnly below
  NMEA2000.SetMode(tNMEA2000::N2km_NodeOnly, NodeAddress);
  NMEA2000.ExtendTransmitMessages(TransmitMessages);

  NMEA2000.Open();
  
  delay(200);
}


//*****************************************************************************
// This task runs isolated on core 0 because sensors.requestTemperatures() is slow and blocking for about 750 ms

void GetTemperature( void * parameter) {
  float tmp = 0;
  for (;;) {   // Endless loop
    sensors.requestTemperatures(); // Send the command to get temperatures
    vTaskDelay(100);
    tmp = sensors.getTempCByIndex(0);
    //if (tmp != -127) Temperature = tmp; 
    Temperature=tmp;
    vTaskDelay(100);
  }
}


// Functions to control send interval and time offets

//*****************************************************************************
bool IsTimeToUpdate(unsigned long NextUpdate) {
  return (NextUpdate < millis());
}


//*****************************************************************************
unsigned long InitNextUpdate(unsigned long Period, unsigned long Offset = 0) {
  return millis() + Period + Offset;
}


//*****************************************************************************
void SetNextUpdate(unsigned long & NextUpdate, unsigned long Period) {
  while ( NextUpdate < millis() ) NextUpdate += Period;
}


//*****************************************************************************
void SendN2kTemperature(void) {
  static unsigned long SlowDataUpdated = InitNextUpdate(SlowDataUpdatePeriod, TempSendOffset);
  tN2kMsg N2kMsg;

  if ( IsTimeToUpdate(SlowDataUpdated) ) {
    SetNextUpdate(SlowDataUpdated, SlowDataUpdatePeriod);
        
    Serial.printf("Temperature: %3.1f °C \n", Temperature);

    // Definition from N2kMessages.h
    // void SetN2kPGN130312(tN2kMsg &N2kMsg, unsigned char SID, unsigned char TempInstance, tN2kTempSource TempSource,
    //        double ActualTemperature, double SetTemperature=N2kDoubleNA);

    // tN2kTempSource is defined in N2kTypes.h

    // Set N2K message
    SetN2kPGN130312(N2kMsg, 0, 0, N2kts_ExhaustGasTemperature, CToKelvin(Temperature), N2kDoubleNA);
    
    // Send message
    NMEA2000.SendMsg(N2kMsg);
  }
}


//*****************************************************************************
// Function to check if SourceAddress has changed (due to address conflict on bus)

void CheckSourceAddressChange() {
  int SourceAddress = NMEA2000.GetN2kSource();

  if (SourceAddress != NodeAddress) { // Save potentially changed Source Address to NVS memory
    preferences.begin("nvs", false);
    preferences.putInt("LastNodeAddress", SourceAddress);
    preferences.end();
    Serial.printf("Address Change: New Address=%d\n", SourceAddress);
  }
}


//*****************************************************************************
void loop() {
  
  SendN2kTemperature();
  NMEA2000.ParseMessages();
  CheckSourceAddressChange();

  // Dummy to empty input buffer to avoid board to stuck with e.g. NMEA Reader
  if ( Serial.available() ) {
    Serial.read();
  }
}
