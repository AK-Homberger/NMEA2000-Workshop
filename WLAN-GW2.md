# Funktion des WLAN-Gateway-Programms

![Arduino-IDE](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/Arduino-IDE-GW.png).

Wie bereits erwähnt, besteht das Gateway-Programm aus mehreren Teilen. Die Teilprogramme sind alle im gleichen Verzeichnis gespeichert.

Die Dateinamen enstprechen den oben angezeigten Programmreitern.

- NMEA2000-WLAN-Gateway: Das ist unser Hauptprogramm (.ino wird nicht mit angezeigt).

Dann gibt es noch vier weitere Unterkomponenten:

- BoatData.h: In dieser Include-Datei werden Boots-Daten in eine Struktur zusammengefasst, um einfacher über Modulgrenzen hinweg darauf zugreifen zu können.
- List.h: Die Include-Datei wird zur Verwaltung der per WLAN verbundenen TCP-Clients genutzt (Stichwort: verkettete Liste).
- N2kDataToNMEA0183.cpp: Ein C++-Modul, dass die eigentlichen Umwandlungen von NME2000 zu NMEA0183 eledigt.
- N2kDataToNMEA0183.h: Die zum obigen CPP-Modul gehörende Include-Datei, mit Deklaratione/Definitionen.

Starten wir mit der Funktion des Hauptprogramms:


## Zusätzliche Include-Dateien

Als erstes sehen wir, dass wir zusätzliche Include-Dateien verwenden:

```
#include <Seasmart.h>
#include <N2kMessages.h>
#include <WiFi.h>
#include <Preferences.h>

#include "N2kDataToNMEA0183.h"
#include "List.h"
#include "BoatData.h"
```
Die oberen vier sind generelle Include-Dateien für den ESP32 selbst (WiFi.h und Preferences.h) und die NMEA-Bibliothek (Seasmart.h und N2kMessages.h). Seasmart.h ist nur nowedig für die optinale Ausgabe der NMEA2000-Daten im Seasmart-Format. Da es sodut wie keine Client-Programme gibt, die diese Format verarbeite können, gehen wir darauf im Moment nicht weiter ein. Preferences.h kennen wir ja schon aus den früheren Beispielen und WiFi.h benötigen wir für den WLAN-Access-Point.

Die nächsten drei Include-Dateien sind für unsere lokalen Programm-Module, wie oben beschrieben.




