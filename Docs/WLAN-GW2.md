# Funktion des WLAN-Gateway-Programms

![Arduino-IDE](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/Arduino-IDE-GW.png).

Wie bereits erwähnt, besteht das Gateway-Programm aus mehreren Teilen. Die Teilprogramme sind alle im gleichen Verzeichnis gespeichert.

Die Dateinamen enstprechen den oben angezeigten Programmreitern.

- NMEA2000-WLAN-Gateway: Das ist unser Hauptprogramm (.ino wird nicht mit angezeigt).

Dann gibt es noch vier weitere Unterkomponenten:

- BoatData.h: In dieser Include-Datei werden Bootsdaten in eine Struktur zusammengefasst, um einfacher über Modulgrenzen hinweg darauf zugreifen zu können.
- List.h: Die Include-Datei wird zur Verwaltung der per WLAN verbundenen TCP-Clients genutzt (Stichwort: verkettete Liste).
- N2kDataToNMEA0183.cpp: Ein C++ Modul, dass die eigentlichen Umwandlungen von NMEA2000 zu NMEA0183 eledigt.
- N2kDataToNMEA0183.h: Die zum obigen C++ Modul gehörende Include-Datei, mit Deklaratione/Definitionen.

# Hauptprogramm

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
Die oberen vier sind generelle Include-Dateien für den ESP32 selbst (WiFi.h und Preferences.h) und die NMEA-Bibliothek (Seasmart.h und N2kMessages.h). Seasmart.h ist notwedig für die optionale Ausgabe der NMEA2000-Daten im Seasmart-Format. Preferences.h kennen wir ja schon aus den früheren Beispielen und WiFi.h benötigen wir für den WLAN-Access-Point.

Die nächsten drei Include-Dateien sind für unsere lokalen Programm-Module, wie oben beschrieben.

## Definitionen

Im nächsten Schritt werden Definitonen festgelegt:

```
// Buffer config
#define MAX_NMEA0183_MESSAGE_SIZE 150
#define MAX_NMEA2000_MESSAGE_SEASMART_SIZE 500
```
Diese Definitonen sind für die Verwaltung der Pufferspeicher notwendig.


## Globale Variable/Objekte

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

Als erstes wird der TCP-Port für den Server definiert (2222). NodeAddress und Pereferences kennen wir schon aus früheren Beispielen.

Mit "const size_t MaxClients = 10;" legen wir die maximale Anzahl der gleichzeitigen Clients fest. Falls ihr mehr als zehn gleichzeitige Clients im WLAN erwartet, müsst ihr hier den Wert erhöhen.

Mit "sendNMEA0183Conversion = true" und "SendSeaSmart = false" legen wir fest, welche Datenformate gesendet werden sollen.
Im Moment senden wir nur die von NMEA2000 nach NMEA0183 gewandelten Daten. Auf Seasmart kommen wir später noch zurück.

```
WiFiServer server(ServerPort, MaxClients);

using tWiFiClientPtr = std::shared_ptr<WiFiClient>;
LinkedList<tWiFiClientPtr> clients;

tN2kDataToNMEA0183 tN2kDataToNMEA0183(&NMEA2000, 0);  // NMEA 0183 conversion handler
```
Jetzt definieren wir einen WiFiServer (=TCP-Server) mit dem Port ServerPort (=2222) und MaxCliens (=10).
Die nächsten beiden Zeilen benötigen wir zur Verwaltung der Clinets als verkettete Liste. 

Dann wird die Behandlungsfunktion für die Wandlung von NMEA2000 auf NMEA0183 definiert.

Die folgenden Zeilen sind uns aus vorigen Beispielen schon bekannt:

