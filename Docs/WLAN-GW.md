# Aufbau eines NMEA2000 WLAN-Gateways

Im letzten Teil des Workshops werden wir auf dem Lesen vom NMEA2000-Bus aufbauen und ein NMEA2000 zu NME0183 WLAN-Gateway programmieren.

Das Programm wird definierte PGNs vom Bus lesen, die Daten nach NMEA0183 umwandeln und dann über WLAN an Clients weitergeben. Zusätzlich werden die gewandelten Daten auch über die serielle Schnittstelle ausgegeben.

Als Clients werden wir sowohl OpenCPN auf dem PC als auch das Programm NMEAremote auf dem Telefon/Tablet nutzen.

Für das WLAN-Gateway starten wir wieder mit unserem Basis-Steckbrett, wie auch beim vorherigen Teil.

![Steckbrett](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-Basis_Steckplatine.png)

Es macht aber auch nichts, wenn Bauteile aus früheren Beispielen gesteckt bleiben. Sie stören nicht.

Danach laden wir aus dem entpackten ZIP-Ordner das Programm [NMEA2000-WLAN-Gateway.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA2000-WLAN-Gateway/NMEA2000-WLAN-Gateway.ino). Wie immer, mit einem Doppelklick auf die Datei.

![Arduino-IDE](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/Arduino-IDE-GW.png).

Anders als bei den bisherigen Beispielen, sehen wir in der Arduino-IDE nun mehrere Programm-Reiter: "NMEA2000-WLAN-Gateway", "BoatData.h", "List.h" usw. Doch dazu kommen wir später.

Erst einmal wollen wir sehen, was das Programm macht. Dazu laden wir es auf den rechten ESP32 hoch.

### Tipp: Wenn sich mehrere Workshop-Teilnehmer in einem Raum befinden, sollte erst einmal nur einer das Programm starten.

Wenn das Programm erfolgreich gestartet wurde, kann nun auf dem Telefon/Tablet nach einem Access-Point mit dem Namen "NMEA2000-Gateway" gesucht werden.

Das WLAN wird ausgewählt und als Passwort wird "password" eingegeben.

Dann wird die App zur Anzeige gestartet. Für Apple-Geräte ist das NMEAremote(lite). 

# Apple: NMEAremote lite:

Als Erstes muss die Daten-Quelle festgelegt werden. Dazu in der App links oben tippen und "NMEAremote" anwählen. Dann auf "Quellen" tippen und "NMEA over IP" wählen.

Dort [TCP](https://de.wikipedia.org/wiki/Transmission_Control_Protocol) wählen, die IP-Adresse (192.168.4.1) und den Port (2222) eintragen und dann "Aktiviert" einschalten.

Das sollte dann wie folgt aussehen:

![iPad1](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/IMG_0937.PNG)

Wenn auf dem PC der NMEA-Simulator läuft, sollten im blauen Fenster auch gleich empfangene NMEA0183-Daten angezeigt werden. Ansonsten jetzt den NMEA-Simulator starten (Checkbox "Run" nicht vergessen).

Dann kann man die Seite verlassen und durch rechts/links wischen die unterschiedlichen Anzeigeseiten durchblättern.

Hier ein paar Beispiele:

![iPad2](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/IMG_0938.PNG)

![iPad3](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/IMG_0939.PNG)

NMEAremote unterstützt auch das SeaSmart- und Actisense-Format. Mit leichten Änderungen im Beispielprogramm kann man auch "Seasmart" ausprobieren. 

Im Programm dazu folgendes ändern und dass das Programm hochladen:

```
bool SendSeaSmart = true;          // Do we send NMEA2000 messages in SeaSmart format
```

In NMEAremote nun die Quelle "NMEA over IP" löschen und eine neue Quelle "SeaSmart.NET (NMEA 2000)" auswählen. Dann TCP wählen, die IP-Adresse (192.168.4.1) und den Port (2222) eintragen und dann "Aktiviert" einschalten.

Nach Verlassen der Seite sieht man die angezeigten Daten. 

# Android

Für Android können zum Beispiel die Programme "AvNav" oder auch "NKE Display" für die Anzeige der Daten genutzt werden.

Mit Bildschirmfotos kann ich hier nicht dienen, da ich kein Android-System nutze.


# OpenCPN
Sofern auf dem PC OpenCPN läuft, kann man nach Konfiguration der seriellen Schnittstelle (zweiter ESP32) und Konfiguration des OpenCPN-Dashboard-Plugins auch hier die vom NMEA-Simulator gesendeten Daten sehen:

![OpenCPN](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/OpenCPN.png)

# AvNav auf PC
Da AvNav auch auf einem PC läuft, kann ich euch zumindest zeigen, wie es hier aussieht. Die Daten bekommt AvNav hier auch über die serielle Schnittstelle.

![AvNav](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/AvNav.png)

Ihr könnt nun gern etwas mit dem Simulator und dan Anzeigeprogrammen herumprobieren.

Die Daten werden sowohl auf der seriellen Schnittstelle des ESP32 als auch per WLAN ausgegeben.

Es funktioniert also. 

Kommen wir nun aber zum [Programm](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/WLAN-GW2.md) selbst.

