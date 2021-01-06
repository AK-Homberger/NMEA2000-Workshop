# NMEA 2000 Workshop

Dieses Repository enthält Informationen zum NMEA-2000-Workshop im Januar 2021.

Im Rahmen des Workshops werden wir erste Programme mit dem ESP32 und der NMEA2000-Bibliothek von Timo Lappalainen erstellen.

# Vorbereitungen:
- Download und Installation der Arduino-Entwicklungsumgebung [Link](https://www.arduino.cc/en/software)
- Installation der ESP32-Board Informationen [Link](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/esp32-jetzt-mit-boardverwalter-installieren)

Download der NMEA-2000-Bibliotheken [Link](https://github.com/ttlappalainen)

Folgene Bibliotheken als ZIP-Dateien downloaden und installieren:
- NMEA2000
- NMEA2000_esp32
- NMEA0183

Jeweils auf den Namen des Repositories klicken, dann den auf "Code" klicken und "Download ZIP" auswählen. Dann in der Aroino IDE: Sketch, Bibliothek einbinden, .ZIP-Bibliothek hinzufügen auswählen und den Speicherort der heruntergeladenen Bibliotheken für jede der drei Bibliotheken nacheinander auswählen und installieren.

Die Basis für den Workshop bilden zwei ESP32-Module mit CAN-Bus-Transceivern. Der erste ESP32 dient als Schnittstelle zum NMEA-Simulator/-Reader auf dem PC/Laptop. Der zweite wird für die Beispielprogramme genutzt.

An beiden ES32 wird der CAN-Transceiver angeschlossen (3,3 Volt, HND, CAN-RX, CAN-TX). Die beiden Transceiver werden verbunden (CAN-H mit CAN-H und CAN-L mit CAN-L).
Damit haben wir ein kleines NMEA 2000 Netzwerk zum üben.

So sieht das dann auf dem Steckbrett aus:

![Basis](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/Bilder/NMEA2000-Basis_Steckplatine.png)

- Auf dem linken ESP32 ist der Sketch "ActisenseListenerSender-ESP32" zu installieren [Link](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/ActisenseListenerSender-ESP32/ActisenseListenerSender-ESP32.ino). Dazu nur den linken ESP32 mit USB verbinden und in der Arduiono IDE den Seriellen USB-Port auswählen und notieren.
- Auf dem PC muss zum Senden von Daten der NMEA2000-Simulator installiert werden [Link](http://www.kave.fi/Apps/index.html)
- Zum Lesen und Anzeigen von NMEA2000-Daten kann der NMEA Reader verwendet werden: [Link](https://actisense.com/acti_software/nmea-reader/)


# Workshop Programme
Auf dem rechten ESP32 werden unterschiedliche Programme installiert, um die Nutzung der NMEA2000-Bibliothek zu erklären. Dazu wird der ESP32 mit USB verbunden und in der Arduini IDE der neue Serielle USB-Port ausgewählt.
Mit dem NMEA-Reader können wir Daten vom NMEA2000-Netzwerk anzeigen, und mit dem NMEA-Simulator auch Daten an das Netzwerk senden. Beides werden wir im Workshop nutzen.

Folgende Beispielprogramme werden im Workshop gemeinsam erstellt und mit dem Reader/Simulator getestet:

1. NMEA2000-Thermometer, Barometer mit BME280 (I2C)

2. Temperaturfühler mit DS18B20 (1-Wire)

3. Spannungs- und Widerstandsmessung (Bordspannung, Tankanzeige)

4. NMEA2000-WLAN-Gateway und Anzeige der Daten auf OpenCPN und Tablet/Telefon


# Material für den Workshop

Folgende Materialien werden für den Workshop benötigt:

- Steckbrett (Spannungsversorgung Mitte): [Link](https://www.reichelt.de/experimentier-steckboard-1260-300-kontakte-steckboard-sb-3-p139508.html?&trstct=pol_5&nbc=1)
- 2* ESP32 [Link](https://www.amazon.de/AZDelivery-NodeMCU-Development-Nachfolgermodell-ESP8266/dp/B071P98VTG/ref=sxts_sxwds-bia-wc-drs3_0?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&cv_ct_cx=ESP32&dchild=1&keywords=ESP32)
- 2* CAN-Bus Transceiver [Link](https://eckstein-shop.de/Waveshare-SN65HVD230-CAN-Board-33V-ESD-protection)
- BME280 Sensor [Link](https://www.reichelt.de/entwicklerboards-temperatur-feuchtigkeits-und-drucksensor--debo-bme280-p253982.html?&trstct=pos_1&nbc=1)
- DS18B20 Temperaturfühler [Link](https://www.reichelt.de/digital-thermometer-1-wire--0-5-c-to-92-ds-18b20-p58169.html?search=18b20)
- Drehpotentiometer 10 K Ohm [Link](https://www.reichelt.de/drehpotentiometer-10-kohm-logarithmisch-6-mm-pih-pc16ip06103b-p232589.html?&trstct=pol_9&nbc=1)
- Flexible Drahtbrücken [Link](https://www.reichelt.de/flexible-drahtbruecken-15-cm-mehrfarbig-65er-pack-rnd-255-00009-p253202.html?&nbc=1&trstct=lsbght_sldr::253206)

2* USB-Kabel (Micro, passend für ESP32)

1 PC/Laptopt (mit 2 * USB)

1* Tablet/Telefon (Android/Apple)

Der bei Reichelt angebotene ESP32 (Joy-IT) geht prinzipiell auch. Er hat aber eine andere Pin-Belegung als der von AZ-Delivery. Im Workshop nutzen wir den ESP32 von AZ-Delivery.