```
const unsigned long ReceiveMessages[] PROGMEM = {/*126992L,*/ // System time
      127250L, // Heading
      127258L, // Magnetic variation
      128259L, // Boat speed
      128267L, // Depth
      129025L, // Position
      129026L, // COG and SOG
      129029L, // GNSS
      130306L, // Wind
      128275L, // Log
      127245L, // Rudder
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

## Funktionen

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

Mit diesen Zeilen erstellen wir einen WLAN-Access-Point mit dem Namen "NMEA2000-Gateway" und dem Passwort "password".
Dann starten wir den zuvor definierten TCP-Server.

Wir haben auch die Produkt- und Gerärteinformationen etwas agepasst:

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
Wie nutzen Klasse "25" und Gerät "130".

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

Danach legen wir zwei Nachrichten-Behandlungsroutinen fest. Einmal zur Umwandlung von NMEA2000 auf NMEA0183 mit  NMEA2000.AttachMsgHandler(&tN2kDataToNMEA0183) und dann für die optionale Wandlung ins Seasmart-Format mit NMEA2000.SetMsgHandler(HandleNMEA2000Msg). 

Das letzte Kommando kennen wir ja schon aus den Beipielen zum Lesen vom NMEA2000-Bus. NMEA2000.AttachMsgHandler() wird benötigt, um eine zusätzliche Behandlungsroutine hinzuzufügen. Wenn man nur eine Funktion benötigt, reicht NMEA2000.SetMsgHandler().

Als letzes setzen wir hier noch die Funktion, die aufgerufen werden soll, wenn eine NMEA0183-Nachricht gesendet werden soll.

Die beiden Funktionen HandleNMEA2000Msg() und SendNMEA0183Message() sind übrigens direkt im Hauptprogamm definiert. Die Umwandlung von NMEA2000 zu NMEA0183 erfolgt im Modul "N2kDataToNMEA0183.cpp". Doch dazu später.

Die nächsten vier Funktionen:
- AddClient()
- StopClient()
- CheckConnections()
- SendBufToClients()

dienen der Verwaltung der TCP-Verbindungen der Clients. Wir gehen hier nicht weiter darauf ein. Gehen wir einfach davon aus, dass es funktioniert.

Es folgt die Definition von HandleNMEA2000Msg():

```
void HandleNMEA2000Msg(const tN2kMsg &N2kMsg) {

  if ( !SendSeaSmart ) return;

  char buf[MAX_NMEA2000_MESSAGE_SEASMART_SIZE];
  if ( N2kToSeasmart(N2kMsg, millis(), buf, MAX_NMEA2000_MESSAGE_SEASMART_SIZE) == 0 ) return;
  SendBufToClients(buf);
}
```

Diese Funktion wird für jede empfangene NMEA2000-Nachricht aufgerufen. Falls die Variable SendSeaSmart auf "True" gesetzt war, erfolgt die Ausgabe der NMEA2000-Daten direkt im Sesmart-Format an die WLAN-Clients. Ansonsten wird die Funktion beendet.

Danach folgt die Definition von SendNMEA0183Message():

```
void SendNMEA0183Message(const tNMEA0183Msg &NMEA0183Msg) {
  if ( !SendNMEA0183Conversion ) return;

  char buf[MAX_NMEA0183_MESSAGE_SIZE];
  if ( !NMEA0183Msg.GetMessage(buf, MAX_NMEA0183_MESSAGE_SIZE) ) return;
  SendBufToClients(buf);  // Send to WLAN-Clients
  Serial.println(buf);    // Send to USB-Serial
}
```
Auch hier wird als erstes geprüft, ob die Wandlung von NMEA2000 auf NMEA0183 gewünscht ist (SendNMEA0183Conversion = True).

Falls ja, wird die aktuelle NMEA0183-Nachricht in den Puffer "buf" kopiert und an die TCP-Clients im WLAN gesendet. Zusätzlich wird der Inhalt des Puffers auch auf die serielle Schnittstelle ausgegeben. Zum Beispiel für die Anzeige in OpenCPN.

Es bleibt noch die Funktion loop().

```
void loop() {
  CheckConnections();
  
  NMEA2000.ParseMessages();
  
  CheckSourceAddressChange();
  
  tN2kDataToNMEA0183.Update(&BoatData);

  // Dummy to empty input buffer to avoid board to stuck with e.g. NMEA Reader
  if ( Serial.available() ) {
    Serial.read();
  }
}
```

- CheckConnections() prüft regelmässig ob es neue TCP-Clients gibt oder ob Clients, die Verbindung beendet hatten.
- NMEA2000.ParseMessages() und CheckSourceAddressChange() sind ja schon bekannt.
- tN2kDataToNMEA0183.Update(&BoatData) ruft die Update-Funktion in Sub-Modul auf, wobei die Referenz zur Struktur "BoatData" übergeben wird. Nach dem Funktionsaufruf enthält die Strukur alle aktualisierten Daten aus dem Sub-Modul.

# Modul BoatData.h

Dieses Modul wird genutzt, um eine Struktur mit Werten zu deklarieren. Die Struktur dient dazu, unteschiedliche Wete zusammenzufassen, und die Werte über Modulgrenzen hinweg zu nutzen.

```
struct tBoatData {
    unsigned long DaysSince1970;   // Days since 1970-01-01

