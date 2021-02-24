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

// NMEA2000 Temperature, Humidity and Pressure with BME280 and SignalK
// Version 0.1, 08.02.2021, AK-Homberger

#define ESP32_CAN_TX_PIN GPIO_NUM_5  // Set CAN TX port to 5 
#define ESP32_CAN_RX_PIN GPIO_NUM_4  // Set CAN RX port to 4

#include <Arduino.h>
#include <Preferences.h>
#include <NMEA2000_CAN.h>  // This will automatically choose right CAN library and create suitable NMEA2000 object
#include <N2kMessages.h>

// Include files for BME280
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "EspSigK.h"        // For SignalK handling

// Set time offsets
#define SlowDataUpdatePeriod 1000  // Time between CAN Messages sent
#define TempSendOffset 0           // + 0 ms
#define HumiditySendOffset 50      // + 50 ms
#define PressureSendOffset 100     // + 100 ms

Adafruit_BME280 bme; // I2C 

// Connect VIN of the BME280 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL GPIO 22
// Connect SDA GPIO 21

const String hostname  = "NMEA2000-Gateway";    //Hostname for network discovery
const String ssid      = "ssid";     //SSID to connect to
const String ssidPass  = "password";  // Password for wifi

EspSigK sigK(hostname, ssid, ssidPass); // create the object

int NodeAddress;            // To store last Node Address
Preferences preferences;    // Nonvolatile storage on ESP32 - To store LastDeviceAddress

// Set the information for other bus devices, which messages we support
const unsigned long TransmitMessages[] PROGMEM = {130312L, // Temperature
                                                  130313L, // Humidity
                                                  130314L, // Pressure
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

  // Init BME280 I2C address depends on sensor 0x76 or 0x77.
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1) {}
  }

  // SignalK settings
  sigK.setPrintDebugSerial(false);       // Default false, causes debug messages to be printed to Serial (connecting etc)
  sigK.setPrintDeltaSerial(false);       // Default false, prints deltas to Serial.
  //sigK.setServerHost("192.168.0.20");  // Optional. Sets the ip of the SignalKServer to connect to. If not set we try to discover server with mDNS
  //sigK.setServerPort(3000);            // If manually setting host, this sets the port for the signalK Server (default 80);

  //sigK.setServerToken("secret"); // if you have security enabled in node server, it wont accept deltas unles you auth
  // add a user via the admin console, and then run the "signalk-generate-token" script
  // included with signalk to generate the string. (or disable security)

  sigK.begin();                          // Start everything. Connect to wifi, setup services, etc...

  // Reserve enough buffer for sending all messages.
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
                                132, // Device function=Analog to NMEA 2000 Gateway. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                25, // Device class=Inter/Intranetwork Device. See codes on  http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                               );

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
  double Temperature;

  if ( IsTimeToUpdate(SlowDataUpdated) ) {
    SetNextUpdate(SlowDataUpdated, SlowDataUpdatePeriod);

    Temperature = bme.readTemperature();    
    Serial.printf("Temperature: %3.1f °C \n", Temperature);
    sigK.sendDelta("environment.inside.temperature", CToKelvin(Temperature));    

    // Definition from N2kMessages.h
    // void SetN2kPGN130312(tN2kMsg &N2kMsg, unsigned char SID, unsigned char TempInstance, tN2kTempSource TempSource,
    //        double ActualTemperature, double SetTemperature=N2kDoubleNA);

    // tN2kTempSource is defined in N2kTypes.h

    // Set N2K message
    SetN2kPGN130312(N2kMsg, 0, 0, N2kts_MainCabinTemperature, CToKelvin(Temperature), N2kDoubleNA);
    
    // Send message
    NMEA2000.SendMsg(N2kMsg);
  }
}


//*****************************************************************************
void SendN2kHumidity(void) {
  static unsigned long SlowDataUpdated = InitNextUpdate(SlowDataUpdatePeriod, HumiditySendOffset);
  tN2kMsg N2kMsg;
  double Humidity;        

  if ( IsTimeToUpdate(SlowDataUpdated) ) {
    SetNextUpdate(SlowDataUpdated, SlowDataUpdatePeriod);
        
    Humidity = bme.readHumidity();
    Serial.printf("Humidity: %3.1f %% \n", Humidity);
    sigK.sendDelta("environment.inside.relativeHumidity", Humidity);    

    // Definition from N2kMessages.h
    // SetN2kPGN130313(tN2kMsg &N2kMsg, unsigned char SID, unsigned char HumidityInstance,
    //                 tN2kHumiditySource HumiditySource, double ActualHumidity, double SetHumidity=N2kDoubleNA);
    
    // HumiditySource is defined in N2kTypes.h

    // Set N2K message
    SetN2kPGN130313(N2kMsg, 0, 0, N2khs_InsideHumidity, Humidity, N2kDoubleNA);
    
    // Send message
    NMEA2000.SendMsg(N2kMsg);
  }
}


//*****************************************************************************
void SendN2kPressure(void) {
  static unsigned long SlowDataUpdated = InitNextUpdate(SlowDataUpdatePeriod, PressureSendOffset);
  tN2kMsg N2kMsg;
  double Pressure;        

  if ( IsTimeToUpdate(SlowDataUpdated) ) {
    SetNextUpdate(SlowDataUpdated, SlowDataUpdatePeriod);
        
    Pressure = bme.readPressure() /100;  // Read and convert to mBar 
    Serial.printf("Pressure: %3.1f mBar \n", Pressure);
    sigK.sendDelta("environment.inside.pressure", mBarToPascal(Pressure));    

    // Definition from N2kMessages.h
    // SetN2kPGN130314(tN2kMsg &N2kMsg, unsigned char SID, unsigned char PressureInstance,
    //                 tN2kPressureSource PressureSource, double Pressure);
    
    // PressureSource is defined in N2kTypes.h

    // Set N2K message
    SetN2kPGN130314(N2kMsg, 0, 0, N2kps_Atmospheric, mBarToPascal(Pressure));
    
    // Send message
    NMEA2000.SendMsg(N2kMsg);
  }
}


//*****************************************************************************
// Function to check if SourceAddress has changed (due to address conflict on bus)

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
void loop() {

  SendN2kTemperature();
  
  SendN2kHumidity();

  SendN2kPressure();

  sigK.handle();

  NMEA2000.ParseMessages();

  CheckSourceAddressChange();
  
  // Dummy to empty input buffer to avoid board to stuck with e.g. NMEA Reader
  if ( Serial.available() ) {
    Serial.read();
  }
}
