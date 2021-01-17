# Web-Server mit AJAX

Als letztes wollen wir unser WLAN-Gateway noch mit einem Web-Server ausstatten.

Wir werden dazu HTML, CSS und Javascript nutzen. Eine gute Einführung zu allen drei Themen ist [hier](https://www.w3schools.com/html/default.asp) zu finden. Speziell auch zur [AJAX-Methode](https://www.w3schools.com/js/js_ajax_intro.asp), um einfach Daten zwischen Server und Client auszutauschen.

Laden wir nun das Programm [NMEA2000-WLAN-Gateway3.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA2000-WLAN-Gateway3/NMEA2000-WLAN-Gateway3.ino) auf den rechten ESP32.

Der NMEA-Simulator sollte auch gestartet sein. Wir geben etwas Gas (Throttle Gear etwas hochschieben) und drehen am Ruder nach rechts (Rudder etwas nach rechts schieben). Nun ändern sich die Werte für SOG/COG im Simulator.

Dann verbinden wir uns wieder mit dem WLAN und starten dann den Web-Browser. Als Adresse geben wir "192.168.4.1" ein.
Nun sehen wir die Web-Seite des WLAN-Gateways.

![Web-Server](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/Web-Server.PNG)

Wie wir sehen, werden die Daten für COG und SOG im Browser angezeigt.

Mit dem Schieber und den Tasten AN/Aus können wir übrigens die eingebaute LED des ESP32 steueren (Ein/Aus und Dimmen).

## Das Programm

Schauen wir uns jetzt im Programm die Ergänzungen an. Wie wir sehen, gibt es ein neues Modul "index.h". In diesem Modul ist die Web-Seite (HTML, CSS) und der Javascript-Code gepeichert. Das sehen wir uns später an.

Kommen wir erst zum Hauptprogramm. Es gibt zwei neue Include-Dateien.

```
#include <WebServer.h>
#include <ArduinoJson.h>
```
Die Erste wird für die Web-Server-Methoden benötigt, und die Zweite für die Datenübergabe zwischen dem Web-Server auf dem ESP32 und dem Client auf dem Tablet.

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
Die Funktion wird immer dann aufgerufen, wenn der Web-Client dei Hauptseite des Web-Servers aufruft "/". Also zum Beispiel nur die Adresse des Web-Servers eingibt. Die Funktion liefert dann die Web-Seite mit dem HTML- und Javascript-Code an den Client. Die Seite selbst und der Javascript-Code sind im Modul "index.h" ausgelagert. Dazu kommen wir später.

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
Wir definieren einen String mit Namen Text, der die Antwort an den Client enthalten soll. "Buf" ist nur ein Puffer, um formatierte Daten zwischen zu speichern.

Dann erzeugen wir ein [JSON-Objekt](https://de.wikipedia.org/wiki/JavaScript_Object_Notation) mit dem Namen "root", wobei wir als maximale Größe 200 angeben.

Dann setzen wir die Werte für SOG/COG nach dem selben Schema:

- Erst füllen wir den Puffer buf mit dem formatierten Wert für SOG/COG (drei Vorkommastellen, eine Nachkommastelle).
- Dann setzen wir die JSON-Objekte für beide Werte.

Dann setzen wir noch JSON-Objekte für den Dim-Level und den Status der LED (An/Aus).

Mit serializeJsonPretty(root, Text) füllen wir den String Text mit dem vollständigen JSON-Ausdruck, und mit web_server.send() senden wir den Text an den Client.

Die nächsten drei Funktionen steuern die LED.

Die folgende Funktion wird ausgeführt, wenn im Browser die Taste "An" angeklickt wird.
```
void handleOn() {
  OnOff=true;
  ledcWrite(1, 128 - Level);
  web_server.send(200, "text/html");
}
```
LED-Status auf "An" (OnOff=true) und LED-Dimmer auf Level. 

Diese Funktion wird ausgeführt, wenn im Browser die Taste "Aus" angeklickt wird.
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

Dann setzen wir den DIM-Wert der LED auf den Wert Level. Aber nur, wenn der Status der LED auch "An" ist.

Die letzte Behandlungsfunktion definiert, was geschehen soll, wenn der Client eine nicht definierte URL aufruft. Den Code 404 hat bestimmt schon jeder einmal beim Browsen gesehen.

```
void handleNotFound() {                                           // Unknown request. Send error 404
  web_server.send(404, "text/plain", "File Not Found\n\n");
}
````

In loop() ergänzen wir nur eine Zeile:

web_server.handleClient();

Das ist schon alles im Hauptprogramm.

## Modul index.h

Kommen wir nun zum Inhalt des Moduls "index.h". 

Als erstes wird mit "const char MAIN_page[] PROGMEM = R"=====(" die Variable "Main_page" erzeugt. Die kryptischen Zeichen sorgen dafür, dass sie im Programmspeicher des ESP32 gespeichert werden soll (PROGMEM) und dass sie auch Sonderzeichen enthalten kann (R"=====).

Dann wird die Web-Seite als HTML-Code erstellt. Im Bereich "head" werden Metadaten und CSS-Stile "=style" definiert. Die "style"-Daten bstimmen, wie Elemente im Browser dargestellt werden (Farben, Größe, Abstände usw.). Details kann man [hier](https://www.w3schools.com/css/default.asp) nachlesen.

Im Bereich "body" erstellen wir dann die Anzeigeelemente der Seite:

```
<body style="font-family: verdana,sans-serif" BGCOLOR="#819FF7">

  <table>
    <tr><td style="text-align:right;">COG:</td><td style="color:white;"><span id='cog'></span> °</td></tr>
    <tr><td style="text-align:right;">SOG:</td><td style="color:white;"><span id='sog'></span> kn</td></tr>
    <tr><td style="text-align:right;">LED:</td><td style="color:white;"><span id='state'></span></td></tr>
    <tr><td style="text-align:right;">Dim:</td><td style="color:white;"><span id='level'></span> %</td></tr>
  </table>

  <br>
  
  <div class="slidecontainer">
    <p><input type="range" min="0" max="100" value="50" class="slider" id="myRange"></p>
  </div>

  <hr>
  <p>
  <input type="button" class="button" value="  An  " onclick="button_clicked('on')"> 
  <input type="button" class="button" value="Aus" onclick="button_clicked('off')"> 
  </p>
  </hr>
    
  ```
  
Wie HTML im Detail funktioniert, kann man [hier](https://www.w3schools.com/html/default.asp) nachlesen.
  
Die Elemente mit den IDs: cog, sog, state und level werden in einer Tabelle definiert. Mit den IDs können wir später die Werte für die Elemente setzen.

Dann erzeugen wir einen Schieber mit der ID "myRange" und den Min/Max-Werten 0/100.

Als letzte HTML-Elemente kommen noch die beiden Tasten "An" und "Aus", wobei auch gleich festgelegt wird, was geschehen soll, wenn eine Taste angeklickt wird. Es wird dann die Javascript-Funktion button_clicked() aufgerufen, wobei der Wert der Taste "on/off" mit übergeben wird.

Mit <script> wird angezeigt, dass nun der eigentliche Javascript-Code folgt.
  
```
<script>
  
  requestData(); // get intial data straight away 

  var slider = document.getElementById("myRange");
  var output = document.getElementById("level");
```
Als erstes holen wir gleich einmal aktuelle Werte vom Server.

Dann definieren wir zwei Variablen, um später einfacher auf die HTML-Objekte "myRange" und "level" zugreifen zu können.

Dann definieren wir die Funktion, um auf Änderungen das Schiebers zu reagieren:
```
slider.oninput = function() {
      output.innerHTML = this.value;
      var xhr = new XMLHttpRequest();
      xhr.open('GET', 'slider' + '?level=' + this.value, true);
      xhr.send();
    }
```
Die Funktion wird immer dann aufgrufen, wenn der Schieber verändert wird, wobe der Wert des Schiber als Variable "this.value" übergben wird. Als erstes setzen wir die Anzeige des DIM-Levels mit "output.innerHTML = this.value".

Dann erzeugen wir mit "var xhr = new XMLHttpRequest();" ein neues Request-Objekt.

Der Funktion "xhr.open('GET', 'slider' + '?level=' + this.value, true)" ist wichtig, um zu verstehen, wie der Wert für Level in der URL übergeben wird. Das passierte mit "'slider' + '?level=' + this.value". das erzeugt eine Request URL mit folgenem exemplarischen Inhalt "/slider/?level=wert". Im Hauptprogamm wir der so übergebene Wert in der Funktion handleSlider() als Argument erkannt und mit "Level=web_server.arg(0).toFloat();" gesetzt.

Als letztes wird der Request gesendet.

Weitere Argumente werden übrigens nach dem Schema "&name=wert" hinzugefügt. Auf das zweite Argument wir auf Server-Seite dann mit "web_server.arg(1)" zugegriffen.

Mit JSON zur Datenübergabe vom Server zum Client und der URL-Kodierung für Client zu Server haben wir nun alles was wir benötigen, um Werte auszutauschen.


Hier definieren wir die Funktion, um auf die Tasten (An/Aus" zu reagieren:
```
function button_clicked(key) { 
      var xhr = new XMLHttpRequest();
      xhr.open('GET', key, true);
      xhr.send();
      requestData();
    }
  
// request data updates every 500 milliseconds
setInterval(requestData, 500);
```
Diese Funktion wird immer dann aufgerufen, wenn ein Button angeklickt wurde.  Wobei je nach Taste für die Variable key "on" oder "off" mit übergeben wurde. 

Je nach key wird mit xhr.open('GET', key, true) entweder die URL "/on" oder "/off" erzeugt. Im Hauptprogramm wird daher, je nach URL, handleOn() oder handleOff() ausgeführt.

Mit setInterval(requestData, 500) legen wir nun fest, dass die Funktion requestData() alle 500 ms aufgerufen wird. Dadurch werden die Werte im Browser zwei Mal pro Sekunde aktualisiert. 

Als letztes folgt die Definition der Funktion requestData() selbst.

```
    function requestData() {

      var xhr = new XMLHttpRequest();
      
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {

          if (this.responseText) { // if the returned data is not null, update the values

            var data = JSON.parse(this.responseText);

            document.getElementById("cog").innerText = data.cog;
            document.getElementById("sog").innerText = data.sog;
            document.getElementById("state").innerText = data.state;
            
            output.innerHTML = data.level;
            slider.value = data.level;
          } 
        } 
      }
      xhr.open('GET', 'get_data', true);
      xhr.send();
    }
```

Mit var xhr = new XMLHttpRequest() wird ein neuer Request erzeugt, und mit "xhr.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) {" wird auch gleich festgelegt, was passieren soll, wenn der Request erfolgreich beendet wurde. 

Der Wert "200" wurde übrigens zusammen mit dem JSON-Ausdruck "Text" von der Funktion getData() im Hauptprogramm gesendet "web_server.send(200, "text/plain", Text)".

Mit "var data = JSON.parse(this.responseText);" holen wir uns auch den JSON-Ausdruck als Variable "data".
Auf die einzelnen Datenelemente greifen wir einfach mit "data.Elementname" zu.

Für die jeweiligen HTML-Elemente setzen wir nun einfach die übergebenen Werte. Immer nach dem gleichen Schema: 

"document.getElementById("name").innerText = data.name;"

Für "output" und "slider" hatten wir ja extra Variablen erzeugt. Mit diesen Variablen setzen wir nun den Wert für Level. Einmal als Zahl (output.innerHTML = data.level;) und dann auch als Position des Schiebers (slider.value = data.level)

Mit "xhr.open('GET', 'get_data', true)" definieren wir die URL (/get_data). Und mit xhr.send() senden wir den Request.

Das war alles zum Thema Web-Server und AJAX. Es ist recht einfach, die Funktionen auf Server- und auf Cleint-Seite nun mit weiteren Funktionen zu erweitern.

[Das wars...](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Ende.md).
