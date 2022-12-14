char pageHeader[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Telemetry</title>
<style>
body {
background-image: linear-gradient(120deg, #84fab0 0%, #8fd3f4 100%);
}
ul {
list-style-type: none;
margin: 0;
padding: 0;
overflow: hidden;
background-color: black;
}
li {
float: left;
}
li a {
display: block;
color: white;
text-align: center;
padding: 8px;
text-decoration: none;
}
li a:hover {
color: black;
background-color: white;
}
h1 {
text-align: center;
color:rgb(30, 32, 2);
}
span {
float: right;
border: solid 1px gray;
border-style: inset;
padding: 0 5px 0 5px;
margin: 1px 10px 1px 10px;
background-color: white;
}
fieldset {
display:inline;
text-align: left;
}
div {
padding: 10px 5px 10px 5px;
}
form {
text-align: center;
}
footer {
text-align: center;
font-size: smaller; 
}
</style>
</head>
<body>
<ul>
<li><a href="Home.html">Home</a></li>
<li><a href="IP.html">IP</a></li>
<li><a href="Perf.html">Performance</a></li>
<li><a href="BMP.html">BMP280</a></li>
<li><a href="IMU.html">MPU9250</a></li>
<li><a href="Var.html">Var</a></li>
</ul>
)=====";

/*
**************************************************************************************************
  Body1 main body 
**************************************************************************************************
*/
char bodyHome[] = R"=====(
<h1>Lolin32</h1>
)=====";

/*
**************************************************************************************************
  BodyIP IP parameters, function getipv
**************************************************************************************************
*/
char bodyIP[]= R"=====(
<h1>IP Parameters</h1>
<form action="">
<fieldset>
<legend>parameters</legend>
<div><label for="ip">IP Address</label><span id="ip">_</span></div>
<div><label for="ssid">SSID</label><span id="ssid">_</span></div>
<div><label for="home">Home Page</label><span id="home">http://megaWifi/mega.html</span></div>
</fieldset>
</form> 
<script>
function getipv() {
var reqxml
var resp
var obj

reqxml = new XMLHttpRequest()
reqxml.onreadystatechange = function () {
if (this.readyState === 4 && this.status === 200) {
resp = reqxml.responseText
console.log(resp)
obj = JSON.parse(resp)
document.getElementById('ip').innerHTML = obj.ip
document.getElementById('ssid').innerHTML = obj.ssid
}
}
var urlXML = '/getipv'
console.log(urlXML)
reqxml.open('GET', urlXML, true)
reqxml.send()
}

window.onload = getipv();
setInterval(getipv, 10000);
</script>
)=====";

/*
**************************************************************************************************
  BodyPerf Performance, function getprf
**************************************************************************************************
*/
char bodyPerf[] = R"=====(
<h1>Performance Variables</h1>
<form action="">
<fieldset>
<legend>performance key values</legend>
<div><label for="uptime">Uptime</label><span id="uptime">_</span></div>
<div><label for="average">Average (cycles/sec)</label><span id="average">_</span></div>
</fieldset>
</form>
<script> 
function getprf() {
var reqxml
var resp
var obj

reqxml = new XMLHttpRequest()
reqxml.onreadystatechange = function () {
if (this.readyState === 4 && this.status === 200) {
resp = reqxml.responseText
console.log(resp)
obj = JSON.parse(resp)
document.getElementById('uptime').innerHTML = obj.uptime
document.getElementById('average').innerHTML = obj.average
}
}
var urlXML = "/getprf"
console.log(urlXML)
reqxml.open('GET', urlXML, true)
reqxml.send()
}

window.onload = getprf();
setInterval(getprf, 1000);
</script> 
)=====";

/*
**************************************************************************************************
  BodyBMP BMP280, function getbmp
**************************************************************************************************
*/
char bodyBMP[]= R"=====(
<h1>BMP280 Variables</h1>
<form action="">
<fieldset>
<legend>BMP280 key values</legend>
<div><label for="temperature">Temperature</label><span id="temperature">_</span></div>
<div><label for="pressure">Pressure</label><span id="pressure">_</span></div>
<div><label for="altitude">Altitude</label><span id="altitude">_</span></div>
</fieldset>
</form>
<script> 
function getbmp() {
var reqxml
var resp
var obj

reqxml = new XMLHttpRequest()
reqxml.onreadystatechange = function () {
if (this.readyState === 4 && this.status === 200) {
resp = reqxml.responseText
console.log(resp)
obj = JSON.parse(resp)
document.getElementById('temperature').innerHTML = obj.temperature
document.getElementById('pressure').innerHTML = obj.pressure
document.getElementById('altitude').innerHTML = obj.altitude
}
}
var urlXML = "/getbmp"
console.log(urlXML)
reqxml.open('GET', urlXML, true)
reqxml.send()
}

