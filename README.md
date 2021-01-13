# NMEA2000-Workshop

Dieses Repository enthält Informationen zum NMEA-2000-Workshop im Januar/Februar 2021.
Das Material ist eigentlich für die Nutzung in einem Präsenz- oder Online-Workshop gedacht.

Die Informationen können aber auch ohne Workshop in eigener Regie genutzt werden. Alle notwendigen Informationen und Erklärungen sind im Text enthalten.

Nach dem Workshop solltet ihr folgendes können:
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

## NMEA2000-Historie

NMEA2000 ist eine standardisierte Erweiterung des älteren Protokolls [NMEA0183](https://de.wikipedia.org/wiki/NMEA_0183). Grundlegende Informationen zu NMEA2000 sind [hier](https://de.wikipedia.org/wiki/NMEA_2000) zu finden.

Im Rahmen des Workshops werden wir ein kleines NMEA2000-Netzwerk aufbauen und erste Programme mit der NMEA2000-Bibliothek von Timo Lappalainen erstellen und ausprobieren.

## Vorbereiten der Arduino-IDE
- Download und Installation der Arduino-Entwicklungsumgebung [Link](https://www.arduino.cc/en/software)
- Installation der ESP32-Board Informationen [Link](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/esp32-jetzt-mit-boardverwalter-installieren)

In der Arduino-IDE müssen wir als nächstes das richtige Board auswählen. Dazu klicken wir auf "Werkzeuge", "Board" und wählen dann "ESP32 Arduino" und "ESP32 Dev Module".

Download der NMEA2000-Bibliotheken [Link](https://github.com/ttlappalainen)

Folgene Bibliotheken als ZIP-Dateien downloaden und installieren:
- NMEA2000
- NMEA2000_esp32
- NMEA0183

Jeweils auf den Namen des Repositories klicken, dann den auf "Code" klicken und "Download ZIP" auswählen. Dann in der Arduino-IDE: Sketch, Bibliothek einbinden, .ZIP-Bibliothek hinzufügen auswählen und den Speicherort der heruntergeladenen Bibliotheken für jede der drei Bibliotheken nacheinander auswählen und installieren.

Für die spätere Nutzung der Bibliotheken sind speziell drei Dateien wichtig:
1. NMEA2000-Library-Referenz [Link](https://github.com/ttlappalainen/NMEA2000/blob/master/Documents/NMEA2000_library_reference.pdf)
2. Header-Datei "N2kMessages.h" mit den PGN-Definitionen [Link](https://github.com/ttlappalainen/NMEA2000/blob/master/src/N2kMessages.h)
3. Header-Datei "N2kTypes.h" mit Definitonen für Datentypen in den PGNs [Link](https://github.com/ttlappalainen/NMEA2000/blob/master/src/N2kTypes.h)

## Vorbereiten des Steckbretts
Die Basis für den Workshop bilden zwei ESP32-Module mit CAN-Bus-Transceivern. Der erste ESP32 dient als Schnittstelle zum NMEA-Simulator/-Reader auf dem PC/Laptop. Der zweite wird für die Beispielprogramme genutzt.

An beiden ESP32 wird der CAN-Transceiver angeschlossen (3,3 Volt, GND, CAN-RX, CAN-TX). Die beiden Transceiver werden verbunden (CAN-H mit CAN-H und CAN-L mit CAN-L).
Damit haben wir ein kleines NMEA2000-Netzwerk zum Üben. 

Eigenlich besteht ein NMEA2000-Netzwerk aus einem Backbone-Kabel, das an beiden Enden mit einem Widerstand abgeschlossen ist. Vom Backbone können dann mehrere Stichleitungen zu den Geräten abgehen. Für unser Mini-Netzwerk ist das aber nicht weiter wichtig. Der CAN-Bus ist so robust, dass es auch mit einer einfachen Direktverbindung von CAN-L und CAN-H funktioniert.

So sieht das dann auf dem Steckbrett aus:

![Basis](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/Bilder/NMEA2000-Basis_Steckplatine.png)

## Zusätzliche Programme zum Testen von Funktionen (PC und Apple/Android APP)
- Zum Lesen und Anzeigen von NMEA2000-Daten auf dem PC soll der NMEA-Reader verwendet werden: [Link](https://actisense.com/acti_software/nmea-reader/)
- Auf dem PC muss zum Senden von Daten der NMEA2000-Simulator installiert werden: [Link](http://www.kave.fi/Apps/index.html).
- Optional kann auf dem PC auch OpenCPN installiert werden (wird zum Testen des WLAN-Gateways genutzt): [Link](https://opencpn.org/OpenCPN/info/downloads.html)
- Auf dem Telefon/Tablet benötigen wir eine APP zum Anzeigen von NMEA0183-Daten. Zum Beispiel [NMEAremote(lite)](https://apps.apple.com/de/app/nmearemote/id412806204) für Apple-Geräte. Die "lite"-Version ist kostenlos. Für Android-Geräte bitte im App-Store nach einer geeigneten APP suchen und installieren (z.B. AvNav oder nke Display). Ich konnte das leider nicht testen.

## Vorbereiten des linken ESP32
Der einfachste Weg, die Beispielprogramme zu nutzen, ist das ganze NMEA2000-Workshop-Repository als ZIP-Datei herunterzuladen. Dazu klickt man oben auf "NMEA2000-Workshop" und dann auf den grünen Button "Code". Dann "Download ZIP" auswählen. Die heruntergeladene ZIP-Datei in einen frei wählbaren Ordner entpacken. Dann kann man die Beispielprogramme direkt in der Arduino-IDE öffnen. Dazu einfach auf die jeweilige .ino-Datei doppelklicken.

Auf dem linken ESP32 ist der Sketch "ActisenseListenerSender-ESP32.ino" zu installieren [Link](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/ActisenseListenerSender-ESP32/ActisenseListenerSender-ESP32.ino). Dazu im entpackten ZIP-Ordner die Datei "ActisenseListenerSender-ESP32.ino" doppelklicken. Die Arduino-IDE startet dann mit geladenem Sketch. Dann nur den linken ESP32 mit USB verbinden und in der Arduiono-IDE den seriellen USB-Port auswählen und "Sketch" "Hochladen" anwählen. Den seriellen Port bitte notieren. 

## Workshop-Beispielprogramme
Auf dem rechten ESP32 werden unterschiedliche Programme installiert, um die Nutzung der NMEA2000-Bibliothek zu erklären. Dazu wird der ESP32 mit USB verbunden und in der Arduino-IDE der neue serielle USB-Port ausgewählt.

Mit dem NMEA-Reader können wir Daten vom NMEA2000-Netzwerk anzeigen, und mit dem NMEA-Simulator auch Daten an das Netzwerk senden. Beides werden wir im Workshop nutzen.

Folgende Beispielprogramme werden im Workshop gemeinsam erstellt und mit dem Reader/Simulator getestet:

1. [NMEA2000-Thermometer, Hygrometer und Barometer mit BME280 (I2C)](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/BME280.md)
2. [Temperaturfühler mit DS18B20 (1-Wire)](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/DS18B20.md)
3. [Spannungs- und Widerstandsmessung (Batteriespannung, Tankanzeige)](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/AnalogRead.md)
4. [Messung von Frequenzen (ESP32-Interrupts nutzen)](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Frequenz.md)
5. [Daten (PGNs) vom NMEA-2000-Bus lesen und darstellen](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/ReadPGNs.md)
5. [NMEA2000-WLAN-Gateway und Anzeige der Daten auf OpenCPN und Tablet/Telefon](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/WLAN-GW.md)


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
