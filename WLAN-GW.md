# Aufbau eines NMEA2000 WLAN-Gateways

Im letzten Teil des Workshops werden wir auf dem Lesen vom NMEA2000-Bus aufbauen und ein NMEA2000 zu NME0183 WLAN-Gateway programmieren.

Das Programm wird definierte PGNs vom Bus lesen, die Daten nach NMEA0183 umwandeln und dann über WLAN an Clients weitegeben. Zusätzlich werden die gewandelten Daten auch über die serielle Schnittstelle ausgegeben.

Als Clients werden wir sowohl OpenCPN auf dem PC als auch das Programm NMEAremote auf dem Telefon/Tablet nutzen.

Für das WLAN-Gateway starten wir wieder mit unserem Basis-Steckbrett, wie auch beim letzen Teil.

![Steckbrett](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-Basis_Steckplatine.png)

Danach laden wir aus dem entpackten ZIF-Ordner das Programm [NMEA2000-WLAN-Gateway.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/NMEA2000-WLAN-Gateway/NMEA2000-WLAN-Gateway.ino). Wie immer mit einem Doppelklick auf die Datei.

![Arduino-IDE](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/Arduino-IDE-GW.png).

Anders als bei den bisherigen Beispielen sehen wir in der Arduino-IDE nun mehrere Programm-Reiter: "NMEA2000-WLAN-Gateway", "BoatData.h", "List.h" usw. Doch dazu kommen wir später.

Erst einmal wollen wir sehen, was das Programm macht. Dazu laden wir es auf den rechten ESP32 hoch.

### Tipp: Wenn sich mehrere Workshop-Teilnehmer in einem Raum befinden, sollte erst einmal nur einer das Programm starten.

Wenn das Programm erfolgreich gestartet wurde, kann nun auf dem Telefon/Tablet nach einem Access Point mit dem Namen "NMEA2000-Gateway" gesucht werden.
Das WLAN wird ausgewählt und als Passort wird "password" eingegeben.

Dann wird die App zur Anzeige gestartet. Für Apple ist das NMEAremote(lite). 

Als erstes muss die Daten-Quelle fstgelegt werden. Dazu in der App liks oben tippen und "NMEAremote" anwählen. Dann auf "Quellen" tippen und "NMEA over IP" wählen.
Dort TCP wählen, die IP-Adresse (192.168.4.1) und den Port (2222) eintragen und dann "Aktiviert" einschalten.

Das sollte dann wie folgt aussehen:

![iPad1](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/IMG_0937.PNG)

Wenn auf dem PC der NMEA-Simulator läuft, sollten im blauen Fenster auch gleich empfangene NMEA0183-Daten angezeigt werden. Ansonsten jetzt den NMEA-Simulater starten.

Dann kann man die Seite veralssen und durch rechts/links wischen die unterschiedlichen Anzeigeseiten durchblättern.

Hier ein paar Beispiele:

![iPad2](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/IMG_0938.PNG)

![iPad3](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/IMG_0939.PNG)

Sofern auf dem PC OPenCPN läuft kann man nach Konfiguration der seiellen Schnittstelle (zeiter ESP32) und konfiguration des OpenCPN-Dashboard-Plugins auch die vom NMEA-Simulator gesendeten Daten sehen:

![OpenCPN](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/OpenCPN.png)





