# Lesen vom NMEA2000-Bus

Bisher haben wir immer Daten auf den NMEA2000-Bus gesendet. Nun kommen wir zur anderen Richtung. Wir werden jetzt Daten vom NMEA2000-Bus lesen und im Seriellen Monitor anzeigen.

Wir fangen mit einem einfachen Beispiel an und werden es dann erweitern.

Zum Lesen von Daten benötigen wir nur das Basis-Steckbrett mit den beiden ESP32 und den CAN-Bus Transceivern. Es macht aber auch nichts, wenn noch Komponenten vom vorigen Beispiel geteckt sind.

![Basis-Steckbrett](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-Basis_Steckplatine.png)

Wir laden nun das Beispielprogramm NMEA2000-Reade.ino in die Arduino-IDE