    double Heading, SOG, COG, STW, Variation, AWS, TWS, MaxAws, MaxTws, AWA, TWA, AWD, TWD, TripLog, Log, RudderPosition, WaterTemperature,
           WaterDepth, GPSTime,// Secs since midnight,
           Latitude, Longitude, Altitude;

  public:
    tBoatData() {
      Heading = 0;
      Latitude = 0;
      Longitude = 0;
      SOG = 0;
      COG = 0;
      STW = 0;
      AWS = 0;
      TWS = 0;
      MaxAws = 0;
      MaxTws = 0;
      AWA = 0;
      TWA = 0;
      TWD = 0;
      TripLog = 0;
      Log = 0;
      RudderPosition = 0;
      WaterTemperature = 0;
      WaterDepth = 0;
      Variation = 0;
      Altitude = 0;
      GPSTime = 0;
      DaysSince1970 = 0;
    };
};
```

Auf den Inhalt der Dateien List.h und N2kDataToNMEA0183.h gehen wir nicht im Detail ein. Das würde dem Umfang des Workshops sprengen und erfordert C++ Kenntnisse, die wir hier nicht voraussetzen können.

# Modul N2kDataToNMEA0183.cpp
Aber den Aufbau von N2kDataToNMEA0183.cpp sehen wir uns zumindest generell an.

Fangen wir mit HandleMsg an:

```
//*****************************************************************************
void tN2kDataToNMEA0183::HandleMsg(const tN2kMsg &N2kMsg) {
  switch (N2kMsg.PGN) {
    case 127250UL: HandleHeading(N2kMsg);
    case 127258UL: HandleVariation(N2kMsg);
    case 128259UL: HandleBoatSpeed(N2kMsg);
    case 128267UL: HandleDepth(N2kMsg);
    case 129025UL: HandlePosition(N2kMsg);
    case 129026UL: HandleCOGSOG(N2kMsg);
    case 129029UL: HandleGNSS(N2kMsg);
    case 130306UL: HandleWind(N2kMsg);
    case 128275UL: HandleLog(N2kMsg);
    case 127245UL: HandleRudder(N2kMsg);
    case 130310UL: HandleWaterTemp(N2kMsg);
  }
}
```
Im Wesentlichen ist das vegleichbar mit der HandleMessage-Funktion, die wir auch im Beispiel zum Lesen vom NMEA2000-Bus verwendet hatten. Es ist genau genommen, die Funktion, die wir oben im Hauptprogramm mit NMEA2000.AttachMsgHandler(&tN2kDataToNMEA0183) festgelegt hatten. 

Die Funktionsweise ist hier die gleiche wie beim Lesen-Beispiel. Die Funktion wird für jede NMEA2000-Nachricht aufgerufen. Mit "case" prüfen wie die PGN-Nummern und rufen Funktionen zur weiteren Behandlung auf. Hatten wir alles schon.

Schauen wir uns doch exemplarisch einmal zwei Funktionen an:

Erstens HandleHeading():

```
void tN2kDataToNMEA0183::HandleHeading(const tN2kMsg &N2kMsg) {
  unsigned char SID;
  tN2kHeadingReference ref;
  double _Deviation = 0;
  double _Variation;
  tNMEA0183Msg NMEA0183Msg;

  if ( ParseN2kHeading(N2kMsg, SID, Heading, _Deviation, _Variation, ref) ) {
    if ( ref == N2khr_magnetic ) {
      if ( !N2kIsNA(_Variation) ) Variation = _Variation; // Update Variation
      if ( !N2kIsNA(Heading) && !N2kIsNA(Variation) ) Heading -= Variation;
    }
    LastHeadingTime = millis();
    if ( NMEA0183SetHDG(NMEA0183Msg, Heading, _Deviation, Variation) ) {
      SendMessage(NMEA0183Msg);
    }
  }
}
```
Wie wir sehen, ist der Aufbau sehr ähnlich wie beim Lesen vom NMEA2000-Bus.

Im Unterschied zu vorherigen Lesen-Beispiel wird hier geprüft, ob neue Daten vorliegen. In diesem Fall werden globale Variable aktualisiert. 

Mit LastHeadingTime = millis() wird sich der letzte korrekte Empfang der Heading-Information gemerkt (nutzen wir hier nicht).

Dann erfolgt der Zusammenbau der NMEA0183-Nachricht: NMEA0183SetHDG(NMEA0183Msg, Heading, _Deviation, Variation).

Mit SendMessage(NMEA0183Msg) wird die Nachricht dann gesendet. 

Das wirkliche Senden erfolgt im Hauptprogramm mit der Funktion SendNMEA0183Message().
Mit tN2kDataToNMEA0183.SetSendNMEA0183MessageCallback(SendNMEA0183Message) wurde dort diese Funktion ja zum Senden definiert.

Die anderen Behandlungs-Funktionen folgen dem selben Schema.

Sollte euch eine Konversations-Routine von MNEA2000 nach NMEA0183 fehlen, könnt ihr sie nach dem bekannten Ablauf aus dem Daten-Lesen-Beispiel ergänzen.

Die NMEA0183-Bibliothek von Timo Lappalainen ist recht rudimentär. Es sind nicht alle relevanten NMEA0183-Nachrichten enthalten.

Das macht aber in der Praxis nicht viel aus. Durch die in der Bibliothek definierten Hilfsfunktionen ist das Zusammenbauen und Senden von NMEA0183-Nachrichten sehr einfach.

Wir schauen uns hierzu einmal die Funktion HandleLog() an:

```
void tN2kDataToNMEA0183::HandleLog(const tN2kMsg & N2kMsg) {
  uint16_t DaysSince1970;
  double SecondsSinceMidnight;

    if ( ParseN2kDistanceLog(N2kMsg, DaysSince1970, SecondsSinceMidnight, Log, TripLog) ) {

      tNMEA0183Msg NMEA0183Msg;

      if ( !NMEA0183Msg.Init("VLW", "GP") ) return;
      if ( !NMEA0183Msg.AddDoubleField(Log / 1852.0) ) return;
      if ( !NMEA0183Msg.AddStrField("N") ) return;
      if ( !NMEA0183Msg.AddDoubleField(TripLog / 1852.0) ) return;
      if ( !NMEA0183Msg.AddStrField("N") ) return;

      SendMessage(NMEA0183Msg);
    }
  }
