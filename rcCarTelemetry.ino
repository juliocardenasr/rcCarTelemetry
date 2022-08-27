/*
*******************************************************************************
  Program     : rcCarTelemetry
  Author      : Julio Cardenas  juliocardenasr@gmail.com
  Objective   : To obtain telemetry variables for battery rc car
                From a 3-axis accelerometer, giroscope and magnetometer
                read the needed data to obtain the angles pitch, roll and jaw.
                Also obtain the pressure, temperature and altitude.
                Read from the rc receiver the steering and motor throtle data.
                Read from voltage divisors the battery status.
                Read from the hall sensors the rotation speed at the spur to 
                obtain the car speed.
                All this data will showed in serial console, 128x64 display and 
                web page.
                The web server will build in the lolin32 card. 

  This sprint : Ticker: to manage the time of diferent tasks
                Perfomance variables: cycles the main loop per second
                Led Whitness: implement the morse code for the message:
                              "JULIO123"
                clock: implement an hh:mm:ss clock
                Console: print the previous variables each minute
                I2C: The base to comunicate with bmp280 mpu9250 and oled display
                BMP280: to obtain atm pressure, temperature and altitude
                SSD1306: Oled Display to show in several pages the information
                WIFI: Reach connection with only one wifi network
				WebServer: Five pages, html, css and js. 3 ajax routines
				           ip parameters, performance, bmp280 
			    MPU9250: connect the MPU and read accelerometer data.
				         The data is presented in the OLED display and console
                         Read and present the gyroscope data
                         Read and present the magnometer data
                The function actualizeDisplay was modified
                Read and present the angles data
                Add the ajax funcion for the MPU9250
                Read battery voltages, show them in console, display, webserver


  Date        : Jul 28 2022
  platform    : lolin32 v1.0.0
********************************************************************************
*/

/*
**************************************************************************************************
  Wifi variables
**************************************************************************************************
*/
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

#include "arduino_secrets.h"

const char* ssid1;
const char* password1;
const char* ssid2;
const char* password2;
String ipLocal;
String s_ssid;

/*
********************************************************************************
  Performance variables
********************************************************************************
*/
unsigned long cycles;
unsigned long average;

/*
********************************************************************************
  Time variables
********************************************************************************
*/
#include <Ticker.h>
Ticker  tickerLed;
Ticker  tickerClock;
Ticker  tickerBmp280;
Ticker  tickerMPU9250;
Ticker  tickerBattery;
Ticker  tickerConsole;
Ticker  tickerPage;
Ticker  tickerDisplay;

/*
********************************************************************************
  Led variables
********************************************************************************
*/
int pinLed;
int indexLed;
int lenLed;
String msgLed;

// in Lolin 32 the led goes to on when the pin is LOW
void actualizeLed() 
{
    if (indexLed >= lenLed) indexLed = 0;
    if (msgLed.charAt(indexLed) == '1') 
    {
        digitalWrite(pinLed, LOW);
    }
    else
    {
        digitalWrite(pinLed, HIGH);
    }
    indexLed++;
}

/*
********************************************************************************
  Clock variables
********************************************************************************
*/
unsigned long elapsedSeconds;

int seconds;
int minutes;
int hours;
String stime;

void actualizeClock() 
{
    elapsedSeconds++;
    seconds++;

    if (seconds > 59) 
    {
        seconds = 0;
        minutes++;
        if (minutes > 59) 
        {
            minutes = 0;
            hours++;
            if (hours > 23) 
            {
                hours = 0;
            }
        }
    }

    stime = "";
    if (hours < 10) 
    {
        stime = stime + "0";
    }
    stime = stime + String(hours);
    stime = stime + ":";
    if (minutes < 10) 
    {
        stime = stime + "0";
    }
    stime = stime + String(minutes);
    stime = stime + ":";
    if (seconds < 10) 
    {
        stime = stime + "0";
    }
    stime = stime + String(seconds);
}

/*
********************************************************************************
  I2C variables
********************************************************************************
*/
#define PinSDA 21
#define PinSCL 22
#include <Wire.h>

/*
********************************************************************************
  BMP280 variables
********************************************************************************
*/
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;

float  temperature;
float  pressure;
float  altitude;

