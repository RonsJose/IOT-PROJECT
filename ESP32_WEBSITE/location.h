const char locationPage[] PROGMEM = (R"=====(
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

#logo,
.item,
.nav-right {
    flex: 1;
    display: flex;
    align-items: center;
}

#logo {
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

#container {
    width: 70%;
    margin: 10px auto;
    padding: 40px;
    border-radius: 24px;
    background: rgba(20, 20, 20, 0.9);
    backdrop-filter: blur(20px);
    box-shadow: 0 30px 80px rgba(0, 0, 0, 0.8);
    color: white;
}

#container h2 {
    font-size: 22px;
    margin-bottom: 30px;
    opacity: 0.85;
}

#map {
    height: 550px;
    width: 99%;
}

#Address {
    width: 30%;
    margin: 10px auto;
    padding: 40px;
    border-radius: 24px;
    background: rgba(20, 20, 20, 0.9);
    backdrop-filter: blur(20px);
    box-shadow: 0 30px 80px rgba(0, 0, 0, 0.8);
    color: white;
}

#Address h2 {
    font-size: 22px;
    margin-bottom: 30px;
    opacity: 0.85;
    text-align: center;
}

@media (max-width: 900px) {
    #Menu {
        width: 95%;
        padding: 12px 16px;
        gap: 10px;
        flex-wrap: wrap;
        min-height: auto;
    }

    #logo {
        font-size: 18px;
    }

    #Menu img {
        width: 36px;
    }

    .item a {
        font-size: 14px;
    }

    .item {
        justify-content: center;
    }

    .container,
    #container,
    #Address {
        width: 95%;
        padding: 20px;
    }

    #map {
        height: 300px;
    }

    .container-grid {
        grid-template-columns: 1fr;
        gap: 20px;
    }

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
                <h2>Maps</h2>

                <div id="map"></div>

                <!-- prettier-ignore --> <!-- googles new modular loader -->
                <script>(g => { var h, a, k, p = "The Google Maps JavaScript API", c = "google", l = "importLibrary", q = "__ib__", m = document, b = window; b = b[c] || (b[c] = {}); var d = b.maps || (b.maps = {}), r = new Set, e = new URLSearchParams, u = () => h || (h = new Promise(async (f, n) => { await (a = m.createElement("script")); e.set("libraries", [...r] + ""); for (k in g) e.set(k.replace(/[A-Z]/g, t => "_" + t[0].toLowerCase()), g[k]); e.set("callback", c + ".maps." + q); a.src = `https://maps.${c}apis.com/maps/api/js?` + e; d[q] = f; a.onerror = () => h = n(Error(p + " could not load.")); a.nonce = m.querySelector("script[nonce]")?.nonce || ""; m.head.append(a) })); d[l] ? console.warn(p + " only loads once. Ignoring:", g) : d[l] = (f, ...n) => r.add(f) && u().then(() => d[l](f, ...n)) })
                        ({ key: "", v: "weekly" });</script>
            </div>

            <div id="Address">
                <h2>Street Address</h2>
                <h3 id="address">%ADDRESS%</h3>
            </div>

        </div>
    </div>
</body>

<script>
  setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("address").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/address", true);
    xhttp.send();
  }, 1000);


  setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        window.currentLat = parseFloat(this.responseText);
      }
    };
    xhttp.open("GET", "/latitude", true);
    xhttp.send();
  }, 3000);

  setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        window.currentLng = parseFloat(this.responseText);
      }
    };
    xhttp.open("GET", "/longitude", true);
    xhttp.send();
  }, 3000);

     //Updates the map using the gps sensor
  let map;
  let marker;
  let currentLat = 53.277556;
  let currentLng = 9.009750;

  async function initMap() { //Loads the libraries
    const { Map } = await google.maps.importLibrary("maps");
    const { AdvancedMarkerElement } = await google.maps.importLibrary("marker");

    map = new Map(document.getElementById("map"), { //Creates the map 
      center: { lat: currentLat, lng: currentLng },
      zoom: 15,
      mapId: "9040f5107606389e1876cc8e",
    });

    marker = new AdvancedMarkerElement({
      position: { lat: currentLat, lng: currentLng },
      map: map,
      title: "Current location",
    });
  }

  initMap();

  //Updates marker position 
  setInterval(() => {
    if (!marker || !window.currentLat || !window.currentLng) return;

    marker.position = { lat: window.currentLat, lng: window.currentLng };
    map.setCenter({ lat: window.currentLat, lng: window.currentLng });
  }, 3000);
</script>
</html>
      )=====");