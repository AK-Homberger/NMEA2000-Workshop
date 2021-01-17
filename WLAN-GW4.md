# Web-Server mit AJAX

Als letztes wollen wir unser WLAN-Gateway noch mit einem Web-Server ausstatten.

Laden wir dazu das Programm [NMEA2000-WLAN-Gateway3.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA2000-WLAN-Gateway3/NMEA2000-WLAN-Gateway3.ino) auf den rechten ESP32.

Der NMEA-Simulator sollte auch gestartet sein.

Dann verbinden wir uns wieder mit dem WLAN ung starten dann den Web-Browser. Als Adresse geben wir "192.168.4.1" ein.
Nun sehen wir die Web-Seite des WLAN-Gateways.

![Web-Server](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/Web-Server.PNG);







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
