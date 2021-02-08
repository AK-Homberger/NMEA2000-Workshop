# Senden von Daten an einen SignalK-Server

Jetzt sehen wir noch, wie wir Daten zusätzlich zu NMEA2000 auch an einen SignalK-Server senden können.
Ziel ist es Temperatur, Luftfeuchtigkeit und Luftdruck an einen SignalK-Server im eigenen WLAN zu senden.

SignalK hat eine immer stärkere Verbreitung, weil es die Integration unterschiedlicher Datenquellen vereinfacht und darüber hinaus auch gut zur Anzeige von Daten genutzt werden kann.

Grundlegende Informationen zu SignalK sind [hier](https://signalk.org/) zu finden.

Die Basis ist das Steckbrett aus den BME280-Beispielen.

![Steckbrett mit BME280](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/Bilder/NMEA2000-BME280_Steckplatine.png)

Wir laden jetzt das Programm [NMEA2000-BME280-3-Signalk.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA2000-BME280-3-SignalK/NMEA2000-BME280-3-Signalk.ino) in die Arduino-IDE.

![Arduino-IDE](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/BME280-Signalk.png)

Wie wir sehen, besteht das Programm aus drei Modulen. Dazu kommen wir aber erst später.

Zuvor müssen wir noch zwei zusätzliche Bibliotheken installieren.

- ESP32SSDP (als [ZIP-File](https://github.com/luc-github/ESP32SSDP))
- WebSockets (von Markus Sattler, Installation mit dem Bibliotheksverwalter)

Die eigentliche ESP-SignalK-Bibliothek ist direkt im Programmverzeichnis mit abgelegt. Der Grund dafür ist, dass umfangreiche Änderungen gegenüber der [Original-Bibliothek](https://github.com/mxtommy/EspSigK) notwendig waren, um sie an den ESP32 und die aktuelle ArduinoJson-Bibliothek anzupassen. In der IDE sehen wir die Bibliothek als zwei zusätzliche Module (EspSigK.h und EspSigK.cpp).

Um später die vom ESP32 gesendeten SignalK-Daten auch empfangen und anzeigen zu können, müssen wir noch einen SignalK-Server installieren. Wir nutzen hier den Server für Windows. Es sind aber auch Server für andere Plattformen verfügbar (z.B. Linux auf Raspberry Pi). Im Prinzip ist es egal, auf welcher Plattform der Server läuft. Die Bedienung erfolgt hauptsächlich über ein Web-Interface.

Die Windows-Version kann wie [hier](https://github.com/SignalK/signalk-server-windows) beschrieben installiert werden.

Über diesen [Link](https://github.com/SignalK/signalk-server-windows/releases/latest/download/signalk-server-setup.exe) startet der Download. Nach Ausführen der Datei "signalk-server-setup.exe" wird der Server installiert. Die Standard-Vorschläge zur Installation sollten so belassen werden. Der Installationsvorgang dauert etwas. Bitte legt vorerst keinen Admin-Nutzer an, sonst funktioniert das Beispiel so nicht.

Im Modul"NMEA2000-BME280-3-Signalk.ino" müssen wir noch die Zugangsdaten zum eigenen WLAN eingeben. Ohne WLAN funktioniert es nicht.

```
const String ssid      = "ssid";      // SSID to connect to
const String ssidPass  = "password";  // Password for wifi
```

Danach speichern wir das Programm und laden es auf den rechten ESP32 hoch.

Dann starten wir auf dem PC den SignalK-Server, indem wir das Desktop-Icon "Start Signal K Service" ausführen. Das muss als Administrator erfolgen (rechte Maustaste auf das Icon und "als Administrator ausführen" anklicken).

Wenn der Server gestartet wurde, kann mit einen Doppelklick auf das Desktop-Icon "Signal K Server" das Web-Interface gestartet werden.

Nach der Auswahl von "Data Browser" auf der linken Seite sollte das Bild wie folgt aussehen:

![Server](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/SignalK-DataBrowser.png).

Falls keine Daten angezeigt werden, kann es an der Windows-Firewall liegen, die den Zugriff auf den Server verhindert.

Zum Testen kann die Firewall kurz abgeschaltet werden. Dazu rechter Mausklick auf das Netzwerk-Icon unten rechts im Fenster und dann "Netzwerk und Interneteinstellungen öffnen" anklicken. Dann "Windows Firewall" anklicken und die Firewall für das private Netzwerk ausschalten.

Spätestens jetzt sollten die Daten angezeigt werden. Nach dem Testen von SignalK sollte die Firewall auch wieder eingeschaltet werden.

Dank [mDNS](https://en.wikipedia.org/wiki/Multicast_DNS) findet der ESP32 selbstständig den SignalK-Server und verbindet sich mit diesem.

Kommen wir nun zum Programm. Es sind nur wenige Änderungen gegenüber dem letzten BME280-Beispiel hinzugekommen.

Es gibt eine neue Include-Datei:
```
#include "EspSigK.h"        // For SignalK handling
````
Neue globale Definitionen:
```
const String hostname  = "NMEA2000-Gateway";    //Hostname for network discovery
const String ssid      = "ssid";     //SSID to connect to
const String ssidPass  = "password";  // Password for wifi

EspSigK sigK(hostname, ssid, ssidPass); // create the object
````
Hier wird der Netzwerk-Name des Gateways festgelegt und die WLAN-Zugangsdaten gesetzt.

Danach wird das Objekt sigK erstellt und mit den Werten des WLANs initialisiert. Die EspSigK-Bibliothek kümmert sich auch um die WLAN-Verbindung.

In setup()

```
 // SignalK settings
  sigK.setPrintDebugSerial(false);       // Default false, causes debug messages to be printed to Serial (connecting etc)
  sigK.setPrintDeltaSerial(false);       // Default false, prints deltas to Serial.
  //sigK.setServerHost("192.168.0.20");  // Optional. Sets the ip of the SignalKServer to connect to. If not set we try to discover server with mDNS
  //sigK.setServerPort(3000);            // If manually setting host, this sets the port for the signalK Server (default 80);

  //sigK.setServerToken("secret"); // if you have security enabled in node server, it wont accept deltas unles you auth
  // add a user via the admin console, and then run the "signalk-generate-token" script
  // included with signalk to generate the string. (or disable security)

  sigK.begin();                          // Start everything. Connect to wifi, setup services, etc...
```
gibt es Einstellmöglichkeiten für die Funktionsweise der Bibliothek. Die können im Wesentlichen aber auskommentiert bleiben.

Mit sigK.begin() wird die SignalK-Behandlung gestartet.

In loop() 
```
sigK.handle();
```
wurde der Funktionsaufruf sigK.handle() hinzugefügt.

Kommen wir nun zum Senden der Informationen an den SignalK-Server. In den drei Funktionen zum Senden von Temperatatur, Luftfeuchtigkeit und Luftdruck wurde jeweils noch eine Zeile hinzugefügt. Hier exemplarisch für die Temperatur:

```
void SendN2kTemperature(void) {
  static unsigned long SlowDataUpdated = InitNextUpdate(SlowDataUpdatePeriod, TempSendOffset);
  tN2kMsg N2kMsg;
  double Temperature;

  if ( IsTimeToUpdate(SlowDataUpdated) ) {
    SetNextUpdate(SlowDataUpdated, SlowDataUpdatePeriod);

    Temperature = bme.readTemperature();    
    Serial.printf("Temperature: %3.1f °C \n", Temperature);
    sigK.sendDelta("environment.inside.temperature", CToKelvin(Temperature));    

    // Definition from N2kMessages.h
    // void SetN2kPGN130312(tN2kMsg &N2kMsg, unsigned char SID, unsigned char TempInstance, tN2kTempSource TempSource,
    //        double ActualTemperature, double SetTemperature=N2kDoubleNA);

    // tN2kTempSource is defined in N2kTypes.h

    // Set N2K message
    SetN2kPGN130312(N2kMsg, 0, 0, N2kts_MainCabinTemperature, CToKelvin(Temperature), N2kDoubleNA);
    
    // Send message
    NMEA2000.SendMsg(N2kMsg);
  }
}
```
Wie wir sehen, wurde nur die Zeile
```
sigK.sendDelta("environment.inside.temperature", CToKelvin(Temperature));    
```
hinzugefügt. Das ist alles was nötig ist, um Daten an den SignalK-Server zu senden.

Für Luftfeuchtigkeit und Luftdruck sehen die Zeilen ähnlich aus:
```
sigK.sendDelta("environment.inside.relativeHumidity", Humidity);    
...
sigK.sendDelta("environment.inside.pressure", mBarToPascal(Pressure));    
```
Wie wir auch sehen, werden im "Data Browser"-Fenster auch genau die Pfadangaben angezeigt. Für die Temperatur zum Beispiel "environment.inside.temperature". 

Welche Pfadangaben zu nutzen sind, ist nicht willkürlich, sondern in der SignalK-Dokumentation festgelegt. Für Boote sind [diese](https://signalk.org/specification/1.5.0/doc/vesselsBranch.html) Festlegungen zu nutzen.

Im eigenen Programm kann man "/vessels/RegExp/" in der Regel weg lassen, und für "sigK.sendDelta" sind die "/"-Zeichen durch "." zu ersetzen. 
 
So, das war es auch schon. Nun könnt ihr alle möglichen Daten auch an einen SignalK-Server senden und anzeigen lassen.

Zur Anzeige gibt es auch eine schöne "WebApp" die man über den linken Menüeintrag "WebApps" und "@SignalK/Instrumentenpanel" erreicht. Hier kann man die Datenfelder auswählen und unter "Schraubenschlüsselsymbol", "Customise Display", "Preferred Units" auch die bevorzugten Einheiten auswählen. Dann macht die Anzeige auch die Umrechnungen von SI-Einheiten selbstständig.

Ich habe mir folgende kleine Anzeige zusammengebaut. Versucht es doch auch, eure Anzeigen so aussehen zu lassen.

![Instument](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/Signalk-Instruments.png).


## NMEA2000 per USB

Eine Möglichkeit, die Daten vom NMEA2000-Bus an den SignalK-Server zu senden, ist die Verwendung des [ActisenseListenerSender-ESP32.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/ActisenseListenerSender-ESP32/ActisenseListenerSender-ESP32.ino). Diesen können wir nun auch auf den rechten ESP32 laden. 

Im SignalK-Web-Interface legen wir danach eine neue Datenquelle an:

![Quelle](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/SignalK-Actisense.png)

ID könnt ihr frei wählen. Als Quelle wählt ihr "Actisense NGT-1 (canboatjs)" aus. Als COM-Port wählt ihr den Com-Port des **rechten** ESP32 aus. Baud-Rate ist wie immer 115200.

Jetzt haben wir alle NMEA2000-Daten im SignalK-Server.

Mit dem NMEA-Simulator können wir nun Daten über den linken ESP32 an den NMEA2000-Bus senden. Der rechte ESP32 liest die Daten vom Bus und gibt sie im Actisense-Format über USB an den SignalK-Server. Dieser kann sie dann verwenden und auch anzeigen.


![Anzeige](https://github.com/AK-Homberger/NMEA2000-SignalK-Gateway/blob/main/Signalk-Instrumentpanel.png)

[Das war es nun schon ...](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/Ende.md).
