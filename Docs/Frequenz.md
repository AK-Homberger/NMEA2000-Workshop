# Messung von Frequenzen (Motordrehzahl)

Als Nächstes sehen wir uns an, wie wir mit dem ESP32 Frequenzen messen können.

Das Ziel ist es diesmal, die Motordrehzahl an der [Klemme "W" der Lichtmaschine](https://de.wikipedia.org/wiki/Lichtmaschine) zu messen und als PGN127488 (Engine Parameters, Rapid Update) zu senden.
    
Wir nutzen dazu die Interrupt-Funktion des ESP32. Interrupt bedeutet hier, dass der ESP32 auf Änderungen des logischen Signallevels reagiert und eine zuvor festgelegte Funktion ausführt. 

Für das Beispielprogramm nutzen wir Pin 27 als Eingang. Da wir für den Workshop keine Lichtmaschine zur Verfügung haben, nutzen wir einen anderen Pin des ESP32 (GPIO 26) als Signalgenerator. Die Drehzahl stellen wir mit dem Potentiometer ein, dass schon aus dem Beispiel zur Spannungsmessung gesteckt war. Zusätzlich stecken wir hier eine Brücke zwischen GPIO 26 und GPIO 27.

Das Steckbrett sollte dann so aussehen:

![Steckbrett-Frequenz](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-Frequenz_Steckplatine.png)

Um wirklich die Motordrehzahl messen zu können, würden wir noch einen [Optokoppler](https://de.wikipedia.org/wiki/Optokoppler), einen Widerstand und eine kleine [Diode](https://de.wikipedia.org/wiki/Diode) benötigen.

Die genaue Beschaltung ist im Repository [NMEA-2000-Data-Sender](https://github.com/AK-Homberger/NMEA2000-Data-Sender) dargestellt.


## Das Programm

Als Nächstes öffnen wir das Beispielprogramm [NMEA2000-Frequenz.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA2000-Frequenz/NMEA2000-Frequenz.ino) und laden es auf den rechten ESP32 hoch.

Danach starten wir wieder den NMEA-Reader und wählen den PGN 127488.

Anzeige der Motordrehzahl:

![NMEA-Reader7](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEAReader-7.png)

Durch drehen am Potentiometer können wir nun die angezeigte Motordrehzahl verändern (ca. 20 bis 4095).

Kommen wir nun zum Programm.

Die Basis ist wieder unser Standard-Beispielprogramm zum Senden eines Wertes.

Auch hier benötigen wir wieder einen Kalibrierungswert und die Festlegung des Eingang-Pins.
```
#define RPM_Calibration_Value 1.0 // Translates Generator RPM to Engine RPM 
#define Eingine_RPM_Pin 27  // Engine RPM is measured as interrupt on pin 27
```

Hier werden wieder die Sende-Offsets definiert. Diesmal senden wir häufiger. 333 ms bedeutet, drei Mal pro Sekunde senden. Das sehen wir auch im NMEA-Reader (Feld "Interval").
```
// Set time offsets
#define SlowDataUpdatePeriod 333  // Time between CAN Messages sent
#define RPM_SendOffset 0
```

Es folgt die Definition von Variablen für die Zeitmessungen mit der Interrupt-Funktion.

```
// Interrupt data
volatile uint64_t StartValue = 0;                 // First interrupt value
volatile uint64_t PeriodCount = 0;                // period in counts of 0.000001 of a second
volatile unsigned long Last_int_time = 0;         // Stores last Interrupt time
hw_timer_t * timer = NULL;                        // pointer to a variable of type hw_timer_t
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;  // To lock/unlock interrupt
```

Wie schon gewohnt, folgt die Definition der Sende-PGNs.
```
// Set the information for other bus devices, which messages we support
const unsigned long TransmitMessages[] PROGMEM = {127488L, // Engine Parameters, Rapid update                                                  
                                                  0
                                                 };
```




In setup() wird nun die Interrupt-Funktion für Pin 27 initialisiert:

```
// Init RPM measure
  pinMode(Eingine_RPM_Pin, INPUT_PULLUP);                                            // sets pin high
  attachInterrupt(digitalPinToInterrupt(Eingine_RPM_Pin), handleInterrupt, FALLING); // attaches pin to interrupt on Falling Edge
  timer = timerBegin(0, 80, true);                                                   // this returns a pointer to the hw_timer_t global variable
  // 0 = first timer
  // 80 is prescaler so 80MHZ divided by 80 = 1MHZ signal ie 0.000001 of a second
  // true - counts up
  timerStart(timer);   
 ```
 
Als Erstes wird mit pinMode() Pin 27 als Eingangs-Pin mit internem Pull-Up-Widerstand definiert. 

Als Nächstes folgt mit attachInterrupt() die Festlegung von Pin 27 als Interrupt. Es wird festgelegt, dass bei einem externen Signalwechsel an Pin27 von HIGH auf LOW (=FALLING) die Funktion "handleInterrupt" aufgerufen wird.

Im Folgenden wird ein ESP32 interner Timer definiert und gestartet. Den Timer benötigen wir später, um aus dem zeitlichen Abstand von zwei Interrupts auf die Frequenz zu schließen.

Als Letztes in setup() wird noch der Signalgenerator an Pin 26 vorbereitet.

```
  //*****************************************************************************
  // Only for frequency simulation in loop()
  ledcAttachPin(26, 1);     // sets GPIO26 as signal output (for simulation only)
  //*****************************************************************************
```
Das war es mit setup().

Hier wird die Funktion "handleInterrupt" definiert:
 
```
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
```
Wie schon erwähnt, wird diese Funktion immer dann aufgerufen, wenn das Signal an Pin 27 von HIGH auf LOW wechselt. 

Mit "PeriodCount = TempVal - StartValue;" wird die Zeitdifferenz seit dem letzten Interrupt berechnet.


Die nächste Funktion dient der Berechnung der Frequenz aus der letzen Zeitdifferenz (RPM = 1000000.00 / PeriodCount; )
```
// Calculate engine RPM from number of interupts per time
double ReadRPM() {
  double RPM = 0;

  portENTER_CRITICAL(&mux);
  if (PeriodCount != 0) RPM = 1000000.00 / PeriodCount; // PeriodCount in 0.000001 of a second
  if (millis() > Last_int_time + 500) RPM = 0;          // No signals RPM=0;
  portEXIT_CRITICAL(&mux);
  return (RPM);
}
```

Hier wird der Wert EngineRPM gemessen, kalibriert und mit dem [PGN127488](https://github.com/ttlappalainen/NMEA2000/blob/db22adbb3fec182ecae2ae8bc816378ac43d7fc3/src/N2kMessages.h#L204) gesendet:

```
void SendN2kEngineRPM(void) {
  static unsigned long SlowDataUpdated = InitNextUpdate(SlowDataUpdatePeriod, RPM_SendOffset);
  tN2kMsg N2kMsg;
  static double EngineRPM = 0;

  if ( IsTimeToUpdate(SlowDataUpdated) ) {
    SetNextUpdate(SlowDataUpdated, SlowDataUpdatePeriod);

    EngineRPM = ReadRPM() * RPM_Calibration_Value;

    // EngineRPM = (EngineRPM + (ReadRPM() * RPM_Calibration_Value)) / 2.0;  // Implements a low-pass filter

    Serial.printf("Engine RPM  :%4.0f EngineRPM \n", EngineRPM);

    SetN2kEngineParamRapid(N2kMsg, 0, EngineRPM, N2kDoubleNA, N2kInt8NA);

    NMEA2000.SendMsg(N2kMsg);
  }
}
```
Mit: "EngineRPM = (EngineRPM + (ReadRPM() * RPM_Calibration_Value)) / 2.0;" könnten wir für den gemessenen Wert einen Tiefpassfilter einsetzen, der Sprünge bei den Messwerten verringert.

Zur Kalibrierung benötigt man übrigens das Übersetzungsverhältnis zwischen Kurbelwelle und Lichtmachinenwelle. In der Praxis findet man den Wert aber durch Ausprobieren und Vergleich mit dem fest eingebauten Drehzahlmesser.

Sollte eure Lichtmaschine keinen "W"-Anschluss haben, könnt ihr auch eine kleine Reflex-Lichtschranke nutzen [Norbert](https://www.segeln-forum.de/board194-boot-technik/board35-elektrik-und-elektronik/board195-open-boat-projects-org/71890-motormanagement-diy/) hat das hier gut dargestellt.

Genau so gut könnten wir übrigens auch den Durchfluss mit einem [Durchflussgeber](https://www.roboter-bausatz.de/p/yf-s201-halleffekt-wasser-durchflusssensor-1-30l-min-g1-2) messen (Kalibrierungswert = 0,1333 für L/m).

So, nun können wir auch Frequenzen messen. Das Messen von Ereignissen (zum Beispiel [Kettenzählwerksimpulse](https://github.com/AK-Homberger/ESP32_AnchorChainControl_WLAN)) geht übrigens ganz ähnlich. In diesem Fall in der "handleInterrupt"-Funktion einfach die Ereignise hoch- oder runterzählen.

Die Erzeugung des Rechtecksignals an Pin 26 erfolgt übrigens in loop() mit folgenden Zeilen:
```
  // For frequency simulation only
  //**********************************
  static unsigned long timer = 0;
  if (millis() > timer  + 100) {
    timer = millis();
    ledcSetup(1, analogRead(34), 7);
    ledcWrite(1, 64);
  }
  //***********************************
```
Alle 100 Millisekunden wird der analoge Eingang an Pin 34 gemessen und als Frequenz an Pin 26 ausgegeben.

Im nächsten [Teil](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/ReadPGNs.md) werden wir etwas komplett anderes machen, und zur Abwechslung etwas vom NMEA2000-Bus lesen, anstatt zu schreiben.


