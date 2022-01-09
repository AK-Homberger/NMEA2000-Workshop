# NMEA0183 nach NMEA2000 wandeln
Manchmal möchte man auch NMEA0183-Nachrichten zu NMEA2000 wandeln. Ach dies geht sehr einfach mit den Bibliotheken von Timo Lappalainen.
Als Beispiel werden wir nun Wind-Daten, die als MWV-Datensatz vorliegen, nach NMEA2000 wandeln. Prinzipiell kann man nach diesem Schama alle NMEA0183-Datensätze zu NMEA2000 umsetzen. Weitere [Beispiele](https://github.com/ttlappalainen/NMEA0183/tree/master/Examples) sind auch in der Bibliotehk enthalten.

Im [Programm](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA0183WindToN2k/NMEA0183WindToN2k.ino) benötigen wir als erstes zwei zusätzliche Include-Dateien:

```
#include <NMEA0183.h>
#include <NMEA0183Messages.h>
```

Zusätzlich benötigen wir noch eine NMEA-Objekt zur Behandlung der Stream-Daten von der Seriellen Schnittstelle:
```
// Global objects/variables
tNMEA0183 NMEA0183;         // NMEA stream for NMEA0183 receiving
```

In setup() initialisieren wir das Stream-Objekt mit:

```
NMEA0183.Begin(&Serial2, 3, 4800); // Start NMEA0183 stream handling with 4800 baud
```
Die bedeutet, dass wir für das Objekt die 2. serielle Schnittstelle mit der Baud-Rate 4800 angeben.

Die Geräte-Informationen für NMEA2000 setzen wir auf 135 und 25, passend für ein NMEA0183 zu NMEA2000 Gateway:
```
// Det device information
  NMEA2000.SetDeviceInformation(id,  // Unique number. Use e.g. Serial number.
                                135, // Device function=NMEA 0183 Gateway. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20%26%20function%20codes%20v%202.00.pdf
                                25,  // Device class=Inter/Intranetwork Device. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20%26%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                               );
```

Das war es schon mit der Vorbereitung. Die anderen Elemente für NMEA2000 hatten wir bereits in anderen Beispielen.

Die Erkennung der NMEA0183-Nachritentypen erfolgt in der Funktion NMEA0183_ParseMessges():

```
//*****************************************************************************
void NMEA0183_ParseMessages() {
  tNMEA0183Msg NMEA0183Msg;  // Create message container
  
  if (!NMEA0183.GetMessage(NMEA0183Msg)) return;  // New message ?

  // We do have a new message
  if (NMEA0183Msg.IsMessageCode("MWV")) HandleMWV(NMEA0183Msg);  // Check and handle message
}
```
Als erstes wir ein Nachrichtencontainer für eine NMEA0183-Nachricht erstellt. Dann prüfen wir, ob eine gültige NMEA0183-Nachricht von der seriellen Schnittstelle empfangen wurde.
Falls nicht, beenden wir die Funktion.

Falls eine Nachricht empfangen wurde, testen wir nun, um welche Nachricht es sich handelt. Für das Beispiel interessieren wir uns nur für die "MWV"-Nachricht mit Wind-Informationen. Wenn MWV empfangen wurde, rufen wie die entsprechende Behandlunsroutine auf. Nach diesem Verfahren kann man beliebege NMEA0183-Nachrichten behandeln.

Die eigentliche Umwandlung von NMEA0183 zu NMEA2000 erfolgt in der jeweiligen Behandlungsroutine. Hier exeplarisch für MWV:

```
//*****************************************************************************
void HandleMWV(const tNMEA0183Msg &NMEA0183Msg) {
  double WindAngle;
  tNMEA0183WindReference Reference;
  tN2kWindReference WindReference;
  double WindSpeed;
  tN2kMsg N2kMsg;
  
  //Serial.println("MWV Message");
  
  // Parse MWV message (WindSpeed is in m/s !!!)
  NMEA0183ParseMWV_nc(NMEA0183Msg, WindAngle, Reference, WindSpeed);

  // Read/Set wind reference
  if(Reference == NMEA0183Wind_True) {
    WindReference =  N2kWind_True_boat;
  } else {
    WindReference =  N2kWind_Apparent;
  }

  //Serial.printf("Angle=%4.1f°, Speeed=%3.1f kn, Reference=%d\n", WindAngle, msToKnots(WindSpeed), Reference);
  
  // Create NMEA2000 message
  SetN2kWindSpeed(N2kMsg, 0, WindSpeed, DegToRad(WindAngle), WindReference);
  // Send message
  NMEA2000.SendMsg(N2kMsg);
}
```

Als erstes werden benötigte Variablen deklariert. Welche Variablen benötigt werden, hängt von den unterschiedlichen NMEA0183-PareseXXX-Funktionen ab. Die Funktionen sind in der Header-Datei [NMEA0183Messges.h](https://github.com/ttlappalainen/NMEA0183/blob/master/NMEA0183Messages.h) der Bibliothek von Timo Lappalainen festgelegt.

Für die Wind-Informationen im MWV-Typ rufen wir die entsprechende Funktion auf:

```
// Parse MWV message (WindSpeed is in m/s !!!)
  NMEA0183ParseMWV_nc(NMEA0183Msg, WindAngle, Reference, WindSpeed);
```

Als Daten übergeben wir die NMEA0183-Nachricht und Referenzen auf die benötigten Variablen. Die Variablen haben nach Rückkehr die entsprechenden Werte.
Je nach Daten sind eventuell Umsetzungen von NME0183 auf NMA2000 notwendig. Wie hier zum Beispiel für die Referenzen auf die Wind-Typen.

Als letztes wird die NMEA2000-Nachricht zusammengabaut (hier mit SetN2kWindSpeed) und gesendet.

Die Funktion loop() ist für diese Beispiel recht kurz:

```
void loop() {
  NMEA0183_ParseMessages(); 
  NMEA2000.ParseMessages();
  CheckSourceAddressChange();
}
```

Das war es schon mit der Wandlung von NMEA0183 zu NMEA2000. 
Leider können wir das Programm mangels NMEA0183-Daten nicht testen.

Wir kommen nun zum letzten Teil des Workshops, dem Aufbau eines [NMEA2000-WLAN-Gateways](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/WLAN-GW.md).





