# Spannungs- und Widerstandsmessung (Batteriespannung, Tankanzeige)

Als nächstes sehen wir uns an, wie wir mit dem ESP32 Spannungen und Widerstände messen können. Ziel ist es:

1. Die die Batteriespannung zu messen und als PGN127508 zu senden.
2. Den Tanklevel des Wasertanks zu messen und als PGN127505 zu senden.

Wir nutzen dazu die Analog-Digital-Converter (ADC) Funktionen des ESP32. Der ESP32 hat zwei unabhängige ADCs, die durch 18 unterschiedliche Pins angesteuert werden können.

Wir nutzen hier die Pins 34 und 35 (ADC1_CH6 und ADC1_CH7). Zur Vereinfachung verwenden wir im Workshop ein einfaches 10 KOhm Potentiometer, um sowohl die Bordspannung als auch den Tanklevel zu simulieren.

## Schaltung
So muss das Steckbrett dazu aussehen:

![AnalogRead](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-V-R_Steckplatine.png)

Das Potentiometer wird, wie oben gezeigt, mit 3,3 V, GND und den Pins 34/35 verbunden.

Um wirklich die Batteriespannung messen zu können, würde ein Spannungsteiler mit zwei Widerständen (100 KOhm / 27 KOhm) verwendet werden.
Um den Tanklevel zum Beispiel mit einem Tankgeber von Philippi (0 - 180 Ohm) zu messen, wäre ein Widerstand von 1 KOhm nötig (und ggf. eine Z-Diode zum Schutz des Eingangs vor Überspannung).

