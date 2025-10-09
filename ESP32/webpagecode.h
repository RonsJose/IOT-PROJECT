const char homePage[] PROGMEM = (R"=====(
    <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>IoT</title>
    <link rel="icon" href="https://cdn-icons-png.flaticon.com/512/10355/10355707.png">
    <style> 
    *{
    margin: 0px;
    padding: 0px;
    font-family: Arial, Helvetica, sans-serif;
    box-sizing: border-box;
}

#wrapper{
    width: 100%;
    margin: auto;
}

.clearfix::after{
    clear:both;
    display: block;
    content:"";
}

#Nav{ 
    background-color: rgb(209, 235, 241);
    min-height: 75px;
    line-height: 75px;
    display: flex;        
    align-items: center;    
    justify-content: center;
    position: sticky;
    top:0;
    z-index: 1000;
}

#SSIA{
    font-size: 35px;
    font-weight: bold;
    font-family: 'Courier New', Courier, monospace;
}

@media (max-width: 600px) {
    #SSIA {
        font-size: 20px;             
        padding: 0 5px;              
        line-height: 1.2;            
    }
}
#container{
display: flex;
padding-top: 5%;
justify-content: space-around;
flex-wrap: wrap;
}


.item{
  position: relative;  
  width: 300px;
  height: 300px;
  border-radius: 10px;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content:center;
  font-size: 1.5rem;
  font-weight: bold;
  margin: 10px;
  background-color: rgb(209, 235, 241);
}

.item img{
    position: absolute;
    top: 20px;
}
    </style>
</head>
<body>
    <div id="wrapper">
        <div id="Nav" class="clearfix">
            <nav>
                <div id="SSIA">Smart System Integration for Automobiles</div>
            </nav>
        </div>

        <div id="container">
           <div class="item item-1">
             <img src="https://uxwing.com/wp-content/themes/uxwing/download/medical-science-lab/high-temperature-icon.png" alt="temperature-icon" width="100">
             <h2>Temperature</h2>
             <p id="temperature">%TEMPERATURE% </p>
           </div>
           <div class="item item-2">
            <img src="https://static.vecteezy.com/system/resources/thumbnails/024/984/141/small_2x/3d-weather-forecast-icon-raindrops-air-humidity-percentage-3d-illustration-png.png" alt="humidity-icon" width="100">
            <h2>Humidity</h2>
            <p id="humidity">%HUMIDITY% </p>
           </div>
           <div class="item item-3">
            <img src="https://cdn-icons-png.flaticon.com/512/335/335648.png" alt="Distance Img" width="100"> 
            <h2>Distance</h2>
            <p id="distance">%DISTANCE% </p>
           </div>
           <div class="item item-4">
            <img src="https://cdn.freebiesupply.com/logos/large/2x/heart-logo-png-transparent.png" alt="Heart IMG" width="100"> 
            <h2>Heart Rate</h2>
            <p id="heartRate">%HEARTRATE% </p>
           </div>
            <div class="item item-5">
            <img src="https://m.media-amazon.com/images/S/abs-image-upload-na/0/AmazonStores/A21TJRUUN4KGV/bba883408bb96efa3e99895f04d52fac.w300.h300.png" alt="Blood Oxygen IMG" width="100"> 
            <h2>SpO2</h2>
            <p id="spO2">%SPO2% </p>
           </div>
        </div>

    </div>
</body>

<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 5000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 5000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("distance").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/distance", true);
  xhttp.send();
}, 5000 ) ;

  setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("heartRate").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/heartRate", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("spO2").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/spO2", true);
  xhttp.send();
}, 1000 ) ;

</script>

</html>
)=====");
