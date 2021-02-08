# Senden von Daten an SignalK-Server

Jetst sehen wir noch, wie wir Daten zusätzlich zu NMEA2000 auch an einen SignalK-Server senden können.
Ziel ist es Temperatur, Luftfeuchtigkeit und Luftdruck an einen SignalK-Server im eignen WLAN zu senden.

SignaK hat eine immer stärkere Verbreitung, weil es die ntegration unterschiedlicher Datenquellen vereinfacht und darüber hinaus auch gut zur Anzeige von Daten genutzt weden kann.

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




