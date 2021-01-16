# NMEA0183 Multiplexer

Jetzt fügen wir dem Gateway noch eine Multiplexing-Funktion für AIS hinzu.

Multiplexing bedutet, dass man NMEA0183-Nachrichten von mehreren Quellen zu einem kombinierten Nachrichtenstrom zusammen setzt.

Zusätzlich zu den gewandelten NMEA2000-Daten wollen wir nun auch noch AIS über eine serielle Schnittstelle einlesen und auch an die Clients weiterleiten.

Für die Pegelwandlung von RS-232 (oder RS-433) benötigen wir eine kleine [Schaltung](https://github.com/AK-Homberger/NMEA2000WifiGateway-with-ESP32).

Die Erweiterung des WLAN-Gateways um eine Multiplexing-Funktion ist ganz einfach. Es sind nur wenige Zeilen Code nötig.

Fügt folgende globale Objekte hinzu:
```
// NMEA message and stream for AIS receiving and multiplexing
tNMEA0183Msg NMEA0183Msg;
tNMEA0183 NMEA0183;
```
Das erzeugt einen NMEA0183-Nachrichten-Container und auch eine Methode zur Behandlung eines NMEA0183-Nachrichten-Streams.

Dann fügt ihr in setup() nach der Initialisierung der seriellen USB-Schnittstelle die Initialisierung der 2. seriellen Schnittstelle (GPIO 16):

```
// Init AIS serial port 2
  Serial2.begin(38400, SERIAL_8N1);      // Configure Serial2 (GPIO 16)
  NMEA0183.Begin(&Serial2, 3, 38400);    // Start NMEA0183 stream handling
```

In loop() dann noch folgende Zeilen ergänzen:

```
if (NMEA0183.GetMessage(NMEA0183Msg)) {  // Get AIS NMEA sentences from serial2
    SendNMEA0183Message(NMEA0183Msg);    // Send to clients
  }
```

Das war alles. Nun habt ihr ein NMEA2000 zu NMEA0183-WLAN-Gateway mit einem NMEA0183-Multiplexer.
Das fertige Programm ist [hier](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/NMEA2000-WLAN-Gateway2/NMEA2000-WLAN-Gateway2.ino) gespeichert.

Leider können wir die Funktion hier nicht testen.

# Das war es schon
Das war es nun mit dem Workshop.

Kommen wir nun noch einmal zu den Zielen des Workshops zurück und sehen nach, ob wir alle Ziele erreicht haben.

Ihr solltet nun folgendes können:

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

Ich glaube, wir können alles abhaken!

Vielen Dank für die Teilnahme am Workshop.
Feedback wird gern entgegengenommen.
