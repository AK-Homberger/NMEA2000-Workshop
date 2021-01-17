# Auflösung: Anzeige der Wassertiefe

Hier kommt die Auflösung der Workshop-Aufgabe.

Als erstes müssen wir die Funktion MyHandleNMEA2000Msg() um PGN128267 und die aufzurufene Funktion HandleDepth() erweitern:

```
void MyHandleNMEA2000Msg(const tN2kMsg &N2kMsg) {

  switch (N2kMsg.PGN) {
    case 127250L: HandleHeading(N2kMsg);
    case 128259L: HandleBoatSpeed(N2kMsg);
    case 128267L: HandleDepth(N2kMsg);
  }
}
```

Aus [N2kMessages.h](https://github.com/ttlappalainen/NMEA2000/blob/master/src/N2kMessages.h) Zeile 726 können wir den Aufbau der Funktion und der nötigen Variablen entnehmen:

````
ParseN2kWaterDepth(const tN2kMsg &N2kMsg, unsigned char &SID, double &DepthBelowTransducer, double &Offset, double &Range)
````

Wenn ihr bei der neuen Funktion die Namen der Variablen aus der PGN-Dokumentation übernommen hattet, sollte die neue Funktion HandleDepth() in etwa so aussehen:

```
void HandleDepth(const tN2kMsg &N2kMsg) {
  unsigned char SID;
  double DepthBelowTransducer;
  double Offset;
  double Range;
  double WaterDepth;

  if ( ParseN2kWaterDepth(N2kMsg, SID, DepthBelowTransducer, Offset, Range) ) {
    WaterDepth = DepthBelowTransducer + Offset;
    Serial.printf("PGN128267: Water Depth: %4.1f Meter \n", WaterDepth);
  }
}
```

Um die wahre Wassertiefe zu erhalten, wurde hier zusätzlich eine Variable WaterDepth angelegt. Mit "WaterDepth = DepthBelowTransducer + Offset" werden dann die beiden Werte "Tiefe unter dem Sensor" und dem definierten Offset addiert. Eine Umrechnung ist hier nicht nötig, da die Werte schon in der Einheit Meter vorliegen.

Nach den Hochladen des Programms wird im Seriellen Monitor nun auch die Wassertiefe angezeigt. Das Programm "NMEA-Simulator" muss dazu natürlich auf dem PC auch laufen.

![SerialMonitor](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/SerialMonitor2.png)

Falls es nicht funktioniert hat, könnt ihr das eigene Programm auch mit [NMEA2000-Reader2.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA2000-Reader2/NMEA2000-Reader2.ino) vergleichen.

Das war schon alles zum Thema Lesen vom Bus.

Wir kommen num zu letzen Teil des Workshops, dem Aufbau eines [NMEA2000-WLAN-Gateways](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/WLAN-GW.md).

