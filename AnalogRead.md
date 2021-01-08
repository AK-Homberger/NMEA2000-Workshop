# Spannungs- und Widerstandsmessung (Bordspannung, Tankanzeige)

Als nächstes sehen wir uns an, wie wir mit dem ESP32 Spannungen und Widerstände messen können. Ziel ist es:

1. Die die Bordspannung zu messen und als PGN127508 zu senden.
2. Den Tanklevel des Wasertanks zu messen und als PGN127505 zu senden.

Wir nutzen dazu die Analog-Digital-Converter (ADC) Funktionen des ESP32. Der ESP32 hat zwei unabhängige ADCs, die durch 18 unterschiedliche Pins angesteuert werden können.

Wir nutzen hier die Pins 34 und 35 (ADC1_CH6 und ADC1_CH7). Zur Vereinfachung verwenden wir im Workshop ein einfaches 10 KOhm Potentiometer, um sowohl die Bordspannung als auch den Tanklevel zu simulieren.

So muss das Steckbrett dazu aussehen:

![AnalogRead](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-V-R_Steckplatine.png)

Das Potentiometer wird, wie oben gezeigt, mit 3,3 V, GND und den Pins 34/35 verbunden.

Um wirklich die Boardspannung messen zu können, würde ein Spannungsteiler mit zwei Widerständen (100 KOhm / 27 KOhm) verwendet werden.
Um den Tanklevel zum Beispiel mit einem Tankgeber von Pilippi (0 - 180 Ohm) zu messen, wäre ein Widerstand von 1 KOhm nötig (und ggf. eine Z-Diode zum Schutz des Eingangs vor Überspannung).

Die genaue Beschaltung ist im Repository [NMEA-2000-Data-Sender](https://github.com/AK-Homberger/NMEA2000-Data-Sender) dargestellt.

Kommen wir num zum Programm.

Aus dem enpackten Download-Verzeichnis laden wir num das Programm [NMEA2000-ADC.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/NMEA2000-ADC/NMEA2000-ADC.ino).

Nach dem Hochladen auf dem rechten ESP32 können wir wieder den NMEA-Reader starten und sehen, was auf dem Bus gesendet wird.

Hier sehen wir die Bordspannung (12.12 Volt) als PGN127508:

![Spannung](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEAReader-5.png)

Und hier sehen wir den Wassertanklevel (88.3 %) als PGN127505

![TankLevel](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEAReader-6.png)

Das Programm selbst unterscheidet sich nicht sehr von den bisherigen Beispielprogrammen.

Neu ist:

```
#define ADCpin1 34                   // GPIO 34 (Analog input, ADC1_CH6)
#define ADC_Calibration_Value1 250.0 // For resistor measure 5 Volt and 180 Ohm equals 100% plus 1K resistor.

#define ADCpin2 35                   // GPIO 35 (Analog input, ADC1_CH7))
#define ADC_Calibration_Value2 34.3  // To measure battery voltage. The real value depends on the true resistor values for the ADC input (100K / 27 K).
```

Hier werden die Pins für die beiden ADC-Eingänge festgelegt (GPIO34 und 35).
Mit ADC_Calibration_Value1 und ADC_Calibration_Value2 werden Korrekturwerte für die Spannung und die Widerstandsmessung festgelegt.
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

Mit dieser Funktion wird der Analoge Eingang des ADCs ausgelesen und korrigiert. Dieser Schritt ist leider notwendig, weil der ADC des ESP32 nicht sehr linear ist. Auf der oben dargestellten [Web-Seite](https://github.com/G6EJD/ESP32-ADC-Accuracy-Improvement-function) wird der Hintergrund erklärt.





                                                