Die genaue Beschaltung ist im Repository [NMEA-2000-Data-Sender](https://github.com/AK-Homberger/NMEA2000-Data-Sender) dargestellt.


## Das Programm
Kommen wir nun zum Programm.

Aus dem enpackten Download-Verzeichnis laden wir nun das Programm [NMEA2000-ADC.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/NMEA2000-ADC/NMEA2000-ADC.ino).

Nach dem Hochladen auf dem rechten ESP32 können wir wieder den NMEA-Reader starten und sehen, was auf dem Bus gesendet wird.

Hier sehen wir die Batteriespannung (12.12 Volt) als PGN127508:

![Spannung](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEAReader-5.png)

Und hier sehen wir den Wassertanklevel (88.3 %) als PGN127505

![TankLevel](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEAReader-6.png)

Wenn wir nun am Potentiometer drehen, verändern sich die Spannungen and den ADC-Eingängen und damit auch die angezeigten Werte entsprechend.

Das Programm selbst unterscheidet sich nicht sehr von den bisherigen Beispielprogrammen.

Neu ist:

```
#define ADCpin1 34                   // GPIO 34 (Analog input, ADC1_CH6)
#define ADC_Calibration_Value1 250.0 // For resistor measure 5 Volt and 180 Ohm equals 100% plus 1K resistor.

#define ADCpin2 35                   // GPIO 35 (Analog input, ADC1_CH7))
#define ADC_Calibration_Value2 34.3  // To measure battery voltage. The real value depends on the true resistor values for the ADC input (100K / 27 K).
```

Hier werden die Pins für die beiden ADC-Eingänge festgelegt (GPIO 34 und 35).
Mit ADC_Calibration_Value1 und ADC_Calibration_Value2 werden Korrekturwerte für die Spannungs- und die Widerstandsmessung festgelegt.
Wir sehen später, wie sie verwendet werden.

Hier sehen wir die gewohnte Definition der Sende-Offsets:

```
// Set time offsets
#define SlowDataUpdatePeriod 1000  // Time between CAN Messages sent
#define BatteryVoltageSendOffset 0
#define WaterTankLevelSendOffset 50
```

Und der PGN-Liste:

```
const unsigned long TransmitMessages[] PROGMEM = {127508L, // Battery Status
                                                  127505L, // Fluid level
                                                  0
                                                 };
```

Wirklich neu ist dieser Teil:
```
// ReadADC is used to improve the linearity of the ESP32 ADC see: https://github.com/G6EJD/ESP32-ADC-Accuracy-Improvement-function
double ReadADC(byte pin) {
  double reading = analogRead(pin); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
  if (reading < 1 || reading > 4095) return 0;
  // return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
  return (-0.000000000000016 * pow(reading, 4) + 0.000000000118171 * pow(reading, 3) - 0.000000301211691 * pow(reading, 2) + 0.001109019271794 * reading + 0.034143524634089) * 1000;
} // Added an improved polynomial, use either, comment out as required
```

Mit dieser Funktion wird der analoge Eingang des ADCs ausgelesen und korrigiert. Dieser Schritt ist notwendig, weil der ADC des ESP32 nicht sehr linear ist. Mit der oben berechneten Korrektur (Pylynominalfunktion) erreichen wir eine Genauigkeit von ca. 1 %. Das ist für unsere Zwecke meist ausreichend.


Hier kommen nun die beiden Funktionen zum Messen der Werte und Senden der PGNs:

Batteriespannung:

```
void SendN2kBatteryVoltage(void) {
  static unsigned long SlowDataUpdated = InitNextUpdate(SlowDataUpdatePeriod, BatteryVoltageSendOffset);
  tN2kMsg N2kMsg;
  double BatteryVoltage;

  if ( IsTimeToUpdate(SlowDataUpdated) ) {
    SetNextUpdate(SlowDataUpdated, SlowDataUpdatePeriod);

    BatteryVoltage = ReadADC(ADCpin2) * ADC_Calibration_Value2 / 4096;

    Serial.printf("Battery Voltage: %3.1f V \n", BatteryVoltage);

    // Definition from N2kMessages.h
    // void SetN2kPGN127508(tN2kMsg &N2kMsg, unsigned char BatteryInstance, double BatteryVoltage, double BatteryCurrent=N2kDoubleNA,
    //                 double BatteryTemperature=N2kDoubleNA, unsigned char SID=1);

    // Set N2K message
    SetN2kPGN127508(N2kMsg, 0, BatteryVoltage, N2kDoubleNA, N2kDoubleNA, 0);

    // Send message
    NMEA2000.SendMsg(N2kMsg);
  }
}
```

Wassertanklevel:

```
void SendN2kWaterTankLevel(void) {
  static unsigned long SlowDataUpdated = InitNextUpdate(SlowDataUpdatePeriod, WaterTankLevelSendOffset);
  tN2kMsg N2kMsg;
  double TankLevel;

  if ( IsTimeToUpdate(SlowDataUpdated) ) {
    SetNextUpdate(SlowDataUpdated, SlowDataUpdatePeriod);

    TankLevel = ReadADC(ADCpin1) * ADC_Calibration_Value1 / 4096;

    Serial.printf("Tank Level: %3.1f %% \n", TankLevel);

    // Definition from N2kMessages.h
    // void SetN2kPGN127505(tN2kMsg &N2kMsg, unsigned char Instance, tN2kFluidType FluidType, double Level, double Capacity);

    // FluidType is defined in N2kTypes.h

    // Set N2K message
    SetN2kPGN127505(N2kMsg, 0, N2kft_Water, TankLevel, 100);

    // Send message
    NMEA2000.SendMsg(N2kMsg);
  }
}
````

der Aufbau der beiden Funktionen unterscheidet sich nicht sehr von den bisherigen Beispielen.

Wichtig sind die beiden Zeilen:

1. BatteryVoltage = ReadADC(ADCpin2) * ADC_Calibration_Value2 / 4096;

2. TankLevel = ReadADC(ADCpin1) * ADC_Calibration_Value1 / 4096;

Hier sehen wir, wie der ADC-Wert (0 - 4095) mit dem Kalibrierungswert korrigiert wird, um die gewünschte Anzeige zu erhalten.

Das war es schon zum Thema Spannungs- und Widerstandsmessung.

Es wäre ein Leichtes, das Programm um zusätzliche Tanks und Batteriespannungen zu erweitern.
Die notwendigen Schritte hatten wir ja schon beim BME280 geübt.

Im nächsten [Teil](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Frequenz.md) geht es um Frequenzmessung (Motordrehzahl).

