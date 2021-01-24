# NMEA2000-Workshop

Dieses Repository enthält Informationen zum NMEA-2000-Workshop im Januar/Februar 2021.

Die Informationen sind eigentlich für die Nutzung in einem Präsenz- oder Online-Workshop gedacht. Sie können aber auch ohne Workshop in eigener Regie genutzt werden. Alle notwendigen Informationen und Erklärungen sind im Text enthalten.

Für den Workshop sind einige Bauteile notwendig. Die Liste mit genauen Bestelldaten folgt weiter [unten](https://github.com/AK-Homberger/NMEA2000-Workshop#material-f%C3%BCr-den-workshop).

## Ziele
Nach dem Workshop solltet ihr folgendes können:
- Aufbau eines NMEA2000-Netzwerks auf einem Steckbrett (ESP32, CAN-Bus-Transceiver)
- Die Arduino-IDE installieren
- Die nötigen Bibliotheken installieren (ZIP-Datei und Bibliotheksverwalter)
- Grundlegende Informationen zur NMEA2000-Bibliothek finden (PGNs, Datentypen)
- Arduino-IDE nutzen (Programme laden und auf den ESP32 hochladen)
- Daten von einem NMEA2000-Bus auslesen und auf dem PC darstellen (mit NMEA-Reader)
- Den Aufbau eines typischen Programms (C/C++) verstehen
- I2C-Sensoren (hier BME280) nutzen (Anschluss I2C, Bibliotheken)
- Messen von Werten (Temperatur, Luftfeuchte, Druck) und Senden entsprechender PGNs
- Nutzung von 1-Wire und Multitasking mit ESP32 (Temperatursensor DS18B20)
- Messung von Spannungen und Widerständen (ESP32-ADC nutzen)
- Messung von Frequenzen (ESP32-Interrupts nutzen)
- Spezifische Daten mit dem ESP32 vom NMEA2000-Bus lesen (PGNs) und nutzen
- Aufbau eines NMEA2000-zu-NMEA0183-WLAN-Gateways und Darstellung von simulierten Daten (NMEA-Simulator) in OpenCPN und Tablet

Vorkenntnisse in Programmiersprachen wie C und C++ sind hilfreich, für den Workshop aber nicht unbedingt notwendig. Die Nutzung der Beispiele und auch das eigenständige Erweitern von Programmen wird auch ohne Vorkenntnisse funktionieren.

## NMEA2000-Historie

NMEA2000 ist eine standardisierte Erweiterung des älteren Protokolls [NMEA0183](https://de.wikipedia.org/wiki/NMEA_0183). Grundlegende Informationen zu NMEA2000 sind [hier](https://de.wikipedia.org/wiki/NMEA_2000) zu finden.

Im Rahmen des Workshops werden wir ein kleines NMEA2000-Netzwerk aufbauen und erste Programme mit der [NMEA2000-Bibliothek](https://github.com/ttlappalainen) von Timo Lappalainen erstellen und ausprobieren.

Die üblichen Hersteller verkaufen ihre NMEA2000-/CAN-Bus-Kabel zu extremen Preisen. Eine günstigere Alternative ist das CAN-BUS-Kabel von Unitronic. Unter anderem erhältlich bei [Voelkner](https://www.voelkner.de/products/146612/LAPP-2170261-1-Busleitung-UNITRONIC-BUS-2-x-2-x-0.22mm-Violett-Meterware.html). Zum Beispiel in Verbindung mit schraubbaren [Micro-C-Steckverbindern](https://www.gmm-yacht.de/nmea_2000_installation.html).

Für kleinere Netzwerke funktionieren aber auch alle anderen paarweise verdrillten Leitungen (inclusive [CAT-5](https://www.reichelt.de/cat-5e-patchkabel-sf-utp-25m-4x2-awg26-7-25s-p505.html?&nbc=1&trstct=lsbght_sldr::2520) Ethernet-Netzwerkkabel). Gerne auch geschirmt; der [CAN-Bus-Standard](https://de.wikipedia.org/wiki/Controller_Area_Network#Maximale_%C3%9Cbertragungsrate_und_Leitungsl%C3%A4nge) fordert das aber nicht. 

Für die feuchte Bootsumgebung ist aber das Kabel allein nicht entscheidend, sondern die Qualität der Verbindungen und Stecker.

## ESP32
Wir werden im Workshop den Prozessor ESP32 von Espressif verwenden. Zur einfacheren Verwendung werden wir ein "ESP32 Dev Module" verwenden, bei dem die notwendige externe Beschaltung schon vorhanden ist. Inclusive USB-Anschluss, der für die Workshop-Beispiele auch die Spannungsversorgung liefert. Für die Beispiele auf dem Steckbrett nutzen wir das [Development-Modul](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/esp32-das-multitalent) von AzDelivery. Es hat diese [Pin-Belegung](https://www.dropbox.com/s/o90qsp1rn5ol9gf/NodeMCU.png?dl=0).


## Vorbereiten der Arduino-IDE
- Download und Installation der Arduino-Entwicklungsumgebung [Link](https://www.arduino.cc/en/software)
- Installation der ESP32-Board Informationen [Link](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/esp32-jetzt-mit-boardverwalter-installieren)

In der Arduino-IDE müssen wir als Nächstes das richtige Board auswählen. Dazu klicken wir auf "Werkzeuge", "Board" und wählen dann "ESP32 Arduino" und "ESP32 Dev Module".

Download der NMEA2000-Bibliotheken

Folgene Bibliotheken als ZIP-Dateien downloaden und installieren:
- [NMEA2000](https://github.com/ttlappalainen/NMEA2000)
- [NMEA2000_esp32](https://github.com/ttlappalainen/NMEA2000_esp32)
- [NMEA0183](https://github.com/ttlappalainen/NMEA0183)

Jeweils auf den Namen des Repositories klicken, dann den auf "Code" klicken und "Download ZIP" auswählen. Dann in der Arduino-IDE: Sketch, Bibliothek einbinden, .ZIP-Bibliothek hinzufügen auswählen und den Speicherort der heruntergeladenen Bibliotheken für jede der drei Bibliotheken nacheinander auswählen und installieren.

Für die spätere Nutzung der Bibliotheken sind speziell drei Dateien wichtig:
1. NMEA2000-Library-Referenz [Link](https://github.com/ttlappalainen/NMEA2000/blob/master/Documents/NMEA2000_library_reference.pdf)
2. Header-Datei "N2kMessages.h" mit den PGN-Definitionen [Link](https://github.com/ttlappalainen/NMEA2000/blob/master/src/N2kMessages.h)
3. Header-Datei "N2kTypes.h" mit Definitonen für Datentypen in den PGNs [Link](https://github.com/ttlappalainen/NMEA2000/blob/master/src/N2kTypes.h)

## Vorbereiten des Steckbretts
Die Basis für den Workshop bilden zwei ESP32-Module mit CAN-Bus-Transceivern. Der erste ESP32 dient als Schnittstelle zum NMEA-Simulator/-Reader auf dem PC/Laptop. Der zweite wird für die Beispielprogramme genutzt.

An beiden ESP32 wird der CAN-Transceiver angeschlossen (3,3 Volt, GND, CAN-RX, CAN-TX). **Die beiden Transceiver werden verbunden (CAN-H mit CAN-H und CAN-L mit CAN-L)**.
Damit haben wir ein kleines NMEA2000-Netzwerk zum Üben. 

Eigentlich besteht ein NMEA2000-Netzwerk aus einem Backbone-Kabel, das an beiden Enden mit einem Widerstand abgeschlossen ist. Vom Backbone können dann mehrere Stichleitungen zu den Geräten abgehen. Für unser Mini-Netzwerk ist das aber nicht weiter wichtig. Der CAN-Bus ist so robust, dass es auch mit einer einfachen Direktverbindung von CAN-L und CAN-H funktioniert.

So sieht das dann auf dem Steckbrett aus:

![Basis](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/Bilder/NMEA2000-Basis_Steckplatine.png)

## Vorbereiten des linken ESP32
Der einfachste Weg, die Beispielprogramme zu nutzen, ist das ganze NMEA2000-Workshop-Repository als ZIP-Datei herunterzuladen. Dazu klickt man oben auf "NMEA2000-Workshop" und dann auf den grünen Button "Code". Dann "Download ZIP" auswählen. Die heruntergeladene ZIP-Datei in einen frei wählbaren Ordner entpacken. Dann kann man die Beispielprogramme direkt in der Arduino-IDE öffnen. Dazu einfach auf die jeweilige .ino-Datei doppelklicken. Alle Programme befinden sich im Ordner "Software".

Auf dem **linken** ESP32 ist der Sketch [ActisenseListenerSender-ESP32.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/ActisenseListenerSender-ESP32/ActisenseListenerSender-ESP32.ino) zu installieren. Dazu im entpackten ZIP-Ordner die Datei "ActisenseListenerSender-ESP32.ino" doppelklicken. Die Arduino-IDE startet dann mit geladenem Sketch. Dann nur den linken ESP32 mit USB verbinden und in der Arduino-IDE den seriellen USB-Port auswählen und "Sketch" "Hochladen" anwählen. **Den seriellen Port bitte notieren**.

Im unteren Statusfenster wird beim Übertragen "Connecting........_____....._____.." angezeigt. Normalerweise erfolgt das Hochladen nach ein paar Sekunden automatisch. Falls die Fehlermeldung "A fatal error occurred: Failed to connect to ESP32:" angezeigt wird, kann es helfen, während des Upload-Prozesses, die "Boot"-Taste auf dem ESP2-Board gedrückt zu halten, bis der Upload-Prozess startet.

Sobald "Leaving...  Hard resetting via RTS pin..." angezeigt wird, ist das Programm erfolgreich übertragen und auch schon gestartet worden.

## Zusätzliche Programme zum Testen von Funktionen (PC und Apple/Android APP)
- Zum Lesen und Anzeigen von NMEA2000-Daten auf dem PC soll der NMEA-Reader verwendet werden: [Link](https://actisense.com/acti_software/nmea-reader/)
- Auf dem PC muss zum Senden von Daten der NMEA2000-Simulator installiert werden: [Link](http://www.kave.fi/Apps/index.html).
- Optional kann auf dem PC auch OpenCPN installiert werden (wird zum Testen des WLAN-Gateways genutzt): [Link](https://opencpn.org/OpenCPN/info/downloads.html)
- Auf dem Telefon/Tablet benötigen wir eine APP zum Anzeigen von NMEA0183-Daten. Zum Beispiel [NMEAremote(lite)](https://apps.apple.com/de/app/nmearemote-lite/id413937027) für Apple-Geräte. Die "lite"-Version kostet wohl 3,49 Euro. Für Android-Geräte bitte im App-Store nach einer geeigneten APP suchen und installieren (z.B. [AvNav](https://play.google.com/store/apps/details?id=de.wellenvogel.avnav.main&hl=de&gl=DE) oder [nke Display](https://play.google.com/store/apps/details?id=nke.appandroid.activities&hl=de&gl=DE)). Ich konnte das leider nicht testen.

## Workshop-Beispielprogramme
Auf dem rechten ESP32 werden unterschiedliche Programme installiert, um die Nutzung der NMEA2000-Bibliothek zu erklären. Dazu wird der ESP32 mit USB verbunden und in der Arduino-IDE der neue serielle USB-Port ausgewählt.

Mit dem NMEA-Reader können wir Daten vom NMEA2000-Netzwerk anzeigen, und mit dem NMEA-Simulator auch Daten an das Netzwerk senden. Beides werden wir im Workshop nutzen.

Folgende Beispielprogramme werden im Workshop gemeinsam erstellt/erweitert und mit dem Reader/Simulator getestet:

1. [NMEA2000-Thermometer, Hygrometer und Barometer mit BME280 (I2C)](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/Docs/BME280.md)
2. [Temperaturfühler mit DS18B20 (1-Wire)](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/Docs/DS18B20.md)
3. [Spannungs- und Widerstandsmessung (Batteriespannung, Tankanzeige)](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/AnalogRead.md)
4. [Messung von Frequenzen (ESP32-Interrupts nutzen)](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/Frequenz.md)
5. [Daten (PGNs) vom NMEA-2000-Bus lesen und darstellen](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/ReadPGNs.md)
6. [NMEA2000-WLAN-Gateway und Anzeige der Daten auf OpenCPN und Tablet/Telefon](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/WLAN-GW.md)
7. [Gateway plus AIS-Multiplexing](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/WLAN-GW3.md)
8. [Optional: Gateway mit Web-Server](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/WLAN-GW4.md)


# Material für den Workshop

Folgende Materialien werden für den Workshop benötigt:

- Steckbrett (Spannungsversorgung Mitte): [Link](https://www.reichelt.de/experimentier-steckboard-1260-300-kontakte-steckboard-sb-3-p139508.html?&trstct=pol_5&nbc=1)
- 2* ESP32 [Link](https://www.amazon.de/AZDelivery-NodeMCU-Development-Nachfolgermodell-ESP8266/dp/B071P98VTG/ref=sxts_sxwds-bia-wc-drs3_0?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&cv_ct_cx=ESP32&dchild=1&keywords=ESP32)
- 2* CAN-Bus-Transceiver [Link](https://eckstein-shop.de/Waveshare-SN65HVD230-CAN-Board-33V-ESD-protection)
- BME280-Sensor [Link](https://www.reichelt.de/entwicklerboards-temperatur-feuchtigkeits-und-drucksensor--debo-bme280-p253982.html?&trstct=pos_1&nbc=1)
- DS18B20-Temperaturfühler [Link](https://www.reichelt.de/digital-thermometer-1-wire--0-5-c-to-92-ds-18b20-p58169.html?search=18b20)
- Drehpotentiometer 10 KOhm [Link](https://www.reichelt.de/drehpotentiometer-10-kohm-linear-6-mm-pih-pc16ip061033-p232701.html?&nbc=1)
- Flexible Drahtbrücken [Link](https://www.reichelt.de/flexible-drahtbruecken-15-cm-mehrfarbig-65er-pack-rnd-255-00009-p253202.html?&nbc=1&trstct=lsbght_sldr::253206)

2* USB-Kabel (Micro, passend für ESP32)

1 PC/Laptop (mit 2 * USB)

1* Tablet/Telefon (Android/Apple)

Der bei Reichelt angebotene ESP32 (Joy-IT) geht prinzipiell auch. Er hat aber eine andere Pin-Belegung als der von AZ-Delivery. Im Workshop nutzen wir den ESP32 von AZ-Delivery.