window.onload = getbmp();
setInterval(getbmp, 1000);
</script> 
)=====";

/*
**************************************************************************************************
  BodyIMU MCU9250, function getmcu
**************************************************************************************************
*/
char bodyIMU[] = R"=====(
<h1>IMU 9250 variables</h1>
<form action="">
<fieldset>
<legend>IMU9250 key values</legend>
<div><label for="accx">Acceleration x</label><span id="accx">_</span></div>
<div><label for="accy">Acceleration y</label><span id="accy">_</span></div>
<div><label for="accz">Acceleration z</label><span id="accz">_</span></div>
<div><label for="gyrx">Gyroscope x</label><span id="gyrx">_</span></div>
<div><label for="gyry">Gyroscope y</label><span id="gyry">_</span></div>
<div><label for="gyrz">Gyroscope z</label><span id="gyrz">_</span></div>
<div><label for="magx">Magnetometer x</label><span id="magx">_</span></div>
<div><label for="magy">Magnetometer y</label><span id="magy">_</span></div>
<div><label for="magz">Magnetometer z</label><span id="magz">_</span></div>
<div><label for="angx">Angles x</label><span id="angx">_</span></div>
<div><label for="angy">Angles y</label><span id="angy">_</span></div>
<div><label for="angz">Angles z</label><span id="angz">_</span></div>
<div><label for="pitch">Pitch</label><span id="pitch">_</span></div>
<div><label for="roll">Roll</label><span id="roll">_</span></div>
</fieldset>
</form>
<script> 
function getmcu() {
var reqxml
var resp
var obj
reqxml = new XMLHttpRequest()
reqxml.onreadystatechange = function () {
if (this.readyState === 4 && this.status === 200) {
resp = reqxml.responseText
console.log(resp)
obj = JSON.parse(resp)
document.getElementById('accx').innerHTML = obj.accx
document.getElementById('accy').innerHTML = obj.accy
document.getElementById('accz').innerHTML = obj.accz
document.getElementById('gyrx').innerHTML = obj.gyrx
document.getElementById('gyry').innerHTML = obj.gyry
document.getElementById('gyrz').innerHTML = obj.gyrz
document.getElementById('magx').innerHTML = obj.magx
document.getElementById('magy').innerHTML = obj.magy
document.getElementById('magz').innerHTML = obj.magz
document.getElementById('angx').innerHTML = obj.angx
document.getElementById('angy').innerHTML = obj.angy
document.getElementById('angz').innerHTML = obj.angz
document.getElementById('pitch').innerHTML = obj.pitch
document.getElementById('roll').innerHTML = obj.roll
}
}
var urlXML = "/getmcu"
console.log(urlXML)
reqxml.open('GET', urlXML, true)
reqxml.send()
}
window.onload = getmcu();
setInterval(getmcu, 1000);
</script> 
)=====";

/*
**************************************************************************************************
  BodyVar Car variables, function getvar
**************************************************************************************************
*/
char bodyVar[] = R"=====(
<h1>Car variables</h1>
<form action="">
<fieldset>
<legend>Car key values</legend>
<div><label for="v1m">Voltage  cell 1</label><span id="v1m">_</span></div>
<div><label for="c1p">Capacity cell 1</label><span id="c1p">_</span></div>
<div><label for="v1m">Voltage  cell 2</label><span id="v2m">_</span></div>
<div><label for="c1p">Capacity cell 2</label><span id="c2p">_</span></div>
<div><label for="v1m">Voltage  cell 3</label><span id="v3m">_</span></div>
<div><label for="c1p">Capacity cell 3</label><span id="c3p">_</span></div>
</fieldset>
</form>
<script> 
function getvar() {
var reqxml
var resp
var obj
reqxml = new XMLHttpRequest()
reqxml.onreadystatechange = function () {
if (this.readyState === 4 && this.status === 200) {
resp = reqxml.responseText
console.log(resp)
obj = JSON.parse(resp)
document.getElementById('v1m').innerHTML = obj.v1m
document.getElementById('c1p').innerHTML = obj.c1p
document.getElementById('v2m').innerHTML = obj.v2m
document.getElementById('c2p').innerHTML = obj.c2p
document.getElementById('v3m').innerHTML = obj.v3m
document.getElementById('c3p').innerHTML = obj.c3p
}
}
var urlXML = "/getvar"
console.log(urlXML)
reqxml.open('GET', urlXML, true)
reqxml.send()
}
window.onload = getvar();
setInterval(getvar, 5000);
</script> 
)=====";


char pageFooter[] = R"=====(
<footer>
Developed by: <a href="mailto:juliocardenasr@gmail.com"> juliocardenasr@gmail.com</a>
</footer> 
</body>
</html>
)=====";
