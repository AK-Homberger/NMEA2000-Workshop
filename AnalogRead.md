# Spannungs- und Widerstandsmessung (Bordspannung, Tankanzeige)

Als nächsten sehen wir uns an, wie wir mit dem ESP32 Spannungen un Widerstände messen können. Ziel ist:

1. Die die Bordspannung zu messen und als PGN127508 zu senden
2. Den Tanlkevel des Wasertanks zu mesen und als PGN127505 zu senden

Wir nutzen dazu die Analog-Digital-Converter (ADC) Funktionen des ESP32. 


![AnalogRead](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-V-R_Steckplatine.png)


![Spannung](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEAReader-5.png)

![TankLevel](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEAReader-6.png)

