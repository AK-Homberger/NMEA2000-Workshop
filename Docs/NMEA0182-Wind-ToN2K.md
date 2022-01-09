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
Die bedeutet, dass wir für das Ojekt die 2. serielle Schnittstell mit der Baud-Rate 4800 angeben.
Das war es schon mit der Vorbereitung. Die anderen Elemente für NMEA2000 hatten wir bereits in anderen Beispielen.


