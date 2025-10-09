String homePagePart1 = F(R"=====(<!DOCTYPE html>
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
    background-color:rgb(209, 235, 241) ;
    min-height: 75px;
    line-height: 75px;
    position: sticky;
    top: 0;
    z-index: 1000;  
}

#SSIA{
    display: flex;
    text-align: center;
    font-size: 35px; 
    font-weight: bold;
    font-family: 'Courier New', Courier, monospace;
    position: absolute;
    left: 50%;
    transform: translateX(-50%);
}

#container{
display: flex;
padding-top: 5%;
justify-content: space-around;
}


.item{
  width: 300px;
  height: 300px;
  border-radius: 10px;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content:center;
  font-size: 1.5rem;
  font-weight: bold;
}

.item img{
    position: absolute;
    top: 210px;
}

.item-1{background-color: rgb(209, 235, 241);}
.item-2{background-color: rgb(209, 235, 241);}
.item-3{background-color: rgb(209, 235, 241);}
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
             <p>)=====");
String homePagePart2 = F(R"=====(°C</p>
           </div>
           <div class="item item-2">
            <img src="https://static.vecteezy.com/system/resources/thumbnails/024/984/141/small_2x/3d-weather-forecast-icon-raindrops-air-humidity-percentage-3d-illustration-png.png" alt="humidity-icon" width="100">
            <h2>Humidity</h2>
            <p></p>
            <p> )=====");
String homePagePart3 = F(R"=====( %</p>
           </div>
           <div class="item item-3">
            <img src="https://cdn-icons-png.flaticon.com/512/335/335648.png" alt="ultra-sonic-sensor" width="100"> 
            <h2>Distance</h2>
            <p> )=====");
String homePagePart4 = F(R"=====( Cm</p>
           </div>
        </div>

    </div>
</body>
</html>)=====");