# NMEA0183-Multiplexer

Jetzt fügen wir dem Gateway noch eine Multiplexing-Funktion für AIS hinzu.

Multiplexing bedeutet, dass man NMEA0183-Nachrichten von mehreren Quellen zu einem kombinierten Nachrichtenstrom zusammen setzt.

Zusätzlich zu den gewandelten NMEA2000-Daten wollen wir nun auch noch AIS über eine serielle Schnittstelle einlesen und an die Clients weiterleiten.

Für die Pegelwandlung von [RS-232](https://de.wikipedia.org/wiki/RS-232) (oder [RS-422](https://de.wikipedia.org/wiki/EIA-422)) würden  wir noch eine kleine Schaltung benötigen. Der Schaltplan ist [hier](https://github.com/AK-Homberger/NMEA2000WifiGateway-with-ESP32), siehe Eingang AIS. Für den Workshop lassen wir das aber, da wir sowieso keinen AIS-Empfänger zum Testen haben.

Die Erweiterung des WLAN-Gateways um eine Multiplexing-Funktion ist ganz einfach. Es sind nur wenige Zeilen Code nötig.

Fügt als erstes folgende globalen Objekte hinzu:
```
// NMEA message and stream for AIS receiving and multiplexing
tNMEA0183Msg NMEA0183Msg;
tNMEA0183 NMEA0183;
```
Das erzeugt einen NMEA0183-Nachrichten-Container und auch eine Methode zur Behandlung eines NMEA0183-Nachrichten-Streams.

Dann fügt ihr in setup(), nach der Initialisierung der seriellen USB-Schnittstelle, die Initialisierung der zweiten seriellen Schnittstelle hinzu (RX an GPIO 16):

```
// Init AIS serial port 2
  Serial2.begin(38400, SERIAL_8N1);      // Configure Serial2 (GPIO 16)
  NMEA0183.Begin(&Serial2, 3, 38400);    // Start NMEA0183 stream handling
```
AIS wird üblicherweise mit 38400 Baud übertragen. "SERIAL_8N1" bedeutet 8 Bit, kein Paritätsbit und  ein Stopbit. Mit NMEA0183.Begin() wird dann die Stream-Behandlung für die zweite serielle Schnittstelle gestartet.


In loop() dann noch folgende Zeilen ergänzen:

```
if (NMEA0183.GetMessage(NMEA0183Msg)) {  // Get AIS NMEA sentences from serial2
    SendNMEA0183Message(NMEA0183Msg);    // Send to clients
  }
```
Hier wird mit NMEA0183.GetMessage() geschaut, ob eine Nachricht erkannt wurde. Falls ja, wird sie mit SendNMEA0183Message() an die Clients gesendet.

Das war alles. Nun habt ihr ein NMEA2000 zu NMEA0183-WLAN-Gateway mit einem zusätzlichen NMEA0183-Multiplexer.
Das fertige Programm ist [hier](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA2000-WLAN-Gateway2/NMEA2000-WLAN-Gateway2.ino) gespeichert.

Falls ihr nur einen NMEA0183-WLAN-Multiplexer ohne NMEA2000 benötigt, könnt ich auch diesen [Multiplexer](https://github.com/AK-Homberger/NMEA0183-WiFi-Multiplexer) nutzen.

Leider können wir die Funktion hier nicht testen.

Als Bonus-Material könnten wir unser Gateway noch mit einem [Web-Server](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/WLAN-GW4.md) erweitern.

Oder aber wir [beenden](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/Ende.md) hier den Workshop.
