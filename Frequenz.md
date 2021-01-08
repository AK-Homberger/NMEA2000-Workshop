# Messung von Frequenzen (Motordrehzahl)

Als nächstes sehen wir uns an, wie wir mit dem ESP32 Frequenzen messen können.

Das Ziel ist es diesmal, die Motordrehzahl an der Klemme "W" der Lichtmaschine zu messen und als PGN127488 (Engine Parameters, Rapid update) zu senden.
    
Wir nutzen dazu die Interrupt-Funktion des ESP32. Interrupt bedeutet hier, dass der ESP32 auf Änderungen des logischen Signallevels reagiert und eine zuvor festgelegte Funktion ausführt. 

Für das Beispielprogramm nutzen wir Pin 27 als Eingang. Da wir für den Workshop keine Lichtmaschine zur Verfügung haben, nutzen wir einen kleinen Taster zur Simulation der Klemme "W".

Das Steckbrett sollte so aussehen:

![Steckbrett-Frequenz](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-Frequenz_Steckplatine.png)

Der Taster auf dem Steckbrett wird mit GND und Pin 27 verbunden. Solltet ihr keinen Taster haben, so macht das auch nichts. In diesem Fall einfach zwei Stecklitzen mit GND und Pin 27 verbinden. Durch kurzes zusmmenfügen und lösen der beiden offenen Kabelenden können Tastendrücke simuliert werden.

Um wirklich die Motordrehzahl messen zu können, würden wir noch einen Optokoppler, einen Widerstand und eine kleine Diode benötigen.

Die genaue Beschaltung ist im Repository [NMEA-2000-Data-Sender](https://github.com/AK-Homberger/NMEA2000-Data-Sender) dargestellt.


# Das Programm

Als nächstes öffnen wir das Beispielprogramm [NMEA2000-Frequenz.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/NMEA2000-Frequenz/NMEA2000-Frequenz.ino) und laden es auf den rechten ESP32 hoch.

Danach starten wir wieder den NMEA-Reader und wählen en PGN 127488.

Anzeige des Motordrehzahl:

![NMEA-Reader7](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEAReader-7.png)

Durch schnelles, wiederholtes drücken des Tasters können wir nun die angezeigte Motordrehzahl verändern.

Kommen wir nun zum Programm.

Die Basis ist wieder unser Standard-Beispielprogramm zum Senden eines Wertes.

Auch hier benötigen wir wieder einen Kalibrierungswert und die Festlegung des Eingangs-Pins.
```
#define RPM_Calibration_Value 1.0 // Translates Generator RPM to Engine RPM 
#define Eingine_RPM_Pin 27  // Engine RPM is measured as interrupt on pin 27
```

Hier werden wieder die Sende-Offsets definiert. Diesmal senden wir häufiger. 333 ms bedeutet drei Mal pro Sekunde senden. Das sehen wir auch im NMEA-Reader (Feld "Interval").
```
// Set time offsets
#define SlowDataUpdatePeriod 333  // Time between CAN Messages sent
#define RPM_SendOffset 0
```

Es folgt die Definition von Variablen für die Zeitmessungen mit der Interrupt-Funktion.

```
// Interrupt data
volatile uint64_t StartValue;                     // First interrupt value
volatile uint64_t PeriodCount;                    // period in counts of 0.000001 of a second
unsigned long Last_int_time = 0;                  // Stores last Interrupt time
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

In setup() wird nun die Interupt-Funktion für Pin 27 initialisiert:

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
 
Als erstes wird mit pinMode() Pin 27 als Eingangs-Pin mit internem Pull-Up-Widerstand definiert. Das erspart uns einen externen Widerstand auf dem Steckbrett.
Als nächstes folgt mit attachInterrupt() die Festlegung von Pin 27 als Interrupt. Es wird festgelegt, dass bei einem externen Signalwechsel an Pin27 von HIGH auf LOW (=FALLING) die Funktion "handleInterrupt" aufgerufen wird.
Im folgenden wird ein ESP32 interner Timer definiert und gestartet. Den Timer benötigen wir später um aus dem zeitlichen Abtand von zwei Interrupts auf die Frequenz zu schließen.

Hier wird die Funktion "handleInterrupt" definiert:
 
 ```
// RPM Event Interrupt
// Enters on falling edge
void IRAM_ATTR handleInterrupt()
{
  portENTER_CRITICAL_ISR(&mux);
  uint64_t TempVal = timerRead(timer);        // value of timer at interrupt
  PeriodCount = TempVal - StartValue;         // period count between rising edges in 0.000001 of a second
  StartValue = TempVal;                       // puts latest reading as start for next calculation
  portEXIT_CRITICAL_ISR(&mux);
  Last_int_time = millis();
}
```
Wie schon erwähnt, wird diese Funktion immer dann aufgerufen, wenn das Signal an Pin 27 von HIGH auf LOW wechselt. Also immer dann, wenn wir den Taster drücken. Durch den interne Pull-Up-Widertand ist ohne gedrückten Taster Pin 27 auf HIGH-Niveau. Durch Tastendruck wird der Pin auf GND geschaltet, was LOW entspricht.

Mit "PeriodCount = TempVal - StartValue;" wird die Zeitdifferenz seit dem letzten Interrupt berechnet.


Die nächste Funktion dient der Berechnung der Frequenz aus der letzen Zeitdifferenz (RPM = 1000000.00 / PeriodCount; )
```
// Calculate engine RPM from number of interupts per time
double ReadRPM() {
  double RPM = 0;

  portENTER_CRITICAL(&mux);
  RPM = 1000000.00 / PeriodCount;                    // PeriodCount in 0.000001 of a second
  portEXIT_CRITICAL(&mux);
  if (millis() > Last_int_time + 200) RPM = 0;       // No signals RPM=0;
  return (RPM);
}

```

Hier wird der Wert EngineRPM gemessen, kalibriert und mit dem PGN127488 gesendet:

```
void SendN2kEngineRPM(void) {
  static unsigned long SlowDataUpdated = InitNextUpdate(SlowDataUpdatePeriod, RPM_SendOffset);
  tN2kMsg N2kMsg;
  double EngineRPM;
  
  if ( IsTimeToUpdate(SlowDataUpdated) ) {
    SetNextUpdate(SlowDataUpdated, SlowDataUpdatePeriod);

    EngineRPM = ReadRPM() * RPM_Calibration_Value;
    
    Serial.printf("Engine RPM  :%4.0f EngineRPM \n", EngineRPM);

    SetN2kEngineParamRapid(N2kMsg, 0, EngineRPM, N2kDoubleNA, N2kInt8NA);

    NMEA2000.SendMsg(N2kMsg);
  }
}
```
Zur Kalibrierung benötigt man übrigens das Übersetzungsverhältnis zwischen Kubelwelle und Lichtmachienenwelle. In der Praxis findet man den Wert aber durch Ausprobieren und Vergleich mit dem fest eingebauten Drehzahlmesser.

So, nun können wir auch Frequenzen messen. Das Messen von Ereignissen (zum Beispie Kettenzählwerksimpulse) geht übrigens ganz ähnlich. In diesem Fall in der "handleInterrupt"-Funktion einfach die Ereignise hoch- oder runterzählen.

Im nächsten Teil werden wir etwas komplett anderes machen, und zur Abwechslung etwas vom NMEA2000-Bus lesen anstatt zu schreiben.


