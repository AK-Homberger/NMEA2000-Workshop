# Senden von Daten an SignalK-Server

Jetst sehen wir noch, wie wir Daten zusätzlich zu NMEA2000 auch an einen SignalK-Server senden können.
Ziel ist es Temperatur, Luftfeuchtigkeit und Luftdruck an einen SignalK-Server im eignen WLAN zu senden.

SignaK hat eine immer stärkere Verbreitung, weil es die Integration unterschiedlicher Datenquellen vereinfacht und darüber hinaus auch gut zur Anzeige von Daten genutzt weden kann.

Grundlegende Informationen zu SignalK sind [hier](https://signalk.org/) zu finden.

Die Basis ist das Steckbrett aus den BME280-Beispielen.

![Steckbrett mit BME280](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/Bilder/NMEA2000-BME280_Steckplatine.png)

Wir laden jetzt das Programm [NMEA2000-BME280-3-Signalk.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA2000-BME280-3-SignalK/NMEA2000-BME280-3-Signalk.ino) in die Arduino-IDE.

![Arduino-IDE](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/BME280-Signalk.png)

Wie wir sehen, besteht das Programm aus drei Modulen. Dazu kommen wir aber erst später.

Zuvor müssen wir noch zwei paar zusätzliche Bibliotheken intallieren.

- ESP32SSDP (als [ZIP-File](https://github.com/luc-github/ESP32SSDP))
- WebSockets (fom Markus Sattler, mit dem Bibliotheksverwalter)

Die eigentliche ESP-SignalK-Bibliothek ist direkt im Programmverzeichnis mit abgelegt. Der Grund dafür ist, dass umfangreich Änderungen gegenüber der [Original-Bibliothek](https://github.com/mxtommy/EspSigK) notwendig waren, ums sie an den ESP32 und die aktuelle ArduinoJson-Bibliothek anzupassen. In der IDE sehen wir die Bibliothek als zwei zusätzliche Module (EspSigK.h und EspSigK.cpp).

Um später die SignalK-Daten auch empfangen und anzeigen zu können, müssen wir noch den SignalK-Server installieren. Wir nutzen hier den Server für Windows. Es sind aber auch Server für andere Plattformen verfügbar (z.B. Linux auf RaspberryPi). Im Prinzip ist es egal, auf welcher Plattform der Server läuft. Die Bedienung erfolgt hauptsächlich über ein Web-Interface.

Die Windows-Version kann wie [hier](https://github.com/SignalK/signalk-server-windows) beschrieben installiert werden.
Über diesen [Link](https://github.com/SignalK/signalk-server-windows/releases/latest/download/signalk-server-setup.exe) startet der Download. Nach Ausführen der Datei "signalk-server-setup.exe" wird der Server installiert. Die Standard-Vorschläge zur Installation sollten so belassen weden.

Im Programm müssen wir noch die Zugangsdaten zum eigenen WLAN eingeben. Ohne WLAN funktioniert es nicht.

```
const String ssid      = "ssid";      // SSID to connect to
const String ssidPass  = "password";  // Password for wifi
```

Danach speichern wir das Programm und laden es auf den rechten ESP32 hoch.

Dann starten wir auf dem PC den SignalK-Server, indem wir das Desktop-Icon "Start Signal K Service" ausführen. Das muss als Administrator erfolgen (rechte Maustaste und "als Administrator ausführen" anklicken).

Wenn der Server gestatet wurde, kann mit einen Doppelklick auf "Signal K Server" das Web-Interfach gestartet werden.

Nach der Auswahl von "Data Browser" auf der linken Seite sollte das Bild wie folgt aussehen:
![Server](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/SignalK-DataBrowser.png).

Falls keine Daten angezeigt werden, kann es an der Windows-Firewall liegen, die den Zugriff auf den Server verhindert.
Zum Testen kann die Firewall kurz abgeschaltet werden. Dazu rechter Mausklick auf das Netzwerk-Icon und dann "Netzwerk und Interneteinstellungen öffnen" anklicken. Dann "Windows Firewall" anklicken un die Firewall für das private Netzwer auschalten.

Spätestens jetzt sollten die Daten angezeigt werden. Nach dem Testen von SignalK sollte die Firewall auch wieder eingeschaltet werden.

Dan [mDNS](https://en.wikipedia.org/wiki/Multicast_DNS) findet der ESP32 selstständig den SignalK-Server und verbindet sich mit diesem.

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
Dansch wird das Objekt sgK erstellt und mit den Werten des WLANs initialisiert. Die EspSigK-Bibliothek kümmer sich auch um die WLAN-Verbindung.

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
gibt es Einstellmöglichkeiten für die Funktionsweise der Bibliothek. Die können im Wesentlichen aber auskommentiert beliben.

Mit sigK.begin() wird die SignalK-Behandlung gestartet.

In loop() 
```
sigK.handle();
```
Wurde der Funktionsaufruf sigK.handle() hinzugefügt.

Kommen wir num zum Senden der Informationen an den SignalK-Server. In den drei Funktionen zum Senden von Tempertature, Luftfeuchtigkeit und Luftdruck wurde jeweils noch eine Zeile hinzugefügt. Hier exemplarisch die Temperatur:

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
Wie wir sehen wurde nur die Zeile
```
sigK.sendDelta("environment.inside.temperature", CToKelvin(Temperature));    
```
hinzugefügt. Das ist alles was nötig ist, Daten an den SignalK-Server zu senden.

Für Luftfeuchtigkeit und Luftdruck sehen die Zeilen ähnlich aus:
```
sigK.sendDelta("environment.inside.relativeHumidity", Humidity);    
...
sigK.sendDelta("environment.inside.pressure", mBarToPascal(Pressure));    
```
Wie wir auch sehen, werden im "Data Browser"-Fenster auch genau die Pfadangabe angezeigt. Für die Temperatur zum Beispiel "environment.inside.temperature". 

Welche Pfadangeben zu nutzen dien, ist nicht willkürlich, sondern in der SignalK-Dokumentation festgelegt. Für Boote sind [diese](https://signalk.org/specification/1.5.0/doc/vesselsBranch.html) festlegungen zu nutzen.

Im Programm kann man "/vessels/<RegExp>/" in derRegel weg lassen, Und für "sigK.sendDelta" sind die "/"-Zeichen durch "." zu ersetzen. So, das war es auch schon. Nun könnt ihr alle Daten auch an einen SignalK-Server senden und anzeigen lassen.

Zur Anzeige gibt es auch eine schöne "WebApp" die man über den linken Menüeintrag "WebApps" und "@SignalK/Instrumentenpanel" erreicht. Hier kann man die Datenfelder auswählen und unter "Schraubenschlüsselsymbol", "Curtomise Displa", "Preferred Units" auch die beforugten Einheiten auswählen. Dann mach die Anzeige auch die Umrechnungen von SI-Einheiten selbstständig.

Ich habe mir folgende kleine Anzeige zusammengebaut. Versucht es doch auch so aussehen zu lassen.

![Instument](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/Signalk-Instruments.png).