```
In der Funktion parsen wir mit ParseN2kDistanceLog() die NMEA2000-Nachricht mit PGN128275, wobei wir die nowendigen Variablen mit übergeben. Log und TripLog sind übrigens im Modul global definiert.
  
Wir erzeugen nun mit "tNMEA0183Msg NMEA0183Msg" einen Nachrichten-Container für eine NMEA0183-Nachricht.
  
Dann verwenden wir die Variablen Log und TripLog und bauen uns einfach die NMEA0183-Nachricht "VLW" passgenau zusammen. Auch die Umwandlung vom m/s auf kn führen wir hier durch.

Dann wird die Nachricht mit SendMessage(NMEA0183Msg) gesendet.

Die unterschiedlichen Funktionen zum Zusammenbauen von NMEA0183-Nachrichten sind [hier](https://github.com/ttlappalainen/NMEA0183/blob/04a9235182b12ead309fb349cca534cd211e1334/NMEA0183Msg.cpp#L151) definiert.

Die anderen Funktionen im Modul sind recht ähnlich aufgebaut. Einer eigenstängigen Erweiterung steht nichts mehr im Wege.
Eine Liste mit NMEA0183-Nachrichtentypen ist [hier](http://www.nmea.de/nmea0183datensaetze.html) zu finden.

Im nächsten [Teil](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/WLAN-GW3.md) erweitern wir das Gateway noch um eine Multiplexing-Funktion.
