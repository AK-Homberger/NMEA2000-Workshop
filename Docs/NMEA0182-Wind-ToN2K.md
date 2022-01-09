# NMEA0183 nach NMEA2000 wandeln
Manchmal möcht man auch NMEA0183-Nachrichten zu NMEA2000 wandeln. Ach dies geht sehr einfach mit den Bibliotheken von Timo Lappalainen.
Als Beispiel werden wir Wind-Daten, die als MWV-Datensatz vorliegen, nach NMEA2000 wandeln. Prinzipiell kann man nach diesem Schama alle NMEA0183-Datensätze zu NMEA2000 umsetzen.

Im Programm benötigen wir als erstes zwei zusätzliche Includ-Dateinen:

```
#include <NMEA0183.h>
#include <NMEA0183Messages.h>
```


