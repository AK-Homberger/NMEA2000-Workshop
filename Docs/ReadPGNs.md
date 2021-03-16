# Lesen vom NMEA2000-Bus

Bisher haben wir immer Daten auf den NMEA2000-Bus gesendet. Nun kommen wir zur anderen Richtung. Wir werden jetzt Daten vom NMEA2000-Bus lesen und im Seriellen Monitor anzeigen.

Wir fangen mit einem einfachen Beispiel an und werden es dann erweitern.

Zum Lesen von Daten benötigen wir nur das Basis-Steckbrett mit den beiden ESP32 und den CAN-Bus-Transceivern. Es macht aber auch nichts, wenn noch Komponenten vom vorigen Beispiel gesteckt sind.

![Basis-Steckbrett](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-Basis_Steckplatine.png)


## Das Programm

Wir laden nun das Beispielprogramm [NMEA2000-Reader.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA2000-Reader/NMEA2000-Reader.ino) in die Arduino-IDE. Danach können wir das Programm auf den **rechten** ESP32 hochladen.

Auf dem PC starten wir das Programm "NMEA-Simulator". Ein eventuell zuvor gestartetes NMEA-Reader-Programm beenden wir, damit die serielle Schnittstelle zur Verfügung steht.

Im NMEA-Simulator konfigurieren wir die serielle Schnittstelle für NMEA2000. Das geht mit "Tools", "Options", Reiter "NMEA2000". Dann wählen wir die serielle Schnittstelle vom **linken** ESP32 (Baudrate 115200).
 
![Settings](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA-Simulator-Settings.png)
 
Im unteren Bereich "NMEA2000 PGNs to be set" müssen wir noch auswählen, welche PGN gesendet werden sollen. Wählt für "Navigation" und "Wind, temp" einfach alle PGN aus.

Danach OK klicken.

![Simulator](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA-Simulator1.png)

Jetzt können wir im Simulator Werte ändern und die Motordrehzahl erhöhen. Dazu "Throttle Gear" hochschieben.

**Danach wählen wir die Check-Box "Run" aus.**

Im Seriellen Monitor der Arduino-IDE sollen jetzt folgende Zeilen ausgegeben werden:

![Serial Monitor](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/SerialMonitor1.png)

Wenn im NMEA-Simulator andere Werte gesetzt wurden, sieht die Ausgabe natürlich auch anders aus.

## Programm-Elemente

Kommen wir nun zu den neuen Programm-Elementen.

Wir starten mit der Liste der PGN. Hier aber nicht mit TransmitMessages[], sondern ReceiveMessges[].
Wir definieren gleich eine ganze Reihe von PGN, auch wenn wir sie jetzt noch nicht benötigen.

```
const unsigned long ReceiveMessages[] PROGMEM = {/*126992L,*/ // System time
      127250L, // Heading
      127258L, // Magnetic variation
      128259L, // Boat speed
      128267L, // Depth
      129025L, // Position
      129026L, // COG and SOG
      129029L, // GNSS
      130306L, // Wind
      128275L, // Log
      127245L, // Rudder
      0
    };
    
```

Die Produkt- und Geräteinformationen wurden angepasst:

```
// Set product information
  NMEA2000.SetProductInformation("1", // Manufacturer's Model serial code
                                 100, // Manufacturer's product code
                                 "NMEA Reader",  // Manufacturer's Model ID
                                 "1.0.2.25 (2019-07-07)",  // Manufacturer's Software version code
                                 "1.0.2.0 (2019-07-07)" // Manufacturer's Model version
                                );
  // Set device information
  NMEA2000.SetDeviceInformation(id,  // Unique number. Use e.g. Serial number.
                                131, // Device function=NMEA 2000 to Analog Gateway. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                25,  // Device class=Inter/Intranetwork Device. See codes on  http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                               );
```

Wir setzen hier die Klasse "25" als "Inter/Intranetwork Device" und die Geräte-Funktion "131" als "NMEA2000 to Analog Gateway".
Das stimmt zwar nicht ganz, ist aber nah dran, da wir die Werte einfach seriell ausgeben, um sie im Seriellen Monitor anzusehen.

Die folgenden drei Zeilen sind anders als in den bisherigen Beispielen:
```
  NMEA2000.SetMode(tNMEA2000::N2km_ListenOnly, NodeAddress);
  NMEA2000.ExtendReceiveMessages(ReceiveMessages);
  NMEA2000.SetMsgHandler(MyHandleNMEA2000Msg);
```
Als Modus setzen wir hier "N2km_ListenOnly". Das ist die minimale Funktion der NMEA2000-Bibliothek. Die Daten werden vom Bus gelesen und an den Message-Stream weitergeleitet.