void actualizeBmp280() 
{
    temperature = bmp.readTemperature();
    pressure    = bmp.readPressure();
    altitude    = bmp.readAltitude(1013.25);
}

/*
**************************************************************************************************
  MPU9250 variables
**************************************************************************************************
*/
#include <MPU9250_WE.h>
#define MPU9250_ADDR 0x68
MPU9250_WE MPU9250 = MPU9250_WE(MPU9250_ADDR);

// acelerometer variables
xyzFloat accRaw; 
xyzFloat accCorrRaw;
xyzFloat gValue; 
float    resultantG;

// gyroscope variables
xyzFloat gyrRaw; 
xyzFloat gyrCorrRaw;
xyzFloat gyr;

// magnetometer variables
xyzFloat mag;

// angles
xyzFloat angles;
float    pitch;
float    roll;
float    jaw;

void actualizeMPU9250() 
{
     accRaw     = MPU9250.getAccRawValues();
     accCorrRaw = MPU9250.getCorrectedAccRawValues();
     gValue     = MPU9250.getGValues();
     resultantG = MPU9250.getResultantG(gValue);

     gyrRaw     = MPU9250.getGyrRawValues();
     gyrCorrRaw = MPU9250.getCorrectedGyrRawValues();
     gyr        = MPU9250.getGyrValues();

     mag        = MPU9250.getMagValues();

     angles     = MPU9250.getAngles();
     pitch      = MPU9250.getPitch();
     roll       = MPU9250.getRoll();
}

 /*
**************************************************************************************************
  Battery variables
**************************************************************************************************
*/
float v1s;
float v1t;
float v1m;
float k1m;
float c1p;
 
float v2s;
float v2t;
float v2m;
float k2m;
float c2p;

float v3s;
float v3t; 
float v3m;
float k3m;
float c3p;

int const PINV1T = 36;
int const PINV2T = 39;
int const PINV3T = 32;

//constants to convert read values to voltage 
float const    m = (3.2 -0.1) / (4095-0);
float const    b = 3.2 - m*4095;

//constants to calculate the cell capacity percentage
float const   m1 = (100.0 - 0.0) / (4.2 - 3.3);
float const   b1 = 100.0 - m1 * 4.2;

float readVoltage(int PIN)
{
     int volt;
     volt = analogRead(PIN);
     return m*volt + b;
}

void actualizeBattery()
{
    v1t = readVoltage(PINV1T);
    v2t = readVoltage(PINV2T);
    v3t = readVoltage(PINV3T);

    v1s = k1m * v1t;
    v2s = k2m * v2t;
    v3s = k3m * v3t;

    v1m = v1s;
    v2m = v2s - v1s;
    v3m = v3s - v2s;

    c1p = m1*v1m + b1;
    c2p = m1*v2m + b1;
    c3p = m1*v3m + b1; 
}

/*
********************************************************************************
  Console variables
********************************************************************************
*/
void actualizeConsole() 
{
	Serial.println("\n\n\n***************************************************");
	Serial.print("Time      :  ");
    Serial.println(stime);
    Serial.print("cycles/sec : ");
    Serial.println(average);
    Serial.println();

    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Pressure   : ");
    Serial.println(pressure);
    Serial.print("Altitude   : ");
    Serial.println(altitude);
	Serial.println();

	Serial.print("Acceleration x: ");
    Serial.println(accCorrRaw.x); 
    Serial.print("Acceleration y: ");
    Serial.println(accCorrRaw.y); 
    Serial.print("Acceleration z: ");
    Serial.println(accCorrRaw.z); 
    Serial.println();

    Serial.print("Gyroscope x: ");
    Serial.println(gyr.x);
    Serial.print("Gyroscope y: ");
    Serial.println(gyr.y); 
    Serial.print("Gyroscope z: ");
    Serial.println(gyr.z);  
    Serial.println();

    Serial.print("Magnetometer x: ");
    Serial.println(mag.x); 
    Serial.print("Magnetometer y: ");
    Serial.println(mag.y); 
    Serial.print("Magnetometer z: ");
    Serial.println(mag.z);
    Serial.println();

    Serial.print("Angle x: ");
    Serial.println(angles.x); 
    Serial.print("Angle y: ");
    Serial.println(angles.y); 
    Serial.print("Angle z: ");
    Serial.println(angles.z);
    Serial.print("Pitch  : ");
    Serial.println(pitch);
    Serial.print("Roll   : ");
    Serial.println(roll);
    Serial.println();     
}

