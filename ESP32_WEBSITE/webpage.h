const char homePage[] PROGMEM = (R"=====(
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
    background:
        linear-gradient(rgba(0,0,0,0.6), rgba(0,0,0,0.6)),
        url("https://media3.giphy.com/media/v1.Y2lkPTc5MGI3NjExeHp4bzN3bmRtbWo3OWpvd2ZwMXRkY3JyOWlwc3hwZHh4c3JreTEyZCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/xT4uQF7h39mlsF5czK/giphy.gif");
    background-size: cover;
    background-position: center;
    display: flex;
    justify-content: center;
    align-items: center;
}

#Nav {
    display: flex;
    position: absolute;

    top: 3%;
    left: 1%;
    align-items: center;
    justify-content: center;
}

#Nav p {
    color: white;
    position: relative;
    left: 10%;
    font-size: 30px;
    font-style: italic;
}

#Nav img {
    width: 50px;
}

#Center {
    display: flex;
    flex-direction: column;
    position: absolute;
    justify-content: center;
    align-items: center;
    top: 40%;
    left: 50%;
    color: white;
    transform: translateX(-50%);
    text-align: center;

    max-width: 750px;
    width: 100%;
}

#Center h1 {
    font-size: 80px;
}

#Center p {
    font-family: 'Trebuchet MS', 'Lucida Sans Unicode', 'Lucida Grande', 'Lucida Sans', Arial, sans-serif;
    margin-top: 25px;
    font-size: 25px;
}

#buttons {
    display: flex;
    gap: 100px;              
    justify-content: center;
    align-items: center;

    position: absolute;
    top: 110%;
    left: 50%;
    transform: translateX(-50%);
}


.button{
    margin-top: 50px;
    padding: 15px 30px; 
    color: whitesmoke;
    background-color: rgba(22, 51, 97);
    border: none; 
    font-size: 18px;  
    min-width: 200px;
    border-radius: 16px;
}

.button:hover{
    cursor: pointer;
    transform: translateY(-8px);
}
    </style>
</head>

<body>
    <div id="wrapper">
        <div id="Background">

            <div id="Nav">
                <img src="https://www.nicepng.com/png/full/89-891434_white-car-icon-png-car-icon-green-png.png">
                <p>SSIA</p>
            </div>

            <div id="Center">
                <h1>Your Vehicle, Smarter Than Ever</h1>
                <p>A unified system of vehicle intelligence, safety, and real-time monitoring. Fast, reliable, and
                    always connected.</p>

                <div id="buttons">
                    <a href="vehicle.html" target="_self">
                        <button class="button">Vehicle Condition</button>
                    </a>

                    <a href="location.html" target="_self">
                        <button class="button">Location</button>
                    </a>

                    <a href="health.html" target="_self">
                        <button class="button">Health</button>
                    </a>

                    <a href="graphs.html" target="_self">
                        <button class="button">Graphs</button>
                    </a>
                </div>
            </div>

        </div>
    </div>
</body>

</html>
  )=====");