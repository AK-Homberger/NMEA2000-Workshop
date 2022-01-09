# NMEA0183 nach NMEA2000 wandeln
Manchmal möcht man auch NMEA0183-Nachrichten zu NMEA2000 wandeln. Ach dies geht sehr einfach mit den Bibliotheken von Timo Lappalainen.
Als Beispiel werden wir Wind-Daten, die als MWV-Datensatz vorliegen, nach NMEA2000 wandeln. Prinzipiell kann man nach diesem Schama alle NMEA0183-Datensätze zu NMEA2000 umsetzen.

Im [Programm](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA0183WindToN2k/NMEA0183WindToN2k.ino) benötigen wir als erstes zwei zusätzliche Include-Dateien:

```
#include <NMEA0183.h>
#include <NMEA0183Messages.h>
```



