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

// NMEA2000 Frequenz messen (Motordrehzahl) mit Interrupt
// Version 0.1, 08.01.2021, AK-Homberger

#define ESP32_CAN_TX_PIN GPIO_NUM_5  // Set CAN TX port to 5 
#define ESP32_CAN_RX_PIN GPIO_NUM_4  // Set CAN RX port to 4

#include <Arduino.h>
#include <Preferences.h>
#include <NMEA2000_CAN.h>  // This will automatically choose right CAN library and create suitable NMEA2000 object
#include <N2kMessages.h>

#define RPM_Calibration_Value 1.0 // Translates Generator RPM to Engine RPM 
#define Eingine_RPM_Pin 27  // Engine RPM is measured as interrupt on pin 27

// Set time offsets
#define SlowDataUpdatePeriod 333  // Time between CAN Messages sent
#define RPM_SendOffset 0

// Interrupt data
volatile uint64_t StartValue;                     // First interrupt value
volatile uint64_t PeriodCount;                    // period in counts of 0.000001 of a second
unsigned long Last_int_time = 0;                  // Stores last Interrupt time
hw_timer_t * timer = NULL;                        // pointer to a variable of type hw_timer_t
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;  // To lock/unlock interrupt

int NodeAddress;            // To store last Node Address
Preferences preferences;    // Nonvolatile storage on ESP32 - To store LastDeviceAddress


// Set the information for other bus devices, which messages we support
const unsigned long TransmitMessages[] PROGMEM = {127488L, // Engine Parameters, Rapid update
                                                  0
                                                 };

// Forward declaration to avoid compilation error
void IRAM_ATTR handleInterrupt();

//*****************************************************************************
void setup() {
  uint8_t chipid[6];
  uint32_t id = 0;
  int i = 0;

  // Init USB serial port
  Serial.begin(115200);
  delay(10);

  // Init RPM measure
  pinMode(Eingine_RPM_Pin, INPUT_PULLUP);                                            // sets pin high
  attachInterrupt(digitalPinToInterrupt(Eingine_RPM_Pin), handleInterrupt, FALLING); // attaches pin to interrupt on Falling Edge
  timer = timerBegin(0, 80, true);                                                   // this returns a pointer to the hw_timer_t global variable
  // 0 = first timer
  // 80 is prescaler so 80MHZ divided by 80 = 1MHZ signal ie 0.000001 of a second
  // true - counts up
  timerStart(timer);

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
                                130, // Device function=Temperature. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                75, // Device class=Sensor Communication Interface. See codes on  http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
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

//*****************************************************************************
// RPM Event Interrupt
// Enters on falling edge
void IRAM_ATTR handleInterrupt()
{
  portENTER_CRITICAL_ISR(&mux);
  uint64_t TempVal = timerRead(timer);        // value of timer at interrupt
  PeriodCount = TempVal - StartValue;         // period count between falling edges in 0.000001 of a second
  StartValue = TempVal;                       // puts latest reading as start for next calculation
  Last_int_time = millis();
  portEXIT_CRITICAL_ISR(&mux);
}


//*****************************************************************************
// Calculate engine RPM from number of interupts per time
double ReadRPM() {
  double RPM = 0;

  portENTER_CRITICAL(&mux);
  if (PeriodCount != 0) RPM = 1000000.00 / PeriodCount;                   // PeriodCount in 0.000001 of a second
  portEXIT_CRITICAL(&mux);
  if (millis() > Last_int_time + 500) RPM = 0;       // No signals RPM=0;
  return (RPM);
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


void SendN2kEngineRPM(void) {
  static unsigned long SlowDataUpdated = InitNextUpdate(SlowDataUpdatePeriod, RPM_SendOffset);
  tN2kMsg N2kMsg;
  static double EngineRPM = 0;

  if ( IsTimeToUpdate(SlowDataUpdated) ) {
    SetNextUpdate(SlowDataUpdated, SlowDataUpdatePeriod);

    if (ReadRPM() < 5000) {
      EngineRPM = ReadRPM() * RPM_Calibration_Value;
    }

    Serial.printf("Engine RPM  :%4.0f EngineRPM \n", EngineRPM);

    SetN2kEngineParamRapid(N2kMsg, 0, EngineRPM, N2kDoubleNA, N2kInt8NA);

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

  SendN2kEngineRPM();

  NMEA2000.ParseMessages();

  CheckSourceAddressChange();

  // Dummy to empty input buffer to avoid board to stuck with e.g. NMEA Reader
  if ( Serial.available() ) {
    Serial.read();
  }
}
