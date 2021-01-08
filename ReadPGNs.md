# Lesen vom NMEA2000-Bus

Bisher haben wir immer Daten auf den NMEA2000-Bus gesendet. Nun kommen wir zur anderen Richtung. Wir werden jetzt Daten vom NMEA2000-Bus lesen und im Seriellen Monitor anzeigen.

Wir fangen mit einem einfachen Beispiel an und werden es dann erweitern.

Zum Lesen von Daten benötigen wir nur das Basis-Steckbrett mit den beiden ESP32 und den CAN-Bus Transceivern. Es macht aber auch nichts, wenn noch Komponenten vom vorigen Beispiel geteckt sind.

![Basis-Steckbrett](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-Basis_Steckplatine.png)

Wir laden nun das Beispielprogramm [NMEA2000-Reade.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/NMEA2000-Reader/NMEA2000-Reader.ino) in die Arduino-IDE.

Danach können wir das Programm auf den rechten ESP32 hochladen.

Auf dem PC starten wir das Programm "NMEA-Simulator". Einen eventuell zuvor gestartetes NMEA-Reader-Programm beenden wir, damit die serielle Schnittstelle zur Verfügung steht. Im NMEA-Simulator konfigurierren wird die Serielle Snittstelle für NMEA2000. Das geht mit "Tools", "Options". Dann wählen wir die serielle Schnittselle vom linken ESP32. Danach OK klicken.

![Simulator](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA-Simulator1.png)

Jetzt können wir im Simulator Werte ändern und die Motordrehzahl erhöhen "Throttle Gear" hochschieben.

Danach wählen wir die CheckBox "Run" aus.

Im Seriellen Monitor der Arduino-IDE sollen jetzt Zeilen ausgegeben werden:

```
PGN127250: Heading: 272.2 Deviation: 1.0 Variation: 2.0
```







