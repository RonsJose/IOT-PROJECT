const char vehiclePage[] PROGMEM = (R"=====(
  <!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>SSIA</title>
    <style>
    * {
    margin: 0px;
    padding: 0px;
    font-family: Arial, Helvetica, sans-serif;
    box-sizing: border-box;
}

#wrapper {
    width: 100%;
    margin: auto;
}

.clearfix::after {
    clear: both;
    display: block;
    content: "";
}

#Background {
    min-height: 100vh;
    width: 100%;
    background-color: black;
    display: flex;
    justify-content: center;
    align-items: flex-start;
    padding-top: 120px; 
    flex-direction: column;
}


#Menu img {
    width: 50px;
}

#Menu {
    position: absolute;
    top: 2%;
    left: 50%;
    transform: translateX(-50%);
    width: 50%;
    min-height: 80px;

    display: flex;
    align-items: center;

    background: rgba(15, 15, 15, 0.85);
    backdrop-filter: blur(20px);
    border-radius: 16px;
    padding: 0 32px;
    gap: 20px;
}


#logo {
    display: flex;
    align-items: center;
    gap: 12px;
    color: white;
    font-size: 22px;
}

#Menu a {
    text-decoration: none;
    color: gray;
    margin-left: 10px;
    font-size: 18px;
}


#Menu a:hover {
    color: white;
    font-size: 19px;
}

#logo, .item, .nav-right {
    flex: 1;
    display: flex;
    align-items: center;
}

#logo{
    gap: 12px;
    color: white;
    font-size: 22px;
}


.item {
    justify-content: center;
    gap: 28px;
}


.nav-right {
    justify-content: flex-end;
}

.container {
  width: 70%;
  margin: 10px auto;
  padding: 40px;
  border-radius: 24px;
  background: rgba(20,20,20,0.9);
  backdrop-filter: blur(20px);
  box-shadow: 0 30px 80px rgba(0,0,0,0.8);
  color: white;
}

.container h2 {
  font-size: 22px;
  margin-bottom: 30px;
  opacity: 0.85;
}


.container-grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 30px;
}


.container-tile {
  background: rgba(255,255,255,0.03);
  border-radius: 20px;
  padding: 30px;
  box-shadow: inset 0 0 0 1px rgba(255,255,255,0.06);
  transition: transform .3s, box-shadow .3s;
  text-align: center;
}

.container-tile:hover {
  transform: translateY(-8px);
  box-shadow: 0 20px 60px rgba(0,0,0,0.8);
}

.container-tile .icon {
  font-size: 32px;
  opacity: .8;
}

.container-tile h3 {
  font-size: 48px;
  margin: 15px 0 8px;
  font-weight: 600;
}

.container-tile h3 span {
  font-size: 20px;
  opacity: .6;
}

.container-tile p {
  letter-spacing: .1em;
  font-size: 12px;
  text-transform: uppercase;
  opacity: .6;
}

.temp { box-shadow: inset 0 0 40px rgba(255,80,80,.08); }
.humidity { box-shadow: inset 0 0 40px rgba(80,160,255,.08); }
.distance { box-shadow: inset 0 0 40px rgba(255,200,80,.08); }

.Camera {
  width: 320px;
  height: 320px;
  margin: 60px auto 0;
  border-radius: 50%;
  position: relative;

  background: rgba(255,255,255,0.03);
  backdrop-filter: blur(20px);
  box-shadow:
      0 40px 80px rgba(0,0,0,0.8),
      inset 0 0 40px rgba(255,255,255,0.08);
  
  overflow: hidden;
}


.Camera img {
  width: 100%;
  height: 100%;
  object-fit: cover;
  border-radius: 50%;
  opacity: 0.9;
}


.Camera::before {
  content: "";
  position: absolute;
  inset: 0;
  border-radius: 50%;
  box-shadow: 0 0 40px rgba(80,160,255,.3);
  pointer-events: none;
}

    </style>
</head>

<body>
    <div id="wrapper">
        <div id="Background">

            <div id="Menu">

                <div id="logo">
                    <img src="https://www.nicepng.com/png/full/89-891434_white-car-icon-png-car-icon-green-png.png">
                    <p>SSIA</p>
                </div>

                <div class="item">
                    <a href="/vehicle" target="_self">
                        Vehicle
                    </a>

                </div>

                <div class="item">
                    <a href="/location" target="_self">
                        Location
                    </a>
                </div>

                <div class="item">
                    <a href="/health" target="_self">
                        Health
                    </a>
                </div>

                <div class="item">
                    <a href="/graphs" target="_self">
                        Graphs
                    </a>
                </div>

                <div class="item">
                    <a href="/" target="_self">
                        Home
                    </a>
                </div>

                <div class="nav-right"></div>
            </div>

            <div id="container">
                <div class="object object-1">

                </div>
                <div class="object object-2">

                </div>
                <div class="object object-3">

                </div>
            </div>

            <div class="container">
                <h2>Environment</h2>

                <div class="container-grid">
                    <div class="container-tile temp">
                        <span class="icon">🌡</span>
                        <h3 id="temperature">%TEMPERATURE%<span>°C</span></h3>
                        <p>Temperature</p>
                    </div>

                    <div class="container-tile humidity">
                        <span class="icon">💧</span>
                        <h3 id="humidity">%HUMIDITY%<span>%</span></h3>
                        <p>Humidity</p>
                    </div>

                    <div class="container-tile distance">
                        <span class="icon">📏</span>
                        <h3 id="distance">%DISTANCE%<span>cm</span></h3>
                        <p>Distance</p>
                    </div>
                </div>
            </div>

            <div class="Camera">
                <img id="camera" src="%IP%" alt="Camera feed">
            </div>

        </div>
    </div>
</body>

<script>
//AJAX requests to update data without refreshing the page
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {//When it changes
    if (this.readyState == 4 && this.status == 200) { //If it was successful
      document.getElementById("temperature").innerHTML = this.responseText+ "<span>°C</span>";
    }
  };
  xhttp.open("GET", "/temperature", true); //prepares the GET request
  xhttp.send(); //Sends the request
}, 5000 ) ;//Time to wait before it goes again

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText+ "<span>%</span>";
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 5000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("distance").innerHTML = this.responseText +"<span>cm</span>";
    }
  };
  xhttp.open("GET", "/distance", true);
  xhttp.send();
}, 5000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("camera").src = "http://" + this.responseText + ":81/stream";
    }
  };
  xhttp.open("GET", "/camera", true);
  xhttp.send();
}, 1000 ) ;
</script>

</html>
      )=====");