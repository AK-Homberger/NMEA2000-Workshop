# NMEA0183 nach NMEA2000 wandeln
Manchmal möcht man auch NMEA0183-Nachrichten zu NMEA2000 wandeln. Ach dies geht sehr einfach mit den Bibliotheken von Timo Lappalainen.
Als Beispiel werden wir Wind-Daten, die als MWV-Datensatz vorliegen, nach NMEA2000 wandeln. Prinzipiell kann man nach diesem Schama alle NMEA0183-Datensätze zu NMEA2000 umsetzen.

Im [Programm](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA0183WindToN2k/NMEA0183WindToN2k.ino) benötigen wir als erstes zwei zusätzliche Include-Dateien:

```
#include <NMEA0183.h>
#include <NMEA0183Messages.h>
```

Zusätzlich benötigen wir noch eine NMEA-Objekt zur Behandlung der Stream-Daten von der Seriellen Schnittstelle:
```
// Global objects/variables
tNMEA0183 NMEA0183;         // NMEA stream for NMEA0183 receiving
```

In setup() initialisieren wir das Stream-Objekt mit:

```
NMEA0183.Begin(&Serial2, 3, 4800); // Start NMEA0183 stream handling with 4800 baud
```
Die bedeutet, dass wir für das Objekt die 2. serielle Schnittstelle mit der Baud-Rate 4800 angeben.

Die Geräte-Informationen für NMEA2000 setzen wir auf 135 und 25, passend für ein NMEA0183 zu NMEA2000 Gateway:
```
// Det device information
  NMEA2000.SetDeviceInformation(id,  // Unique number. Use e.g. Serial number.
                                135, // Device function=NMEA 0183 Gateway. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20%26%20function%20codes%20v%202.00.pdf
                                25,  // Device class=Inter/Intranetwork Device. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20%26%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                               );
```

Das war es schon mit der Vorbereitung. Die anderen Elemente für NMEA2000 hatten wir bereits in anderen Beispielen.

Die eigentliche Umwandlung von NMEA0183 zu NMEA2000 startet in der folgenden Funktion:

```
//*****************************************************************************
void NMEA0183_ParseMessages() {
  tNMEA0183Msg NMEA0183Msg;  // Create message container
  
  if (!NMEA0183.GetMessage(NMEA0183Msg)) return;  // New message ?

  // We do have a new message
  if (NMEA0183Msg.IsMessageCode("MWV")) HandleMWV(NMEA0183Msg);  // Check and handle message
}
```
Als erstes wir ein Nachrichtencontainer für eine NMEA0183-Nachricht erstellt. Dann prüfen wir, ob eine gültige NMEA0183-Nachricht von der seriellen Schnittstelle empfangen wurde.
Falls nicht, beenden wir die Funktion.

Falls eine Nachricht empfangen wurde, testen wir nun, um welche Nachricht es sich handelt. Für das Beispiel interessieren wir uns nur für die "MWV"-Nachricht mit Wind-Informationen. Wenn MWV empfangen wurde, rufen wie die entsprechende Behandlunsroutine auf. Nach diesem Verfahren kann man beliebege NMEA0183-Nachrichten behandeln.








