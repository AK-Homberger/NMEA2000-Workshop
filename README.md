# NMEA2000-Workshop

Dieses Repository enthält Informationen zum NMEA2000-Workshop in 2021.

Die Informationen sind eigentlich für die Nutzung in einem Präsenz- oder Online-Workshop gedacht. Sie können aber auch ohne Workshop in eigener Regie genutzt werden. Alle notwendigen Informationen und Erklärungen sind im Text enthalten.

Es haben inzwischen zwei Online-Workshops stattgefunden:

- Am 30.01.21 als Teil der [Yachttechnikseminare](https://www.segeln-forum.de/board69-gewerbliche-angebote/board114-aus-weiterbildung-gewerblich/77672-seminar-workshop-nmea-2000/).
- Am 24.02.21 (Teil 1) und 10.03.21 (Teil 2) im Rahmen von [Open Boat Projects live](https://www.segeln-forum.de/board194-boot-technik/board195-open-boat-projects-org/78918-open-boat-projects-live-3/?s=c8766c7bea94061aa3d0a8ebb954e6c4d6c9e1ec).

Die Zweiteilung des Workshops hat sich dabei bewährt, da es für einen Termin zu viel Inhalt ist.

Weitere Workshops können bei Bedarf folgen.

Bei Interesse bitte im Segeln-Forum [hier](https://www.segeln-forum.de/board194-boot-technik/board195-open-boat-projects-org/78918-open-boat-projects-live-3/?s=c8766c7bea94061aa3d0a8ebb954e6c4d6c9e1ec) melden. 

Sobald genügend Interessenten dabei sind, planen wir dann den nächsten Workshop.

Für Fragen und Diskussionen zum Workshop könnt ihr gern auch das [Segeln-Forum](https://www.segeln-forum.de/board194-boot-technik/board195-open-boat-projects-org/78918-open-boat-projects-live-3/?s=c8766c7bea94061aa3d0a8ebb954e6c4d6c9e1ec) nutzen.

Für den Workshop sind einige Bauteile notwendig. Die Liste mit genauen Bestelldaten folgt weiter [unten](https://github.com/AK-Homberger/NMEA2000-Workshop#material-f%C3%BCr-den-workshop).

## Ziele
Nach dem Workshop solltet ihr folgendes können:
- Grundlegende Informationen zu NMEA0183 und NMEA2000 finden und verstehen
- Aufbau eines NMEA2000-Netzwerks auf einem Steckbrett (ESP32, CAN-Bus-Transceiver)
- Die Arduino-IDE installieren
- Die nötigen Bibliotheken installieren (ZIP-Datei und Bibliotheksverwalter)
- Grundlegende Informationen zur NMEA2000-Bibliothek finden (PGN, Datentypen)
- Arduino-IDE nutzen (Programme laden und auf den ESP32 hochladen)
- Daten von einem NMEA2000-Bus auslesen und auf dem PC darstellen (mit NMEA-Reader)
- Den Aufbau eines typischen Programms (C/C++) verstehen
- I2C-Sensoren (hier BME280) nutzen (Anschluss I2C, Bibliotheken)
- Messen von Werten (Temperatur, Luftfeuchte, Druck) und Senden entsprechender PGNs
- Nutzung von 1-Wire und Multitasking mit ESP32 (Temperatursensor DS18B20)
- Messung von Spannungen und Widerständen (Batteriespannung, Tanksensoren)
- Messung von Frequenzen (Motordrehzahl, Durchflussgeber)
- Daten mit dem ESP32 vom NMEA2000-Bus lesen (PGN) und nutzen
- Aufbau eines NMEA2000-zu-NMEA0183-WLAN-Gateways und Darstellung von simulierten Daten (NMEA-Simulator) in OpenCPN und Tablet
- Optional: Aufbau eines ESP32-Web-Servers und Senden von Daten an einen SignalK-Server

Vorkenntnisse in Programmiersprachen wie C und C++ sind hilfreich, für den Workshop aber nicht unbedingt notwendig. Die Nutzung der Beispiele und auch das eigenständige Erweitern von Programmen wird auch ohne Vorkenntnisse funktionieren.

Wenn ihr alle Vorbereitungen abgeschlossen habt, startet [hier](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Start.md) der Workshop.

# Vorbereitungen
Für den Workshop sind einige Vorbereitungen notwendig. Diese sollten vor dem Workshop durchgeführt werden, um im Workshop mehr Zeit für die Beispiele zu haben.

Falls es bei den Vorbereitungen zu Problemen kam, können wir die Probleme im Workshop gemeinsam lösen.

## Vorbereiten der Arduino-IDE
- Download und Installation der Arduino-Entwicklungsumgebung [Link](https://www.arduino.cc/en/software)
- Installation der ESP32-Board Informationen [Link](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/esp32-jetzt-mit-boardverwalter-installieren)

In der Arduino-IDE müssen wir als Nächstes das richtige Board auswählen. Dazu klicken wir auf "Werkzeuge", "Board" und wählen dann "ESP32 Arduino" und "ESP32 Dev Module".

## Download der NMEA2000-Bibliotheken

Folgene Bibliotheken als ZIP-Dateien downloaden und installieren:
- [NMEA2000](https://github.com/ttlappalainen/NMEA2000)
- [NMEA2000_esp32](https://github.com/ttlappalainen/NMEA2000_esp32)
- [NMEA0183](https://github.com/ttlappalainen/NMEA0183)

Jeweils auf den Namen des Repositories klicken, dann den auf "Code" klicken und "Download ZIP" auswählen. Dann in der Arduino-IDE: Sketch, Bibliothek einbinden, .ZIP-Bibliothek hinzufügen auswählen und den Speicherort der heruntergeladenen Bibliotheken für jede der drei Bibliotheken nacheinander auswählen und installieren.

## Vorbereiten des Steckbretts

Ein Steckbrett (auch Breadboard), ist die einfachste Art, kleine Schaltungen aufzubauen. Wie ein Steckbrett funktioniert wird zum Beispiel [hier](https://www.exp-tech.de/blog/tutorial-breadboard-steckplatine) beschrieben.

Die Basis für den Workshop bilden zwei [ESP32-Module](https://de.wikipedia.org/wiki/ESP32) mit zusätzlichen CAN-Bus-[Transceivern](https://de.wikipedia.org/wiki/Transceiver). Der linke ESP32 dient als Schnittstelle zum NMEA-Simulator/-Reader auf dem PC/Laptop. Der rechte wird für die Beispielprogramme genutzt.

**Um Schäden durch falsche Steckverbindungen zu vermeiden, bitte den ESP32 immer zuerst von der USB-Stromversorgung trennen, bevor Steckverbindungen gesteckt oder geändert werden.**

An beiden ESP32 wird der CAN-Transceiver angeschlossen:
- 3,3 Volt an 3,3 Volt
- GND an GND
- CAN-RX an GPIO 4
- CAN-TX an GPIO 5 

Die beiden Transceiver werden dann verbunden (CAN-H mit CAN-H und CAN-L mit CAN-L).

Damit haben wir ein kleines NMEA2000-Netzwerk zum Üben. 

Eigentlich besteht ein NMEA2000-Netzwerk aus einem Backbone-Kabel, das an beiden Enden mit einem Widerstand abgeschlossen ist. Vom Backbone können dann mehrere Stichleitungen zu den Geräten abgehen. Für unser Mini-Netzwerk ist das aber nicht weiter wichtig. Der CAN-Bus ist so robust, dass es auch mit einer einfachen Direktverbindung von CAN-L und CAN-H funktioniert.

So sieht das dann auf dem Steckbrett aus:

![Basis](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/Bilder/NMEA2000-Basis_Steckplatine.png)

## Zusätzliche Programme zum Testen von Funktionen (PC und Apple/Android APP)
- Zum Lesen und Anzeigen von NMEA2000-Daten auf dem PC soll der NMEA-Reader verwendet werden: [Link](https://actisense.com/acti_software/nmea-reader/)
- Auf dem PC muss zum Senden von Daten der NMEA2000-Simulator installiert werden: [Link](http://www.kave.fi/Apps/index.html). Eventuell muss zum Entpacken des Simulators auch das Entpack-Programm [7-ZIP](https://www.7-zip.de/) installiert werden.
- Optional kann auf dem PC auch OpenCPN installiert werden (wird zum Testen des WLAN-Gateways genutzt): [Link](https://opencpn.org/OpenCPN/info/downloads.html)
- Auf dem Telefon/Tablet benötigen wir eine APP zum Anzeigen von NMEA0183-Daten. Zum Beispiel [NMEAremote(lite)](https://apps.apple.com/de/app/nmearemote-lite/id413937027) für Apple-Geräte. Die "lite"-Version kostet wohl 3,49 Euro. Für Android-Geräte bitte im App-Store nach einer geeigneten APP suchen und installieren (z.B. [AvNav](https://play.google.com/store/apps/details?id=de.wellenvogel.avnav.main&hl=de&gl=DE) oder [nke Display](https://play.google.com/store/apps/details?id=nke.appandroid.activities&hl=de&gl=DE)). Ich konnte das leider nicht testen.

## Workshop-Beispielprogramme
Auf dem rechten ESP32 werden unterschiedliche Programme installiert, um die Nutzung der NMEA2000-Bibliothek zu erklären. 

Mit dem NMEA-Reader können wir Daten vom NMEA2000-Netzwerk anzeigen, und mit dem NMEA-Simulator auch Daten an das Netzwerk senden. Beides werden wir im Workshop nutzen.

Hier [startet](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Start.md) der Workshop.

# Material für den Workshop
Folgende Materialien werden für den Workshop benötigt:

- Steckbrett (Spannungsversorgung Mitte) [Link](https://www.reichelt.de/experimentier-steckboard-1260-300-kontakte-steckboard-sb-3-p139508.html?&trstct=pol_5&nbc=1)
- 2* ESP32 [Link](https://www.amazon.de/AZDelivery-NodeMCU-Development-Nachfolgermodell-ESP8266/dp/B071P98VTG/ref=sxts_sxwds-bia-wc-drs3_0?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&cv_ct_cx=ESP32&dchild=1&keywords=ESP32)
- 2* CAN-Bus-Transceiver [Link](https://eckstein-shop.de/Waveshare-SN65HVD230-CAN-Board-33V-ESD-protection)
- BME280-Sensor [Link](https://www.reichelt.de/entwicklerboards-temperatur-feuchtigkeits-und-drucksensor--debo-bme280-p253982.html?&trstct=pos_1&nbc=1). Die Stiftleiste bitte anlöten. Am Besten so herum, dass man die Kontaktbezeichnungen gesteckt noch lesen kann.
- DS18B20-Temperaturfühler [Link](https://www.reichelt.de/digital-thermometer-1-wire--0-5-c-to-92-ds-18b20-p58169.html?search=18b20)
- Drehpotentiometer 10 kOhm [Link](https://www.reichelt.de/drehpotentiometer-10-kohm-linear-6-mm-pih-pc16ip061033-p232701.html?&nbc=1). Falls das Potentiometer nicht direkt ins Steckbrett gesteckt werden kann, bitte kurze Drahtbrücken anlöten.
- Flexible Drahtbrücken [Link](https://www.reichelt.de/flexible-drahtbruecken-15-cm-mehrfarbig-65er-pack-rnd-255-00009-p253202.html?&nbc=1&trstct=lsbght_sldr::253206)

Zusätzlich benötigen wir auch noch folgendes:
- 2* USB-Kabel (Micro, passend für ESP32)
- PC/Laptop (mit 2* USB)
- Tablet/Smartphone (Android/Apple)

Der bei Reichelt angebotene ESP32 (Joy-IT) geht prinzipiell auch. Er hat aber eine andere Pin-Belegung als der von AZ-Delivery. Im Workshop nutzen wir den ESP32 von AZ-Delivery.