/*
********************************************************************************
  Display variables
********************************************************************************
*/
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int indexPage;
int indexPagePrevious;
int maxPage;
enum pages {ip, perf, atm, mcuA, mcuG, mcuM, ang, batt, inf};

void sendStringXY(String msg, int X, int Y)
{
    unsigned char i;
    char c;

    display.setCursor(X*6, Y*8);
    for (i = 0; i < msg.length(); i++) 
    {
        c = msg.charAt(i);   
        display.write(c);
    }
}

void messageValue(String message, float value, int row, int col1, int col2)
{
    String svalue;
    sendStringXY(message, col1, row);
    svalue = String(value) + "       ";
    svalue = svalue.substring(0,7);
    sendStringXY(svalue, col2, row);
}

void changePage(){
    indexPage++;
    if (indexPage >= maxPage) indexPage = 0; 
}

void actualizeDisplay() {
    if (indexPage != indexPagePrevious) {
        indexPagePrevious = indexPage;
    }
    display.clearDisplay();
    switch (indexPage) {
        case ip:
		        sendStringXY("1 - Network  ",  0, 0);
                sendStringXY("IP local:",      0, 2);
                sendStringXY(ipLocal,          0, 3);
                sendStringXY("SSID    :",      0, 5);
                sendStringXY(s_ssid,           0, 6);
                break; 
        case perf:
                sendStringXY("2 - Performance", 0, 0);
                sendStringXY("Time:",           0, 2);
                sendStringXY(stime,             8, 2);
                messageValue("Avg:", average, 4, 0, 8);
                break;
        case atm:
                sendStringXY("3 - Press. and Temp.", 0, 0);
                messageValue("Temp:", temperature, 2, 0, 8);
                messageValue("Pres:", pressure,    4, 0, 8);
                messageValue("Alt :", altitude,    6, 0, 8);  
                break;
        case mcuA: 
                sendStringXY("4 - Accelerometer", 0, 0);
                messageValue("Acc x:", accCorrRaw.x, 2, 0, 8);
                messageValue("Acc y:", accCorrRaw.y, 4, 0, 8);
                messageValue("Acc z:", accCorrRaw.z, 6, 0, 8); 
                break;               
        case mcuG: 
                sendStringXY("5 - Gyroscope", 0, 0);
                messageValue("Gyr x:", gyr.x, 2, 0, 8);
                messageValue("Gyr y:", gyr.y, 4, 0, 8);
                messageValue("Gyr z:", gyr.z, 6, 0, 8);
                break;           
        case mcuM: 
                sendStringXY("6 - Magnetometer", 0, 0);
                messageValue("Mag x:", mag.x, 2, 0, 8);
                messageValue("Mag y:", mag.y, 4, 0, 8);
                messageValue("Mag z:", mag.z, 6, 0, 8);
                break;                
        case ang:
                sendStringXY("7 - Angles", 0, 0);
                messageValue("Ang x:", angles.x, 2, 0, 8);
                messageValue("Ang y:", angles.y, 3, 0, 8);
                messageValue("Ang z:", angles.z, 4, 0, 8);
                messageValue("Pitch:", pitch,    6, 0, 8);
                messageValue("Roll :", roll,     7, 0, 8);
                break;                      
        case batt: 
                sendStringXY("8 - Battery", 0, 0);
                messageValue("Cell 1:", v1m, 2, 0, 8);
                messageValue("     %:", c1p, 3, 0, 8);
                messageValue("Cell 2:", v2m, 4, 0, 8);
                messageValue("     %:", c2p, 5, 0, 8);
                messageValue("Cell 3:", v3m, 6, 0, 8);
                messageValue("     %:", c3p, 7, 0, 8);
                break;                        
        case inf:
		        sendStringXY("9 - Information     ", 0, 0);
                sendStringXY("The quick brown fox ", 0, 2);
                sendStringXY("jumps over the lazy ", 0, 3);
                sendStringXY("dog 01234567890     ", 0, 4); 
                sendStringXY("THE QUICK BROWN FOX ", 0, 5); 
                sendStringXY("JUMPS OVER THE LAZY ", 0, 6); 
                sendStringXY("DOG 01234567890     ", 0, 7);
                break;
        default: 
                break;                               
    } wifiMulti.addAP(ssid1, password1);
    display.display();
}


