const char graphsPage[] PROGMEM = (R"=====(
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
    align-items: center;
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

.graphs-panel {
    width: 90%;
    margin-top: 60px;
    padding: 40px;
    border-radius: 30px;
    background: rgba(20,20,20,0.9);
    backdrop-filter: blur(20px);
    box-shadow: 0 30px 80px rgba(0,0,0,0.8);
    color: white;
}

.graphs-panel h2 {
    margin-bottom: 30px;
    font-size: 24px;
    opacity: .85;
}

.graphs-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(320px, 1fr));
    gap: 30px;
}

.graphs-grid iframe {
    width: 100%;
    height: 260px;
    border-radius: 18px;
    border: none;
    background: black;
    box-shadow:
        inset 0 0 0 1px rgba(255,255,255,0.06),
        0 20px 60px rgba(0,0,0,.8);
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
                    <a href="vehicle.html" target="_self">
                        Vehicle
                    </a>

                </div>

                <div class="item">
                    <a href="location.html" target="_self">
                        Location
                    </a>
                </div>

                <div class="item">
                    <a href="health.html" target="_self">
                        Health
                    </a>
                </div>

                <div class="item">
                    <a href="graphs.html" target="_self">
                        Graphs
                    </a>
                </div>

                <div class="item">
                    <a href="main.html" target="_self">
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

            <div class="graphs-panel">
                <h2>Live Sensor Data</h2>

                <div class="graphs-grid">
                    <iframe
                        src="https://thingspeak.mathworks.com/channels/3151130/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=10&type=line"></iframe>
                    <iframe
                        src="https://thingspeak.mathworks.com/channels/3151130/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=10&type=line"></iframe>
                    <iframe
                        src="https://thingspeak.mathworks.com/channels/3151130/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=10&type=line"></iframe>
                    <iframe
                        src="https://thingspeak.mathworks.com/channels/3151130/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=10&type=line"></iframe>
                    <iframe
                        src="https://thingspeak.mathworks.com/channels/3151130/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=10&type=line"></iframe>
                </div>
            </div>


        </div>
    </div>
</body>

</html>
      )=====");