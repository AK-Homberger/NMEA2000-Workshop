# Funktion des WLAN-Gateway-Programms

![Arduino-IDE](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/Arduino-IDE-GW.png).

Wie bereits erwähnt besteht das Gateway-Programm aus mehreren Teilen. Die Teilprogramme sind alle im gleichen Verzeichnis gespeichert:

Die Dateinamen enstprechen den oben angezeigten Programmreitern.

- NMEA2000-WLAN-Gateway: Das ist unser Hauptprogramm (.ino wird nicht mit angezeigt).

Dann gibt es noch vier weitere Unterkomponenten:

- BoatData.h: In dieser Include-Datei werden Boots-Daten in eine Struktur zusammengefasst, um einfacher über Modulgrenzen hinweg darauf zugreifen zu können.
- List.h: Die Include-Date wird zur Verwaltung der per WLAN verbundenen TCP-Clients genutzt (Stichwort: verkettete Liste).
- N2kDataToNMEA0183.cpp: Ein C++-Modul, dass die eigentlichen Umwandlungen von NME2000 zu NMEA0183 eledeigt.
- N2kDataToNMEA0183.h: Die zum obige CPP-Modul gehörende Include-Datei mit Deklaratione/Definitionen.

Starten wir mit der Funktion des Hauptprogramms:

