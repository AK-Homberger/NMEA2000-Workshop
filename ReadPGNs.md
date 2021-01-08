# Lesen vom NMEA2000-Bus

Bisher haben wir immer Daten auf den NMEA2000-Bus gesendet. Nun kommen wir zur anderen Richtung. Wir werden jetzt Daten vom NMEA2000-Bus lesen und im Seriellen Monitor anzeigen.

Wir fangen mit einem einfachen Beispiel an und werden es dann erweitern.

Zum Lesen von Daten benötigen wir nur das Basis-Steckbrett mit den beiden ESP32 und den CAN-Bus Transceivern. Es macht aber auch nichts, wenn noch Komponenten vom vorigen Beispiel geteckt sind.

![Basis-Steckbrett](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA2000-Basis_Steckplatine.png)


# Das Programm

Wir laden nun das Beispielprogramm [NMEA2000-Reade.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/NMEA2000-Reader/NMEA2000-Reader.ino) in die Arduino-IDE.

Danach können wir das Programm auf den rechten ESP32 hochladen.

Auf dem PC starten wir das Programm "NMEA-Simulator". Einen eventuell zuvor gestartetes NMEA-Reader-Programm beenden wir, damit die serielle Schnittstelle zur Verfügung steht. Im NMEA-Simulator konfigurieren wir die serielle Snittstelle für NMEA2000. Das geht mit "Tools", "Options", Reiter "NMEA2000". Dann wählen wir die serielle Schnittselle vom linken ESP32. Danach OK klicken.

![Simulator](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/NMEA-Simulator1.png)

Jetzt können wir im Simulator Werte ändern und die Motordrehzahl erhöhen. Dazu "Throttle Gear" hochschieben.

Danach wählen wir die CheckBox "Run" aus.

Im Seriellen Monitor der Arduino-IDE sollen jetzt folgende Zeilen ausgegeben werden:

```
PGN127250: Heading: 272.2 Deviation: 1.0 Variation: 2.0
```
Wenn im NMEA-Simulator andere Werte gesetzt wurden, sieht due Ausgabe natürlich anders aus.

# Programm-Elemente

Kommen wir nun zu den neuen Programm-Elementen.

Wir starten mit der Liste der PGNs. Hier aber nicht TransmitMessages[] sondern ReceiveMessges[]:
Wir definiren gleich eine ganze Reihe von PGNs, auch wenn wir sie jetzt noch nicht benötigen.

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

Die Produkt- und Geräteinformationen wurde angepasst:

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

Wir setzen hier als Klasse "25" als "Inter/Intranetwork Device" und als Geräte-Funktion "131" als "NMEA 2000 to Analog Gateway".
Stimmt zwar nicht ganz, ist aber nah dran, da wir die Werte einfach auf seriell ausgeben um sie im Seriellen Monitor auszugeben.


Die folgenden drei Zeilen sind anders als in den bisherigen Beispielen:
```
  NMEA2000.SetMode(tNMEA2000::N2km_ListenOnly, NodeAddress);
  NMEA2000.ExtendReceiveMessages(ReceiveMessages);
  NMEA2000.SetMsgHandler(MyHandleNMEA2000Msg);
```







