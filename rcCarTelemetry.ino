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
  Date        : Jul 15 2022
  platform    : lolin32 v1.0.0
********************************************************************************
*/

/*
********************************************************************************
  Performance variables
********************************************************************************
*/
unsigned long cycles;
unsigned long average;
String saverage;

/*
********************************************************************************
  Time variables
********************************************************************************
*/
#include <Ticker.h>
Ticker  tickerLed;
Ticker  tickerClock;
Ticker  tickerConsole;

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
  Console variables
********************************************************************************
*/
void actualizeConsole() 
{
    Serial.print(stime);
    Serial.print(" ");
    Serial.println(average);
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

    // initialize console variables
    tickerConsole.attach(60, actualizeConsole);
    Serial.println("Console variables    : OK");
}

void loop()
{
    cycles++;
    average = cycles / elapsedSeconds;
}