# NMEA2000-Workshop

Wenn alle [Vorbereitungen](https://github.com/AK-Homberger/NMEA2000-Workshop#vorbereitungen) erfolgreich durchgeführt wurden, können wir nun starten.

## NMEA2000
NMEA2000 ist ein von der National Marine Electronics Association ([NMEA](https://de.wikipedia.org/wiki/National_Marine_Electronics_Association)) definierter internationaler Standard, der in der Schifffahrt zur Vernetzung von Geräten verwendet wird. NMEA2000 wurde im Jahr 2000 von der NMEA als Nachfolger des älteren Protokolls [NMEA0183](https://de.wikipedia.org/wiki/NMEA_0183) vorgestellt.

Anders als NMEA0183 definiert NMEA2000 nicht nur eine Schnittstelle, sondern ein Netzwerk-System. Es basiert auf dem Standard [SAE J1939](https://de.wikipedia.org/wiki/SAE_J1939) aus dem Nutzfahrzeugbereich und nutzt zur Datenübertragung die [CAN-Bus-Technologie](https://de.wikipedia.org/wiki/Controller_Area_Network).

NMEA2000 nutzt den CAN-Bus mit einer Datenrate von 250 kBit/s und ist damit über 50-mal schneller als NMEA0183 (4800 Bit/s).
NMEA2000 ist ein selbstkonfigurierendes System. Es funktioniert ohne eine zetrale Steuerinstanz. Alle notwendigen Konfigurationen (z. Adresszuweisung) erledigen die Geräte selbstständig durch Kommunikation miteinander.

### Elektrische Schnittstelle
NMEA2000 definiert zusätzlich zu CAN-Bus Anforderungen für die Kabeltypen und die einzelnen Adern des Kabels.
Es werden zwei paarweise verdrillte Adrenpaare mit zusätzlicher Schirmung gefordert ([S/UTP](https://de.wikipedia.org/wiki/Twisted-Pair-Kabel#S/UTP,_F/UTP_oder_SF/UTP)).

Ein Adernpaar ist für das Signal (CAN_High, CAN_Low) und ein Adernpaar ist für die Versorgungsspannung (12 Volt, GND).

Folgende Farben sind festgelegt:

| Farbe   | Name     | Spannung | Funktion             |
|-------- |----------|----------|----------------------|
| weiß    | CAN_High | +2,5 V   | Signal               |
| blau    | CAN_Low  | -2,5 V   | Signal               |
| Schirm  | Masse    | GND      | Abschirmung          |
| rot     | V+       | +12 V    | Spannungsversorgung  |
| schwarz | V-       | GND      | Spannungsversorgung  |

Die Stromversogung ist nur für Kleinverbraucher (z.B. Sensor/Anzeige) gedacht. Geräte mit höherem Strombedarf müssen extra versorgt werden. Der Maximalstrom ist mit 3 A festgelegt.

Das NMEA2000-Netzwerk darf nur an einer Stelle mit der Versorgungsspannung und Masse (GND) verbunden werden (Vermeidung von [Masseschleifen](https://de.wikipedia.org/wiki/Erdschleife)).

### NMEA-Netzwerk
Ein NMEA2000 Netzwerk besteht aus einem Backbone-Kabel mit Abschlusswiderständen an beiden Enden (120 Ohm 1/4 Watt).
Die Endwiderstände sind notwendig, um Reflexionen des Signals zu vermeiden. 120 Ohm entsprechen dem [Wellenwiderstand](https://de.wikipedia.org/wiki/Wellenwiderstand#Der_Leitungswellenwiderstand,_der_Leitungsabschluss_und_die_Eingangsimpedanz_einer_Leitung) von paarweise verdrillten Leitern. Vom Backbone-Kabel gehen Stichleitungen zu den einzelnen Geräten ab (über T-Stücke).

<p><a href="https://commons.wikimedia.org/wiki/File:NMEA2000_Modified_motor_yacht.jpg#/media/Datei:NMEA2000_Modified_motor_yacht.jpg"><img src="https://upload.wikimedia.org/wikipedia/commons/thumb/b/bf/NMEA2000_Modified_motor_yacht.jpg/1200px-NMEA2000_Modified_motor_yacht.jpg" alt="NMEA2000 Modified motor yacht.jpg"></a><br>

Es gelten folgende Grenzwerte:
- Der Abstand zwischen zwei Punkten des NMEA 2000-Netzwerks darf 100 m nicht überschreiten (Backbonelänge plus Länge der Stichleitungen).
- Die Gesamtlänge aller Stichleitungen muss weniger als 78 m sein.
- Eine Stichleitung  darf maximal 6 m lang sein.
- Es dürfen maximal 50 Geräte an das NMEA2000-Netzwerk angeschlossen werden (s. LEN-Berechnung).

**Tip zur Funktionsprüfung:** Mit dem Multimeter kann man einfach die korrekte Verkabelung der Signalleitungen prüfen. Einfach bei abgeschalteter Spannungsversorgung den Widerstand zwischen CAN_High und CAN_Low messen. Der gemessene Wert sollte ca. 60 Ohm sein.

### Datenübertragung - PGNs
Die Übertragung von Nutzdaten erfolgt mit definierten Parameter-Gruppen-Nummern (PGNs). Jedes Gerät am NMEA2000-Bus hat eine eigene Adresse. Einige PGNs sind adressierbar (Senden an ein einzelnes Gerät). Die meisten PGNs sind jedoch Broadcast-PGNs, die an alle Busteilnehmer adressiert sind.

Die aktuelle Liste der unterstützten PGNs (Stand 2015) ist [hier](https://www.nmea.org/Assets/20151026%20nmea%202000%20pgn_website_description_list.pdf) zu finden. 

Die Liste der PGNs ist nicht ausreichend, um die PGNs im Programm zu nutzen. Dazu sind weitere Informationen zur Interpretation der Felder notwendig. Diese sind nur in der lizenzpflichtigen Dokumentation des Standards enthalten.

Hersteller, die NMEA2000 in ihren Produkten nutzen, dürfen darüber hinaus auch keine weiteren Details offenlegen (s. Seite 6 im Dokument ["Certification Process Overview and License Agreement"](https://www.nmea.org/Assets/NMEA%202000%20Certification%20Process%20Overview%20and%20License%20Agreement-%20Updated%202020.pdf).

### NMEA2000-Kabel
Die üblichen Hersteller verkaufen ihre NMEA2000-/CAN-Bus-Kabel zu extremen Preisen. Eine günstigere Alternative ist das CAN-BUS-Kabel von Unitronic. Unter anderem erhältlich bei [Voelkner](https://www.voelkner.de/products/146612/LAPP-2170261-1-Busleitung-UNITRONIC-BUS-2-x-2-x-0.22mm-Violett-Meterware.html). Zum Beispiel in Verbindung mit schraubbaren [Micro-C-Steckverbindern](https://www.gmm-yacht.de/nmea_2000_installation.html).

Ein standardisiertes NMEA2000(Light)-Kabel muss einen Mindestquerschnitt haben. Für das Signal sind dies [AWG24](https://de.wikipedia.org/wiki/American_Wire_Gauge) (0,205 mm²) und für die Versorgungsspannung [AWG22](https://de.wikipedia.org/wiki/American_Wire_Gauge) (0,326 mm²). Das obige Unitronic-Kabel erfüllt die Anforderungen für das Signal. Für die standardkonforme Stromversorgung ist es etwas zu dünn (0,22 mm²). Darauf muss man bei größeren Netzwerken und der Berechnung der maximalen Last achten (s. LEN-Berechnung weiter unten).

Für kleinere Netzwerke funktionieren aber auch alle anderen paarweise verdrillten Leitungen (inclusive [CAT-5](https://www.reichelt.de/cat-5e-patchkabel-sf-utp-25m-4x2-awg26-7-25s-p505.html?&nbc=1&trstct=lsbght_sldr::2520) Ethernet-Netzwerkkabel). Gerne auch geschirmt; der [CAN-Bus-Standard](https://de.wikipedia.org/wiki/Controller_Area_Network#Maximale_%C3%9Cbertragungsrate_und_Leitungsl%C3%A4nge) fordert das aber nicht. 

Für die feuchte Bootsumgebung ist aber das Kabel allein nicht entscheidend, sondern die Qualität der Verbindungen und Stecker.

### Kabelplan
Hier ein typischer Kabelplan:

![Kabelplan](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/Kabelplan.png)

Im obigen Beispiel wird der NMEA2000/SeatalkNG-Bus durch die ACU-200 (Autopilot-Steuereinheit) mit Spannung versorgt. Der Plotter und das Funkgerät haben eine eigene Stromversorgung. Die Instrumente, das EV-1 und die Autopilot-Fernbedienung werden vom Bus mit Spannung versorgt.

Wichtig ist in diesem Zusammenhang auch der Seatalk1 zu SeatalkNG-Wandler. Der Wandler verbindet die Versorgungsspannung von Seatalk1 und SeatalkNG direkt. Es ist daher nicht erforderlich, und auch nicht erwünscht, den Seatalk1-Bus extra zu versorgen. Speziell nicht über unterschiedliche Sicherungen/Schalter.

Bei größeren Booten mit längerem Backbone-Kabel kann es sinnvoll sein, die Spannungsversorgung in der Mitte des Kabels anzuschließen. Das verringert die Spannungsverluste auf dem Kabelweg.

### Lastfaktor (LEN)
Für die Berechnung des maximalen Lastfaktors (Lastfaktor = LEN) ist die Leistungsaufnahme der angeschlossenen Geräte und die Länge des Backbone-Kabels entscheidend. Detaillierte Informationen zur Berechnung findet man in der Hersteller-Dokumenten. [Hier](https://media1.svb-media.de/media/snr/504053/pdf/manual_2018-08-24_15-45-03_b95c81e6054c9a0496a6fd235f8eee6e.pdf) zum Beipiel für Raymarine.

### Steckerbelegung / Anschluss
- Hier ist die Steckerbelegung für Raymarine SeatalkNG zu [finden](http://raymarine.custhelp.com/app/answers/detail/a_id/4319/~/steckerbelegung-f%E3%BCr-seatalkng-spur--und-backbone-kabel-%28german%29).
- Und hier für [Micro-C](https://klabauterkiste.de/wp-content/uploads/mnea2000_micro-c_steckerbelegung-1536x541.png).

Um später ein ESP32-Gerät mit den Netzwerk zu verbinden, ist es am Einfachsten, einen Stecker vom (Spur-)Kabel abzuschneiden und die Drähte gemäß der Beschreibung zu verbinden (CAN-H, CAN-L, +12V, 0V/GND). Für die Funktion sind mindestens CAN-H, CAN-L und GND notwendig. Die +12V können zusätzlich verwendet werden, um das ESP32-Gerät mit Spannung zu versorgen (über DC/DC-Wandler von 12 Volt auf 5 Volt).

## ESP32
Wir werden im Workshop den Prozessor [ESP32 von Espressif](https://www.espressif.com/en/products/socs/esp32) nutzen. Zur einfacheren Verwendung werden wir ein "ESP32 Dev Module" verwenden, bei dem die notwendige externe Beschaltung schon vorhanden ist. Inclusive USB-Anschluss, der für die Workshop-Beispiele auch die Spannungsversorgung liefert. Für die Beispiele auf dem Steckbrett nutzen wir das [Development-Modul](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/esp32-das-multitalent) von AzDelivery. Es hat diese [Pin-Belegung](https://www.dropbox.com/s/o90qsp1rn5ol9gf/NodeMCU.png?dl=0):

![ESP32](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/ESP32%20NodeMCU-small.jpg)

## NMEA2000-Bibliothek
Für die spätere Nutzung der NMEA2000-Bibliothek sind speziell drei Dateien wichtig:
1. NMEA2000-Library-Referenz [Link](https://github.com/ttlappalainen/NMEA2000/blob/master/Documents/NMEA2000_library_reference.pdf)
2. Header-Datei "N2kMessages.h" mit den PGN-Definitionen [Link](https://github.com/ttlappalainen/NMEA2000/blob/master/src/N2kMessages.h)
3. Header-Datei "N2kTypes.h" mit Definitonen für Datentypen in den PGNs [Link](https://github.com/ttlappalainen/NMEA2000/blob/master/src/N2kTypes.h)

## Download des Repositories
Der einfachste Weg, die Beispielprogramme zu nutzen, ist das ganze NMEA2000-Workshop-Repository als ZIP-Datei herunterzuladen. Dazu klickt man oben auf "NMEA2000-Workshop" und dann auf den grünen Button "Code". Dann "Download ZIP" auswählen. Die heruntergeladene ZIP-Datei in einen frei wählbaren Ordner entpacken. 

Dann kann man die Beispielprogramme direkt in der Arduino-IDE öffnen. Dazu einfach auf die jeweilige .ino-Datei doppelklicken. Alle Programme befinden sich im Ordner "Software".

Die [Fritzing](https://fritzing.org/learning/)-daten sind [hier](https://github.com/AK-Homberger/NMEA2000-Workshop/tree/main/Fritzing) gespeichert. Mit etwas Einarbeitung könnte man mit Fritzing sogar eigene Projekte mit gefertigten Leiterplatten erstellen.

## Vorbereiten des linken ESP32
Der linke ESP32 dient ja als Schnittstelle für den NMEA-Reader und den NMEA-Simulator. Beide Programme verstehen das Actisense-Format an der seriellen Schnittstelle. Damit das später funktioniert, muss ein entsprechendes Programm auf dem ESP32 installiert werden.

Auf dem **linken** ESP32 ist der Sketch [ActisenseListenerSender-ESP32.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/ActisenseListenerSender-ESP32/ActisenseListenerSender-ESP32.ino) zu installieren. 

**Übrigens:** Wenn ihr in anderen Programmen wie OpenCPN oder SignalK etwas von einem **NGT-1** lest, dann könnt ihr auch den ESP32 mit ActisenseListenerSender-ESP32-Programm nutzen. Der NGT-1 macht auch nur die Umwandlung von Actisense-Format auf NMEA2000.

![Steckbrett](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-Basis_Steckplatine.png)
Zum Installieren im entpackten ZIP-Ordner die Datei "ActisenseListenerSender-ESP32.ino" doppelklicken. Die Arduino-IDE startet dann mit geladenem Sketch (Sketch = Programm). Dann nur den linken ESP32 mit USB verbinden und in der Arduino-IDE den seriellen USB-Port auswählen und "Sketch" "Hochladen" anwählen. **Den seriellen Port bitte notieren**.

Im unteren Statusfenster wird beim Übertragen "Connecting........_____....._____.." angezeigt. Normalerweise erfolgt das Hochladen nach ein paar Sekunden automatisch. Falls die Fehlermeldung "A fatal error occurred: Failed to connect to ESP32:" angezeigt wird, kann es helfen, während des Upload-Prozesses, die "Boot"-Taste auf dem ESP2-Board gedrückt zu halten, bis der Upload-Prozess startet.

Sobald "Leaving...  Hard resetting via RTS pin..." angezeigt wird, ist das Programm erfolgreich übertragen und auch schon gestartet worden.

Das Programm wird im [Flash-Speicher](https://de.wikipedia.org/wiki/Flash-Speicher) des ESP32 gespeichert. Es muss nur einmal übertragen werden und verbleibt im Speicher, auch wenn die Versorgungsspannung unterbrochen wird. Beim nächsten Einschalten wird es automatisch wieder gestartet.

Dieser direkte, sekundenschnelle Start, ohne lange Boot-Zeiten, ist einer der wesentlichen Vorteile gegenüber Computern mit einem vollständigen Betriebssystem (Raspberry Pi, Windows 10 usw.).

## Workshop-Beispielprogramme
Auf dem **rechten** ESP32 werden unterschiedliche Programme installiert, um die Nutzung der NMEA2000-Bibliothek zu erklären. Dazu wird der ESP32 mit USB verbunden und in der Arduino-IDE der neue serielle USB-Port ausgewählt.

Mit dem NMEA-Reader können wir Daten vom NMEA2000-Netzwerk anzeigen, und mit dem NMEA-Simulator auch Daten an das Netzwerk senden. Beides werden wir im Workshop nutzen.

Folgende Beispielprogramme werden im Workshop gemeinsam erstellt/erweitert und mit dem Reader/Simulator getestet:

1. [NMEA2000-Thermometer, Hygrometer und Barometer mit BME280 (I2C)](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/Docs/BME280.md)
2. [Temperaturfühler mit DS18B20 (1-Wire)](https://github.com/AK-Homberger/NMEA-Workshop/blob/main/Docs/DS18B20.md)
3. [Spannungs- und Widerstandsmessung (Batteriespannung, Tankanzeige)](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/AnalogRead.md)
4. [Messung von Frequenzen (Motordrehzahl, Durchflussgeber)](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/Frequenz.md)
5. [Daten (PGNs) vom NMEA2000-Bus lesen und darstellen](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/ReadPGNs.md)
6. [NMEA2000-WLAN-Gateway und Anzeige der Daten auf OpenCPN und Tablet/Telefon](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/WLAN-GW.md)
7. [Gateway plus AIS-Multiplexing](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/WLAN-GW3.md)
8. [Optional: Gateway mit Web-Server](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/WLAN-GW4.md)
9. [Optional: Senden von Daten an einen SignalK-Server](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/BME280-3-SignalK.md)