/*
**************************************************************************************************
  WebServer variables
**************************************************************************************************
*/

#include <WebServer.h>
WebServer server(80); 

#include "webpages.h"

void rootPage(){
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", pageHeader);
    server.sendContent(bodyHome);
    server.sendContent(pageFooter);
    server.sendContent("");  
}

void ipPage(){
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", pageHeader);
    server.sendContent(bodyIP);
    server.sendContent(pageFooter);
    server.sendContent("");  
}

void perfPage(){
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", pageHeader);
    server.sendContent(bodyPerf);
    server.sendContent(pageFooter);
    server.sendContent("");  
}

void bmpPage(){
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", pageHeader);
    server.sendContent(bodyBMP);
    server.sendContent(pageFooter);
    server.sendContent("");  
}

void imuPage(){
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", pageHeader);
    server.sendContent(bodyIMU);
    server.sendContent(pageFooter);
    server.sendContent("");  
}

void varPage() 
{
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", pageHeader);
    server.sendContent(bodyVar);
    server.sendContent(pageFooter);
    server.sendContent("");  
 
}

void pageNotFound() {
    server.send(404, "text/plain", "ooops page not found"); 
    
}

//AJAX response of ip variables JSON coded
void getipv()
{
    String response;
    response = "{\"ip\":\"" + ipLocal + "\",\"ssid\":\"" + ssid + "\"}";
    Serial.println(response);
    server.send(200, "json", response);
  
}

void getprf()
{
    String response;
    response = "{\"uptime\":\"" + stime + "\",\"average\":" + String(average) + "}";
    Serial.println(response);
    server.send(200, "json", response);
}

void getbmp(){
    String response;
    response = "{\"temperature\":" + String(temperature) + ",\"pressure\":" + String(pressure) + ",\"altitude\":" + String(altitude) + "}";
    Serial.println(response);
    server.send(200, "json", response);
}

void getmcu()
{
    String response;
    response = "{\"accx\":"  + String(accCorrRaw.x) + ",\"accy\":" + String(accCorrRaw.y) + ",\"accz\":" + String(accCorrRaw.z) +
               ",\"gyrx\":"  + String(gyrCorrRaw.x) + ",\"gyry\":" + String(gyrCorrRaw.y) + ",\"gyrz\":" + String(gyrCorrRaw.z) +
               ",\"magx\":"  + String(mag.x)        + ",\"magy\":" + String(mag.y)        + ",\"magz\":" + String(mag.z)        +
               ",\"angx\":"  + String(angles.x)     + ",\"angy\":" + String(angles.y)     + ",\"angz\":" + String(angles.z)     +
               ",\"pitch\":" + String(pitch)        + ",\"roll\":" + String(roll)         +
               "}";
    Serial.println(response);
    server.send(200, "json", response);         
}

void getvar()
{
    String response;
    response = "{\"v1m\":"  + String(v1m) + ",\"c1p\":" + String(c1p) + 
               ",\"v2m\":"  + String(v2m) + ",\"c2p\":" + String(c2p) + 
               ",\"v3m\":"  + String(v3m) + ",\"c3p\":" + String(c3p) +
               "}";
    //Serial.println(response);
    server.send(200, "json", response); 
}