Mit dem nächsten Kommando definieren wir hier eine Liste mit PGN, die wir empfangen möchten. Bisher haben wir immer die zu sendenden PGN definiert.

Mit "NMEA2000.SetMsgHandler(MyHandleNMEA2000Msg);" setzen wir die Funktion zur Behandlung der empfangenen PGN.

Diese Funktion wird auch gleich nach setup() definiert:

```
void MyHandleNMEA2000Msg(const tN2kMsg &N2kMsg) {

  switch (N2kMsg.PGN) {
    case 127250L: HandleHeading(N2kMsg);
    case 128259L: HandleBoatSpeed(N2kMsg);
  }
}
```
Die Funktion MyHandleNMEA2000Msg() wird von der Bibliothek immer dann aufgerufen, wenn ein neuer PGN vom Bus empfangen wurde.
Mit switch(N2kMsg.PGN) können wir nun prüfen, um welches PGN es sich handelt. Die Nummer ist in der Variablen N2kMsg.PGN gespeichert.
Mit "case PGN-Nummer:" können wir nun nacheinander die Nummern vergleichen, und wenn die Nummer stimmt, eine Funktion aufrufen, die sich um die weitere Verarbeitung kümmert.

Die Funktionen zur Behandlung werden nun definiert.

## Erst HandleHeading():

```
void HandleHeading(const tN2kMsg &N2kMsg) {
  unsigned char SID;
  tN2kHeadingReference ref;
  double Deviation = 0;
  double Variation;
  double Heading;

  if ( ParseN2kHeading(N2kMsg, SID, Heading, Deviation, Variation, ref) ) {
  Serial.printf("PGN127250: Heading: %4.1f Deviation: %3.1f Variation: %3.1f\n", RadToDeg(Heading), RadToDeg(Deviation), RadToDeg(Variation));
  }
}
```

Der Aufbau von Funktionen zur Behandlung der PGN ist immer ähnlich:

Erst lokale Variablen definieren. Welche Variablen nötig sind, richtet sich nach den PGN, die in [N2kMessges.h](https://github.com/ttlappalainen/NMEA2000/blob/master/src/N2kMessages.h) definiert sind. Anders als beim Senden interessieren uns jetzt die Funktionen, die mit **"ParseN2k..."** beginnen.

[ParseN2kHeading()](https://github.com/ttlappalainen/NMEA2000/blob/db22adbb3fec182ecae2ae8bc816378ac43d7fc3/src/N2kMessages.h#L116) benötigen wir die Sequence-ID "SID", die Heading Reference "ref", die Variablen für Deviation, Variation und natürlich für Heading selbst.

Nach dem Aufruf von ParseN2kHeading() enthalten die Variablen die Werte aus dem empfangenen PGN und können genutzt werden.

Dann geben wir die Daten für Heading, Deviation und Variation einfach mit Serial.printf() auf der seriellen Schnittstelle aus, damit sie im Seriellen Monitor der Arduino-IDE angezeigt werden.

Da Daten in NMEA2000-PGN immer als SI-Einheiten gepeichert sind, müssen wir die Winkel zur Anzeige in Grad mit RadtoDeg() umwandeln.

## Und nun HandleBoatSpeed():

```
void HandleBoatSpeed(const tN2kMsg &N2kMsg) {
  unsigned char SID;
  double WaterReferenced;
  double GroundReferenced;
  tN2kSpeedWaterReferenceType SWRT;

  if ( ParseN2kBoatSpeed(N2kMsg, SID, WaterReferenced, GroundReferenced, SWRT) ) {
    Serial.printf("PGN128259: STW: %3.1f kn SOG: %3.1f kn\n", msToKnots(WaterReferenced), msToKnots(GroundReferenced));
  }
}
```

Der Aufbau ist ganz ähnlich. Wir zeigen hier Speed Through Water STW (WaterReferenced) und Course Over Ground COG (GroundReferenced) an. Natürlich müssen wir auch hier zur Anzeige von Knoten mit msToKnots() umrechnen.

Das war schon alles zum Lesen von Daten vom NMEA2000-Bus.

# Workshop-Aufgabe
Als kleine Workshop-Aufgabe erweitert bitte selbständig das Programm für die Wassertiefe. 
- Funktionsname soll "HandleDepth(N2kMsg)" sein.
- Es soll die wahre Wassertiefe angezeigt werden.

Kleiner Tip: Schaut in [N2kMessges.h](https://github.com/ttlappalainen/NMEA2000/blob/master/src/N2kMessages.h) mal nach Zeile 708.

Hier geht es zur [Auflösung](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/ReadPGNs2.md) der Aufgabe.

