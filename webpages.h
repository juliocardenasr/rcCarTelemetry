char pageHeader[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>MegaWifi</title>
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

char bodyIMU[] = R"=====(
<h1>IMU 9250 variables</h1>
)=====";

char pageFooter[] = R"=====(
<footer>
Developed by: <a href="mailto:juliocardenasr@gmail.com"> juliocardenasr@gmail.com</a>
</footer> 
</body>
</html>
)=====";
