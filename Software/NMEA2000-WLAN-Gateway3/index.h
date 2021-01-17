const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>

<meta HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<style>
h1 {
  font-size: 1.5em;
  text-align: center; 
  vertical-align: middle; 
  margin:0 auto;
}

p {
  font-size: 1.5em;
  text-align: center; 
  vertical-align: middle; 
  margin:0 auto;
}

table {
  font-size: 1.5em;
  text-align: left; 
  vertical-align: middle; 
  margin:0 auto;
}

.button {
  font-size: 22px;;
  text-align: center; 
}

.slidecontainer {
  width: 100%;
}

.slider {
  -webkit-appearance: none;
  width: 80%;
  height: 22px;
  background: #d3d3d3;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 18px;
  height: 18px;
  background: #4CAF50;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 18px;
  height: 18px;
  background: #4CAF50;
  cursor: pointer;
}

</style>

<title>Anzeige</title>
<hr>
<h1>Anzeige</h1>
<hr>

</head>

<body style="font-family: verdana,sans-serif" BGCOLOR="#819FF7">

  <table>
    <tr><td style="text-align:right;">COG:</td><td style="color:white;"><span id='cog'></span> °</td></tr>
    <tr><td style="text-align:right;">SOG:</td><td style="color:white;"><span id='sog'></span> kn</td></tr>
    <tr><td style="text-align:right;">LED:</td><td style="color:white;"><span id='state'></span></td></tr>
    <tr><td style="text-align:right;">Dim:</td><td style="color:white;"><span id='level'></span> %</td></tr>
  </table>

  <br>
  
  <div class="slidecontainer">
    <p><input type="range" min="0" max="100" value="50" class="slider" id="myRange"></p>
  </div>

  <hr>
  <p>
  <input type="button" class="button" value="  An  " onclick="button_clicked('on')"> 
  <input type="button" class="button" value="Aus" onclick="button_clicked('off')"> 
  </p>
  <hr>
  
  <script>
 
    requestData(); // get intial data straight away 

    var slider = document.getElementById("myRange");
    var output = document.getElementById("level");
    
    slider.oninput = function() {
      output.innerHTML = this.value;
      var xhr = new XMLHttpRequest();
      xhr.open('GET', 'slider' + '?level=' + this.value, true);
      xhr.send();
    }

    function button_clicked(key) { 
      var xhr = new XMLHttpRequest();
      xhr.open('GET', key, true);
      xhr.send();
      requestData();
    }
  
    // request data updates every 500 milliseconds
    setInterval(requestData, 500);
    
    function requestData() {

      var xhr = new XMLHttpRequest();
      
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {

          if (this.responseText) { // if the returned data is not null, update the values

            var data = JSON.parse(this.responseText);

            document.getElementById("cog").innerText = data.cog;
            document.getElementById("sog").innerText = data.sog;
            document.getElementById("state").innerText = data.state;
            
            output.innerHTML = data.level;
            slider.value = data.level;
          } 
        } 
      }
      xhr.open('GET', 'get_data', true);
      xhr.send();
    }
     
  </script>

</body>

</html>

)=====";
