# Spannungs- und Widerstandsmessung (Bordspannung, Tankanzeige)

Als nächsten sehen wir uns an, wie wir mit dem ESP32 Spannungen un Widerstände messen können. Ziel ist:

1. Die die Bordspannung zu messen und als PGN127508 zu senden
2. Den Tanklevel des Wasertanks zu messen und als PGN127505 zu senden

Wir nutzen dazu die Analog-Digital-Converter (ADC) Funktionen des ESP32. Der ESP32 hat zwei unabhängige ADCs, die durch 18 unterschiedliche pins angesteuert werden können.

Wir nutzen hier die Pins 34 und 35 (ADC1_CH6 und ADC1_CH7). Zur vereinfachung verwenden wir ein einfaches 10 KOhm Potentiometre um sowohl die Bordspannung als auch den Tanklevel zu simulieren.

![AnalogRead](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-V-R_Steckplatine.png)

Das Potentiometer wird, wie oben gezeigt, mit 3,3 V, GND und den Pins 34/35 verbunden.

Um wirklich die Boardspannung messen zu können, würde ein Spannungsteile mit zwei Widertänden verwendet werde.
Um den Tanklevel zum Beispiel mit einem Tankgeber von Pilippi (0 - 180 Ohm) zu messen, wäre ein Widerstand von 1 K Ohm nötig.

Die genaue Beschaltung ist im Repository [NMEA-2000-Data-Sender](https://github.com/AK-Homberger/NMEA2000-Data-Sender) dargestellt.


![Spannung](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEAReader-5.png)

![TankLevel](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEAReader-6.png)

