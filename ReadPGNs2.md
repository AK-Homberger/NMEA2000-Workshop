# Aulösung: Wassertiefe

Hier kommt die Auflösung der Workshop Aufgabe.

Als erstes müssen wir die Funktion MyHandleNMEA2000Msg() um PGN128267 und die auzurufene Funktion HandleDepth() erweitern:

```
void MyHandleNMEA2000Msg(const tN2kMsg &N2kMsg) {

  switch (N2kMsg.PGN) {
    case 127250L: HandleHeading(N2kMsg);
    case 128259L: HandleBoatSpeed(N2kMsg);
    case 128267L: HandleDepth(N2kMsg);
  }
}
```

Aus [N2kMessages.h](https://github.com/ttlappalainen/NMEA2000/blob/master/src/N2kMessages.h) Zeile 720 können wir den Aufbau der Funktion und der nötigen Variablen entnehmen:

````
ParseN2kWaterDepth(const tN2kMsg &N2kMsg, unsigned char &SID, double &DepthBelowTransducer, double &Offset, double &Range)
````

Wenn ihr bei der neuen Funktion die Namen der Variablen genauso übernommen hattet, sollte die neue Funktion HandleDepth() in etwa so aussehen:

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

Um die wahre Wassertife zu erhalten, wurde zusätzlich eine Variable WaterDepth angelegt. Mit "WaterDepth = DepthBelowTransducer + Offset" werden dann die biden Werte von Tiefe unter dem Sensor und dem definierten Offset addiert. Eine Umrechnung ist hier nicht nötig, da die Werte schon in der Einheit Meter vorliegen.

Nach den Hochladen des Programms wird im Seriellen Monitor nun auch die Wassertiefe angezeigt.

Das war schon alles zum Thema Lesen vom Bus.

Wir kommen num zu letten Teil der Workshops, dem Aufbau eine WLAN-Gateways.