/*
********************************************************************************
  setup
********************************************************************************
*/
void setup()
{
    Serial.begin(115200);
    if (!Serial) delay(100);
    Serial.println("Serial port          : OK");

    //initialize wifi variables
    ssid1     = SECRET_SSID_1;
    password1 = SECRET_PASSWORD_1;
    ssid2     = SECRET_SSID_2;
    password2 = SECRET_PASSWORD_2;
    wifiMulti.addAP(ssid1, password1);
    wifiMulti.addAP(ssid2, password2);
    while (wifiMulti.run() != WL_CONNECTED) 
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.print("Wifi adress: ");
    Serial.println(WiFi.localIP());
    ipLocal = WiFi.localIP().toString().c_str();
    s_ssid  = WiFi.SSID();

    // initialize performance Variables
    cycles = 0;
    average = 0;
    Serial.println("Performance variables: OK");

    // initialize led variables
    pinLed = LED_BUILTIN;
    pinMode(pinLed, OUTPUT);
    //.--- ..- .-.. .. --- .---- ..--- ...--   JULIO123
    msgLed = "01011101110111001010111001011101010010100111011101110010111011101110111001010111011101110010101011101110";
    lenLed = msgLed.length();
    indexLed = 0;
    tickerLed.attach_ms(120, actualizeLed);
    Serial.println("Led variables        : OK");

    // initialize clock variables
    elapsedSeconds = 1;
    seconds = 0;
    minutes = 0;
    hours   = 0;
    stime   = "00:00:00";
    tickerClock.attach(1, actualizeClock);
    Serial.println("Clock variables      : OK");

    //Initialize the I2C bus
    Wire.begin(PinSDA,PinSCL); 
    Serial.println("I2C Bus              : OK"); 
    
    //initalize bmp variables
    temperature = 0.0;
    pressure    = 0.0;
    altitude    = 0.0;
    unsigned status;
    bmp.begin(0x76);
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    tickerBmp280.attach(2, actualizeBmp280);
    Serial.println("BMP280               : OK");

    // initialize MPU9250 variables
    if (!MPU9250.init())
    {
        Serial.println("MPU9250 does not respond");
    }
    else
    {
        Serial.println("MPU9250 is connected");
    }
    Serial.println("Position you MPU9250 flat and don't move it - calibrating...");
    delay(1000);
    MPU9250.autoOffsets();
    Serial.println("Done!");

    MPU9250.setSampleRateDivider(5);
    MPU9250.setAccRange(MPU9250_ACC_RANGE_2G);
    MPU9250.enableAccDLPF(true);
    MPU9250.setAccDLPF(MPU9250_DLPF_6);

    MPU9250.setGyrRange(MPU9250_GYRO_RANGE_250);
    MPU9250.enableGyrDLPF();
    MPU9250.setGyrDLPF(MPU9250_DLPF_6);

    if (!MPU9250.initMagnetometer())
    {
        Serial.println("Magnetometer does not respond");
    }
    else
    {
        Serial.println("Magnetometer is connected");
    }
    MPU9250.setMagOpMode(AK8963_CONT_MODE_100HZ);

    tickerMPU9250.attach_ms(900, actualizeMPU9250);
    Serial.println("MPU9250             : OK");

    // initialize battery
    pinMode(PINV1T, INPUT);
    pinMode(PINV2T, INPUT);
    pinMode(PINV3T, INPUT);
    k1m = 2.0;    // (20k + 20K) / 20k  is the resistive divisor inverse
    k2m = 4.4;    // (68k + 20k) / 20k
    k3m = 6.1;    // (51k + 10k) / 10k      
    tickerBattery.attach(2, actualizeBattery);
    Serial.println("Battery              : OK");

    // initialize console variables
    tickerConsole.attach(60, actualizeConsole);
    Serial.println("Console variables    : OK");

    // initialize display variables
    indexPage         = 0;
    indexPagePrevious = 0;
    maxPage           = 9;
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) 
    {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    delay(2000);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);         
    display.cp437(true);
    display.display();
    tickerPage.attach(8, changePage);
    tickerDisplay.attach(1, actualizeDisplay);
    Serial.println("Display variables    : OK");

	// initialize web server variables
    server.on("/Home.html", rootPage);
    server.on("/IP.html",   ipPage);
    server.on("/Perf.html", perfPage);
    server.on("/BMP.html",  bmpPage);
    server.on("/IMU.html",  imuPage);
    server.on("/Var.html",  varPage);
    server.onNotFound(pageNotFound);
    server.on("/getipv",    getipv);
    server.on("/getprf",    getprf);
    server.on("/getbmp",    getbmp);
    server.on("/getmcu",    getmcu);
    server.on("/getvar",    getvar);
    server.begin();
}

void loop()
{
    server.handleClient();  
    cycles++;
    average = cycles / elapsedSeconds;
}