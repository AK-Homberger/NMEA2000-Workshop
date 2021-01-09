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


```
WiFiServer server(ServerPort, MaxClients);

using tWiFiClientPtr = std::shared_ptr<WiFiClient>;
LinkedList<tWiFiClientPtr> clients;

tN2kDataToNMEA0183 tN2kDataToNMEA0183(&NMEA2000, 0);  // NMEA 0183 conversion handler
```
Jetzt definieren wir einen WiFiServer (=TCP-Server) mit dem Port ServerPort (=2222) und MaxClien (=10).
Die nächsten beiden Zeilen benötigen wir zur Verwalung der Clinets als verkettete Liste. 
Dann wird die Behandlings-Funktion für die Wandlung von NMEA2000 auf NMEA0183 definiert.

Die folgenden Zeilen sind uns aus vorigen Beispielen schon bekannt:

```
const unsigned long ReceiveMessages[] PROGMEM = {/*126992L,*/ // System time
      127250L, // Heading
      127258L, // Magnetic variation
      128259UL,// Boat speed
      128267UL,// Depth
      129025UL,// Position
      129026L, // COG and SOG
      129029L, // GNSS
      130306L, // Wind
      128275UL,// Log
      127245UL,// Rudder
      0
    };
```
Die folgenden beiden Zeilen sind Forwärts-Deklarationen.
    
```
// Forward declarations
void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);
void SendNMEA0183Message(const tNMEA0183Msg &NMEA0183Msg);
```
Diese sind notwendig, damit wir die Funktionsbezeichnungen in setup() nutzen können, obwohl die Funktionen erst später definiert werden.

Kommen wir nun zu setup():

```
 // Init WiFi connection
  Serial.println("Start WLAN AP");         // WiFi Mode AP
  WiFi.softAP("NMEA2000-Gateway", "password");
  WiFi.setHostname("NMEA2000-Gateway");
  IPAddress IP = WiFi.softAPIP();
  Serial.println("");
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Start TCP server
  server.begin();
```

Mit diesen Zeilen ertellen wir eine WLAN-Hotspot mit dem Namen "NMEA2000-Gateway" und dem Passort "password".
Dann starten wir den zuvor definierten TCP-Server.

Wir haben auch die Produkt- und Geräreinformationen etwas agepasst:

```
// Set product information
  NMEA2000.SetProductInformation("1", // Manufacturer's Model serial code
                                 100, // Manufacturer's product code
                                 "NMEA 2000 WiFi Gateway",  // Manufacturer's Model ID
                                 "1.0.2.25 (2019-07-07)",  // Manufacturer's Software version code
                                 "1.0.2.0 (2019-07-07)" // Manufacturer's Model version
                                );
  // Set device information
  NMEA2000.SetDeviceInformation(id, // Unique number. Use e.g. Serial number. Id is generated from MAC-Address
                                130, // Device function=Analog to NMEA 2000 Gateway. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                25, // Device class=Inter/Intranetwork Device. See codes on  http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                               );

```
Klasse 25 und Gerät 130.

Dann legen wir weitere Funktionsweisen fest:

```
// If you also want to see all traffic on the bus use N2km_ListenAndNode instead of N2km_NodeOnly below
  NMEA2000.SetMode(tNMEA2000::N2km_ListenAndNode, NodeAddress);

  NMEA2000.ExtendReceiveMessages(ReceiveMessages);
  NMEA2000.AttachMsgHandler(&tN2kDataToNMEA0183); // NMEA 2000 -> NMEA 0183 conversion
  NMEA2000.SetMsgHandler(HandleNMEA2000Msg);      // Also send all NMEA2000 messages in SeaSmart format

  tN2kDataToNMEA0183.SetSendNMEA0183MessageCallback(SendNMEA0183Message);
```
Diesmal setzen wir den Modus als "N2km_ListenAndNode".
Dann geben wir die Liste der zu empfangenen Nachrichten fest.

Danach legen wir zwei Nachrichten-Behandlungsroutinen fest. Einmal zur Umwandlung von NMEA2000 auf NMEA0183 (NMEA2000.AttachMsgHandler(&tN2kDataToNMEA0183) ) und dann für die optionale Wandlung ins Seasmart-Format (NMEA2000.SetMsgHandler(HandleNMEA2000Msg)). Das letzte Kommando kennen wir ja schon aus den Beipielen zum Lesen vom NMEA2000-Bus. NMEA2000.AttachMsgHandler() wird benötigt, um eine zusätzliche Behandlungsroutine hinzuzufügen. Wenn man nur eine benötigt, reicht NMEA2000.SetMsgHandler().













  






    





