# NMEA-Workshop

Dieses Repository enthält Informationen zum NMEA-Workshop im Janaur 2021.

Im Rahmen des Workshops werden wir erste Programme mit dem ESP32 und der NMEA2000 Bibliothek von Tomo Lappalainen erstellen.

Vorbereitungen:
- Download und Installation der Arduino-Entwicklungsumgebeung [Link](https://www.arduino.cc/en/software)
- Installation der ESP32-Board Informationen [Link](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/esp32-jetzt-mit-boardverwalter-installieren)

Download der NMEA-2000-Bibliotheken von Timo Lappalainen [Link](https://github.com/ttlappalainen)

Folgene Bibliotheken alz ZIP-Dateien Downloaden und installieren
- NMEA2000
- NMEA2000_esp32
- NMEA0183

Jeweils auf den Namen des Repositories klicken, dann den auf "Code" klicken und "Download ZIP" auswählen. Dann in der Aroino IDE: Sketch, Bibliothek einbinden, .ZIP-Bibliothek hinzufügen auswählen und den Speicherort der heruntergeladenen Bibliotheken auswählen.

Die Basis für den Workshop bilden zwei ESP32-Module mit CAN-BusTransceivern. Der erste ESP32 dient als Schnittstelle zum NMEA-Simulator auf dem PC/Laptop.
An beiden ES32 wird der CAN-Transceiver angeschlossen (3,3 Volt, HND, CAN-RX, CAN-TX). Die beiden Transceiver werden verbunden (CAN-H mit CAN-H und CAN-L mit CAN-L).
Damit haben wir ein kleines NMEA 2000 Netzwerk zum üben.

- Auf dem ESP32 ist der Sketch "ActisenseListenerSender-ESP32" zu installieren [Link](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/ActisenseListenerSender-ESP32/ActisenseListenerSender-ESP32.ino)
- Auf dem PC muss der NMEA 2000 Simulator installiert werden [Link](http://www.kave.fi/Apps/index.html)

Auf dem zweiten ESP32 werden unterschiedliche Programme installiert, um die Nutzung der NMEA2000 Bibliothek zu erklären.
Mit dem NMEA-2000-Simulator können wird Daten vom NMEA 2000 Netzwerk anzeigen und auch Daten an das Netzwerk senden. Beides werden wir im Workshop nutzen.

1. Programm NMEA 2000 Barometer mit BME280


2. Temparaturfühler mit DS18B20


3. NMEA 2000 WLAN Gateway


# Material für den Workshop

Folgende Materialien werden für den Workshop benötigt:

- Steckbrett (Spannungsversorgung Mitte): [Link](https://www.reichelt.de/experimentier-steckboard-1260-300-kontakte-steckboard-sb-3-p139508.html?&trstct=pol_5&nbc=1)
- 2* ESP32 [Link](https://www.reichelt.de/nodemcu-esp32-wifi-und-bluetooth-modul-debo-jt-esp32-p219897.html?&trstct=pos_0&nbc=1) oder:[Link](
https://www.amazon.de/AZDelivery-NodeMCU-Development-Nachfolgermodell-ESP8266/dp/B071P98VTG/ref=sxts_sxwds-bia-wc-drs3_0?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&cv_ct_cx=ESP32&dchild=1&keywords=ESP32)
- 2* CAN-Bus Transceiver [Link](https://eckstein-shop.de/Waveshare-SN65HVD230-CAN-Board-33V-ESD-protection)
- BME280 Sensor [Link](https://www.reichelt.de/entwicklerboards-temperatur-feuchtigkeits-und-drucksensor--debo-bme280-p253982.html?&trstct=pos_1&nbc=1)
- DS18B20 Temperaturfühler [Link](https://www.reichelt.de/digital-thermometer-1-wire--0-5-c-to-92-ds-18b20-p58169.html?search=18b20)
- Drehpotentiometer 10 K Ohm [Link](https://www.reichelt.de/drehpotentiometer-10-kohm-logarithmisch-6-mm-pih-pc16ip06103b-p232589.html?&trstct=pol_9&nbc=1)
- Flexible Drahtbrücken [Link](https://www.reichelt.de/flexible-drahtbruecken-15-cm-mehrfarbig-65er-pack-rnd-255-00009-p253202.html?&nbc=1&trstct=lsbght_sldr::253206)

2* USB-Kabel (Micro, passend für ESP32)

1 PC/Laptopt (mit 2 * USB)

1* Tablet/Telefon (Android/Apple)

