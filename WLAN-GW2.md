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

Im nächten Schritt weden Definitonen festgelegt:

```
// Buffer config
#define MAX_NMEA0183_MESSAGE_SIZE 150
#define MAX_NMEA2000_MESSAGE_SEASMART_SIZE 500
```
Diese Definitonen sind für die Verwaltung der Pufferspeicher notwendig.

Dann kommen globale Variablen/Objekte:

```
const uint16_t ServerPort = 2222; // Define the port, where server sends data. 

// Struct to update BoatData. See BoatData.h for content
tBoatData BoatData;

int NodeAddress;                    // To store last Node Address
Preferences preferences;            // Nonvolatile storage on ESP32 - To store LastDeviceAddress

const size_t MaxClients = 10;       // Maximum number of concurrent clients
bool SendNMEA0183Conversion = true; // Do we send NMEA2000 -> NMEA0183 conversion
bool SendSeaSmart = false;          // Do we send NMEA2000 messages in SeaSmart format

WiFiServer server(ServerPort, MaxClients);

using tWiFiClientPtr = std::shared_ptr<WiFiClient>;
LinkedList<tWiFiClientPtr> clients;

tN2kDataToNMEA0183 tN2kDataToNMEA0183(&NMEA2000, 0);  // NMEA 0183 conversion handler
```

Als erstes wird der TCP-Port für den server definiert. NodeAdress und Pereference kenn wir schon aus früheren Beispielen.
Mit "const size_t MaxClients = 10;" legen wir die maximale Anzahl der gleichzeitigen Clinets fest. Falls ihr mehr als zehn gleichzeitige Clients im WLAN erwartet, müsst ihr hier den Wert erhöhen.











