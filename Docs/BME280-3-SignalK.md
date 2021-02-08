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

Um später die SignalK-Daten auch empfangen und anzeigen zu können, müssen wir noch den SignalK-Server installieren. Wir nutzen hier den Server für Windows. Es sinda aber auch Server für andere Plattformen verfügbar (Linux auf RaspberryPi). Im Prinzip ist es egal, auf welcher Plattform der Server läuft. Die Bedienung erfolgt hauptsächlich über ein Web-Interface.

Die Windows-Version kann wie [hier](https://github.com/SignalK/signalk-server-windows) beschrieben installiert werden.
Über diesen [Link](https://github.com/SignalK/signalk-server-windows/releases/latest/download/signalk-server-setup.exe) startet der download. Nach Ausführen der Datei "signalk-server-setup.exe" wird der Server installiert. Die Standard-Vorschläge zur Installation sollten so belassen weden.

Im Programm müsen wir noch die Uugangsdaten zum eigenen WLAN eingeben. Ohne WLAN funktioniert es nicht.

```
const String ssid      = "ssid";      // SSID to connect to
const String ssidPass  = "password";  // Password for wifi
```

Danach speichern wir das Programm und laden es auf den rechten ESP32 hoch.

Dann starten wir auf dem PC den SignalK-Server indem wir das Desktop-Icon "Start Signal K Service" ausführen. Das muss als Administrator erfolgen (rechte Maustaste und 
"als Administrator ausführen" anklicken).

Wenn der Server gestatet wurde kann mit einen Doppelklick auf "Signal K Server" das Web-Interfach gestartet werden.

![Server](Signal K Server).



