# Spannungs- und Widerstandsmessung (Bordspannung, Tankanzeige)

Als nächstes sehen wir uns an, wie wir mit dem ESP32 Spannungen und Widerstände messen können. Ziel ist es:

1. Die die Bordspannung zu messen und als PGN127508 zu senden.
2. Den Tanklevel des Wasertanks zu messen und als PGN127505 zu senden.

Wir nutzen dazu die Analog-Digital-Converter (ADC) Funktionen des ESP32. Der ESP32 hat zwei unabhängige ADCs, die durch 18 unterschiedliche Pins angesteuert werden können.

Wir nutzen hier die Pins 34 und 35 (ADC1_CH6 und ADC1_CH7). Zur Vereinfachung verwenden wir im Workshop ein einfaches 10 KOhm Potentiometer, um sowohl die Bordspannung als auch den Tanklevel zu simulieren.

So muss das Steckbrett dazu aussehen:

![AnalogRead](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-V-R_Steckplatine.png)

Das Potentiometer wird, wie oben gezeigt, mit 3,3 V, GND und den Pins 34/35 verbunden.

Um wirklich die Boardspannung messen zu können, würde ein Spannungsteiler mit zwei Widerständen (100 KOhm / 27 KOhm) verwendet werden.
Um den Tanklevel zum Beispiel mit einem Tankgeber von Pilippi (0 - 180 Ohm) zu messen, wäre ein Widerstand von 1 KOhm nötig (und ggf. eine Z-Diode zum Schutz ees Eingangs).

Die genaue Beschaltung ist im Repository [NMEA-2000-Data-Sender](https://github.com/AK-Homberger/NMEA2000-Data-Sender) dargestellt.

Kommen wir num zum Programm.

Aus dem enpackten Downlod laden wir num das Programm


![Spannung](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEAReader-5.png)

![TankLevel](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEAReader-6.png)

