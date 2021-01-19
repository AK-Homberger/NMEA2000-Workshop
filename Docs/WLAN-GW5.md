# Anzeige von Längen- und Breitengrad

Nun werden wir den Web-Server noch etwas erweitern, um auch die Position anzeigen zu können.

Wir brauchen dazu nur wenig zu ändern.

## 1. Hauptprogram

In getData() ergänzen wir einige Zeilen.

Erst zwei neue lokale Variablen:

```
  double minutes;
  double deg;
```
Dann nach "StaticJsonDocument<200> root;" folgendes hinzufügen:

```
  deg = abs(trunc(BoatData.Latitude));
  minutes = abs((BoatData.Latitude - trunc(BoatData.Latitude)) * 60.0);

  if (BoatData.Latitude > 0) {
    snprintf(buf, sizeof(buf), "%02.0f°%06.3f' N", deg, minutes);
  } else {
    snprintf(buf, sizeof(buf), "%02.0f°%06.3f' S", deg, minutes);
  }
  root["lat"] = buf;
```

Wir wollen den Breitengrad in Grad und Minuten anzeigen und nicht als vorzeichenbehaftete Kommazahl in Grad.

Dazu berechnen wir zuerst jeweils den Absolutwert, ohne Vorzeichen von Grad und Minuten (abs). Von Grad wollen wir nur den ganzzahligen Teil (trunc).

Dann unterscheiden wir anhand des Vorzeichens, ob es sich um Nord (>0) oder Süd (<=0) handelt.

Nun bauen wir uns die Ausgabe in den Puffer so zusammen, dass die Formatierung passt. Grad mit zwei Stellen (%02.0) und Minuten mit zwei Vor- und drei Nachkommastellen (%06.3). Jeweils mit fürenden Nullen. Die Zeichen für Grad "°" und Minuten "'" fügen wir auch gleich an den richtigen Stellen ein.

Die Formatierung "%06.3" bedeutet übrigens:
- 0 = Führende Nullen verwenden
- 6 = 6 Stellen insgesammt (der Punkt zählt mit)
- .3 = 3 Nachkommastellen
Also zwei Vorkommastellen und drei Nachkommastellen, wobe fürende nullen verwendet werden.


Das Gleiche wiederholen wir jetzt mit dem Längengrad:
```
  deg = abs(trunc(BoatData.Longitude));
  minutes = abs((BoatData.Longitude - trunc(BoatData.Longitude)) * 60.0);

  if (BoatData.Longitude > 0) {
    snprintf(buf, sizeof(buf), "%03.0f°%06.3f' E", deg, minutes);
  } else {
    snprintf(buf, sizeof(buf), "%03.0f°%06.3f' W", deg, minutes);
  }
  root["lon"] = buf;
```
Hier ist die Formatierung für Grad etwas anders, da Längengradangaben immer dreistellig mit führenden Nullen dargestellt werden. Und statt N/S unterscheiden wir hier zwischen E/W.

## 2. index.h
Nun ergänzen wir den HTML- und Javascript-Code entsprechend.

Erst die Tabelle im HTML-Code. Wir fügen zwei Zeilen für LAT/LON am Anfang der Tabelle hinzu.

```
  <table>
    <tr><td style="text-align:right;">LAT:</td><td style="color:white;"><span id='lat'></span> </td></tr>
    <tr><td style="text-align:right;">LON:</td><td style="color:white;"><span id='lon'></span> </td></tr>
```
    
In der Funktion requestData() fügen wir nach "var data = JSON.parse(this.responseText);" folgende beiden Zeilen hinzu:

```
    var data = JSON.parse(this.responseText);

    document.getElementById("lat").innerText = data.lat;
    document.getElementById("lon").innerText = data.lon;
```

Das ist alles. Nach dem Speicherm, Kompilieren und Hochladen sieht das Bild im Browser so aus.

Eventell WLAN neu verbinden und die Adresse 192.168.4.1 erneut anwählen. Und der NMEA-Simulator muss natürlich auch laufen.

![Web](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Bilder/Web-Server2.PNG)

Im NMEA-Simulator kann man jetzt mit Throttle/Ruder etwas Fahrt aufnehmen. Dann sieht man auch, wie sich die Positionsdaten ändern.

Das komplette Programm ist als [NMEA2000-WLAN-Gateway4.ino](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Software/NMEA2000-WLAN-Gateway4/NMEA2000-WLAN-Gateway4.ino) bei den Beispielen gespeichert.


Das war alles zum Thema Web-Server und AJAX. Es ist recht einfach, die Funktionen auf Server- und auf Cleint-Seite nun für weitere Daten zu erweitern.

[Das wars...](https://github.com/AK-Homberger/NMEA2000-Workshop/blob/main/Docs/Ende.md).

