# Aufbau eines NMEA2000 WLAN-Gateways

Im letzten Teil des Workshops werden wir auf dem Lesen vom NMEA2000-Bus aufbauen und ein NMEA2000 zu NME0183 WLAN-Gateway programmieren.

Das Programm wird definierte PGNs vom Bus lesen, die Daten nach NMEA0183 umwandeln und dann über WLAN an Clients weitegeben. Zusätzlich werden die gewandelten Daten auch über die serielle Schnittstelle ausgegeben.

Als Clients werden wir sowohl OpenCPN auf dem PC als auch das Programm NMEAremote auf dem Telefon/Tablet nutzen.

Für das WLAN-Gateway starten wir wieder mit unserem Basis-Steckbrett, wie auch beim letzen Teil.

![Steckbrett](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-Basis_Steckplatine.png)

Danach laden wir aus dem entpackten ZIF-Ordner das Programm [NMEA2000-WLAN-Gateway.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/NMEA2000-WLAN-Gateway/NMEA2000-WLAN-Gateway.ino). Wie immer mit einem Doppelklick auf die Datei.

![Arduino-IDE](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/Arduino-IDE-GW.png).

Anders als bei den bisherigen Beispielen sehen wir in der Arduino-IDE nun mehrere Programm-Reiter: "NMEA2000-WLAN-Gateway, "BoatData.h", "List.h" usw. Doch dazu kommen wir später.

Erst einmal wollen wir sehen, was das Programm macht. Dazu laden wir es auf den rechten ESP32 hoch.

Tipp: Wenn sich mehrere Workshop-Telnehmer in einem Raum bfinden sollte erst einmal nur einer das Programm starten.

Wenn das Programm erfolgreich gestarte wurde, kann nun auf dem Telefon/Tablet nach einem Access Point mit dem Namen "NMEA2000-Gateway" gesucht werden.
Das WLAN wird ausgewählt und als Passort wird "password" eingegeben.










