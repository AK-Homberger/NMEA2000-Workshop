# Web-Server mit AJAX

Als letztes wollen wir unser WLAN-Gateway noch mit einem Web-Server ausstatten.

Wir werden dazu HTML, CSS und Javascript nutzen. Eine gute Einführung zu allen drei Themen ist [hier](https://www.w3schools.com/html/default.asp) zu finden. Speziell auch zur [AJAX-Methode](https://www.w3schools.com/js/js_ajax_intro.asp), um einfach Daten zwischen Server und Client auszutauschen.

Laden wir nun das Programm [NMEA2000-WLAN-Gateway3.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA2000-WLAN-Gateway3/NMEA2000-WLAN-Gateway3.ino) auf den rechten ESP32.

Der NMEA-Simulator sollte auch gestartet sein. Wir geben etwas Gas (Throttle Gear etwas hochieben) und drehen am Ruder nach rechts (Rudder etwas nach rechts schieben). Nun ändern sich die Werte für SOG/COG im Simulator.

Dann verbinden wir uns wieder mit dem WLAN und starten dann den Web-Browser. Als Adresse geben wir "192.168.4.1" ein.
Nun sehen wir die Web-Seite des WLAN-Gateways.

![Web-Server](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/Web-Server.PNG)

Wie wir sehen, werden die Daten für COG und SOG im Browser angezeigt.

Mit dem Schieber und den Tasten AN/Aus können wir übrigens die eingebaute LED des ESP32 steueren (Ein/Aus und Dimmen).

## Das Programm

Schauen wir uns jetzt im Programm die Ergänzungen an. Wie wir sehen, gibt es ein neues Modul "index.h". In diesem Modul ist die Web-Seite (HTML, CSS) und der Javascript-Code gepeichert. Das sehen wir uns später an.

Kommen wir erst zum Hauptprogramm. Es gibt zei neie Include-Dateien.

```
#include <WebServer.h>
#include <ArduinoJson.h>
```
Die Erste wird für die Web-Server-Methoden benötigt und die Zeite für die Datenübergabe zwischen dem Web-Server auf dem ESP32 und dem Client auf dem Tablet.

Als nächstes definieren wir drei neue Variablen/Objekte:

```
WebServer web_server(80);   // Web server on TCP port 80
bool OnOff = false;         // LED On/Off
int Level = 0;              // LED dimm level
```

Wir erzeugen einen Web-Server auf Port 80. Die beiden nächsten Variablen sind für den Status der eingebauten LED des ESP32.

In setup() ergänzen wir, welche Funktionen für welche URLs aufgerufen werden sollen:

```
// Define web server events
  web_server.on("/", handleRoot);           // This is display page
  web_server.on("/get_data", getData);      // To get update of values
  web_server.on("/on", handleOn);           // On button
  web_server.on("/off", handleOff);         // Off button
  web_server.on("/slider", handleSlider);   // Slider evebt
  web_server.onNotFound(handleNotFound);
  
  web_server.begin();                       // Start web server
```
Mit web_server.begin() starten wir dann den Web-Server.

Dan konfigurieren wir den Dimmer für die LED;
```
  ledcAttachPin(1, 1);                      // PWM channel 1 on GPIO 1 (builtin LED)
  ledcSetup(1, 8000, 7);                    // Setup channel 1 with 8000 Hz
```
Mit ledcAttachPin(1, 1) verbinden wir den Kanal 1 des Pulsweitenmodulators mit GPIO1. Die eingebaute rote LED ist an GPIO1 angeschlossen (gilt für den ESP32 von AzDelivery). Und mit ledcSetup(1, 8000, 7) Setzen wir Kanal 1 auf 8000 Hz und 7 Bit Tastverhältnis (128).

Direkt hinter setup() definieren wir nun die einzelnen Behandlungroutinen:

Als erstes handleRoot():
```
void handleRoot() {
  web_server.send(200, "text/html", MAIN_page); //Send web page
}
```
Die Funktion wird immer dann aufgerufen, wenn der Web-Client dei Hauptseite des Web-Servers aufruft "/". Also zum Beispiel nur die Adresse des Web-Servers eingibt. Die Funktion liefert dann die Web-Seite mit dem HTML und Javascript-Code an den Client. Die Seite selbst und der Javascript-Code sind im Modul "index.h" ausgelagert. Dazu kommen wir später.


Wenn der Client die URL "/get_data" aufruft, wird folgende Funktion ausgeführt:
```
void getData() {
  String Text;
  char buf[20];
  
  StaticJsonDocument<200> root;
  
  snprintf(buf, sizeof(buf), "%4.1f", BoatData.SOG);
  root["sog"] = buf;

  snprintf(buf, sizeof(buf), "%4.1f", BoatData.COG);
  root["cog"] = buf;

  root["level"] = Level;
  
  if (OnOff) root["state"] = "An"; else root["state"] = "Aus";

  serializeJsonPretty(root, Text);
  web_server.send(200, "text/plain", Text); //Send values to client ajax request
}
```
Wir definieren eine String mit Namen Text, der die Antwort an den Client enthalten soll. Buf ist nur ein Puffer, um formatierte Daten zwischenzuspeichern.

Dann erzeugen wir ein JSON-Objekt mit dem Nanen "root", wobei wir als maximale Größe 200 angeben.

Dann setzen wir die Werte für SOG/COG nach dem selben Schema:
Erst füllen wir den Puffer buf mit dem F´formatierten Wert für SOG/COG (drei Vorkommastellen, eine Nachkommastelle).
Dann setzen wir die JSON-Werte für beide Werte;

Dann setzen wir noch JSON-Wete für den Dim-Level und den Status der LED (An/Aus).

Mit serializeJsonPretty(root, Text) füllen wir den String Text mit dem vollständigen JSON-Ausdruck und mit web_server.send() senden wir den Text an den Client.

Die nächsten drei Funktionen steuern die LED.

Die folgende Funktion wird ausgefürt, wenn im Browser die Taste "An" angeklickt wird.
```
void handleOn() {
  OnOff=true;
  ledcWrite(1, 128 - Level);
  web_server.send(200, "text/html");
}
```
LED-Status auf "An" (OnOff=true) und LED-Dimmer auf Level. 


Diese Funktion wird ausgefürt, wenn im Browser die Taste "Aus" angeklickt wird.
```
void handleOff() {
  OnOff=false;
  ledcWrite(1, 128);
  web_server.send(200, "text/html");
}
```
LED-Status auf "Aus" (OnOff=false) und LED-Dimmer auf 0. PWM-Tastverhältnis auf Maximum (128) bedeutet LED ist aus.


Die folgende Funktion wird immer dann ausgeführt, wenn im Browser der Schieber verändert wird.
```
void handleSlider() {
  
  if(web_server.args() > 0) {
    Level=web_server.arg(0).toFloat();
  }
  web_server.send(200, "text/html");

  if(OnOff) ledcWrite(1, 128 - Level);    
}
```
Mit "web_server.args() > 0" prüfen wir zuerst, ob auch ein Wert mit übergeben wurde. Falls ja, setzen wir den Wert für Level mit "web_server.arg(0).toFloat(), wobei wir auch gleich angeben, dass es sich um eine Kommazahl (float) handelt.

Dann setzen wir den DIM-Wert der LED auf den wert Level. Aber nur wenn der Status der LED auch "An" ist.

Die letzte Behandlungsfunktion definiert, was geschehen soll, wenn der Client eine nicht definierte URL aufruft. Den Code 404 hat bestimmt schon jeder einemal beim Browsen gesehen.

```
void handleNotFound() {                                           // Unknown request. Send error 404
  web_server.send(404, "text/plain", "File Not Found\n\n");
}
````

In loop() ergänzen wir nur eine Zeile:

web_server.handleClient();

Das ist alles im Hauptprogramm.






# Das war es schon
Das war es nun mit dem Workshop.

Kommen wir nun noch einmal zu den Zielen des Workshops zurück und sehen nach, ob wir alle Ziele erreicht haben.

Ihr solltet nun folgendes können:

- Aufbau eines NMEA2000-Netzwerks auf einem Steckbrett (ESP32, CAN-Bus-Transceiver)
- Die Arduino-IDE installieren
- Die nötigen Bibliotheken installieren (ZIP-Datei und Bibliotheksverwalter)
- Grundlegende Informationen zur NMEA2000-Bibliothek finden (PGNs, Datentypen)
- Arduinio-IDE nutzen (Programme laden und auf den ESP32 hochladen)
- Daten von einem NMEA2000-Bus auslesen und auf dem PC darstellen (mit NMEA-Reader)
- Den Aufbau eines typischen Programms (C/C++) verstehen
- I2C-Sensoren (hier BME280) nutzen (Anschluss I2C, Bibliotheken)
- Messen von Werten (Temperatur, Luftfeuchte, Druck) und Senden entsprechender PGNs
- Nutzung von 1-Wire und Multitasking mit ESP32 (Temperatursensor DS18B20)
- Messung von Spannungen und Widerständen (ESP32-ADC nutzen)
- Messung von Frequenzen (ESP32-Interrupts nutzen)
- Spezifische Daten mit dem ESP32 vom NMEA2000-Bus lesen (PGNs) und nutzen
- Aufbau eines NMEA2000-zu-NMEA0183-WLAN-Gateways und Darstellung von simulierten Daten (NMEA-Simulator) in OpenCPN und Tablet

Ich glaube, wir können alles abhaken!

Vielen Dank für die Teilnahme am Workshop.
Feedback wird gern entgegengenommen.
