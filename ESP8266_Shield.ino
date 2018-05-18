/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how to use ESP8266 Shield (with AT commands)
  to connect your project to Blynk.

  WARNING!
    It's very tricky to get it working. Please read this article:
    http://help.blynk.cc/hardware-and-libraries/arduino/esp8266-with-at-firmware

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* BLYNK PINOUTS:
 *  Pins from hardware -> app (V0 to V9)
 *   
 *   
 *  Pins from app -> hardware (V10+)
 *   V10 = System Status
 *   V11 = Terminal 
 */


/* Comment this out to disable prints and save space */
//#define BLYNK_PRINT Serial

#define TEMP_PIN            A0
#define COOLING_PIN         12
#define ULTRASOUND_PIN      13
#define DECODE_FLAG         0x01
#define TREAD_FLAG          0x02
#define TMONITOR_FLAG       0x04
#define STATUS_FLAG         0x08
#define END_OF_TEST_FLAG    0x10
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
// You should get Auth Token in the Blynk App. 
// Go to the Project Settings (nut icon). 

// Your WiFi credentials.
// Set password to "" for open networks.

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
//SoftwareSerial EspSerial(7, 8); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

//ESP8266 wifi(&EspSerial);

//BlynkTimer secondTimer;
int runTimeTimerCount = 0;
//char auth[] = "57af6d48d2384134b09fc2cd7bc03d1e";
//char ssid[] = "fearn";
//char pass[] = "fearnside"; 
//F1R3INDAH0L3fire1ndahole(!)!((%"
int tempLimit = 468; // - digital limit calculated for temperature control. 
int tempLimitAnalog = 75; // - analog limit for informing users.
int currentTemp = 0; // 0-1023, not stored as F value.
volatile char operationFlag = 0;
volatile int timeRemaining = 240; // in mins
volatile int secToMin = 0;
char cmdStr[11]; //Commands can be up to 10 letters/number longs
//WidgetTerminal terminal(V12);

void setup()
{
  // Pin Sets
  pinMode(TEMP_PIN, INPUT);
  pinMode(ULTRASOUND_PIN, OUTPUT);
  digitalWrite(ULTRASOUND_PIN, LOW);
  
  // Timer Setup
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A   = 15625; // Comparison Register set to 16,000,000 / 1024 / 1.00Hz (Once every 1 seconds) 
  TCCR1B |= (1 << WGM12); // Enable CTC
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Set clock divider to 1024
  TIMSK1 |= (1 << OCIE1A);
  TCNT1  = 0;
      
  // Debug console
  Serial.begin(115200);
  while(!Serial){
    // delay for serial boot
  }
  // Set ESP8266 baud rate
/*
  EspSerial.begin(ESP8266_BAUD);
  while(!EspSerial){
    // delay for wifi soft serial
  }
*/
//  Blynk.begin(auth, wifi, ssid, pass);
  Serial.println("Blynk booted");
  
//  secondTimer.setInterval(15000L, timerEvent);
//  terminal.println("Boot up complete.");
//  terminal.flush();
}


void loop()
{
// Blynk.run();
// secondTimer.run();
 if(operationFlag & TREAD_FLAG){ //If temperature ISR has run read temp
    readTemp();
 }
// if(operationFlag & TMONITOR_FLAG){
//  monitorTemp();
// }
/*
 if (EspSerial.available()) {
    Serial.write(EspSerial.read());
 }
 */
// Serial.println(currentTemp);
 if(operationFlag & END_OF_TEST_FLAG)
 {
  digitalWrite(ULTRASOUND_PIN,LOW);
 }
 checkForCommand();
 if(operationFlag & DECODE_FLAG){
   decodeCommand();  
 }
 if(operationFlag & STATUS_FLAG){
   operationFlag &= ~STATUS_FLAG;
      int extrapolatedTemperature;
      extrapolatedTemperature = exp((float)(currentTemp+1547)/477);
      Serial.print("Sys: Online");
      Serial.print("ST: ");
      Serial.print(tempLimitAnalog);
      Serial.print(", CT: ");
      Serial.print(extrapolatedTemperature);
      Serial.print(". Time Left: ");
      Serial.print(timeRemaining);
      Serial.println(".");
      Serial.print("US: ");
      if(digitalRead(ULTRASOUND_PIN)){
        Serial.println("On.");
      }
      else{
        Serial.println("Off.");
      }    

    
 }
}

void calculateTempLimit(int limitTemp){
  // Conversion from a given temperature to a logical # (0-1023)
  // Logic = 477 * ln(temperature in F) - 1547 : r^2 = 0.997 over 50 degrees -> 175
  tempLimit = 477 * log(limitTemp) - 1547;
// Change serial bacck to Terminal:  
  Serial.print("New Digital TLim: ");
  Serial.println(tempLimit);
//  terminal.flush();
}

void monitorTemp()
{
  
}
 
void readTemp() 
{
  int tempRead1 = 0;
  int tempRead2 = 0;
  int tempRead3 = 0;
  int tempRead4 = 0;
  int tempRead5 = 0;
  operationFlag &= ~TREAD_FLAG; //turn off temp flag
  tempRead1 = analogRead(A0); //throwaway read so caps can fill
  tempRead2 = analogRead(A0);
  tempRead3 = analogRead(A0);
  tempRead4 = analogRead(A0);
  tempRead5 = analogRead(A0);
  currentTemp = (tempRead2+tempRead3+tempRead4+tempRead5)/4;
  Serial.print("Temp read: ");
  Serial.println(currentTemp);
  //terminal.flush();
}

// BLYNK FUNCTIONS -----------------------------------------------------------------------//
/*
BLYNK_WRITE(V10)
{ // Start and Stop System
  
  int pinValue = param.asInt();
  
  if(pinValue) 
  { // if command is to start system
    operationFlag |= STATUS_FLAG; // system on
    operationFlag |= TMONITOR_FLAG; // enable temperature control
//    terminal.println("Starting System");
//    terminal.flush();
    Serial.println("Starting System");
    digitalWrite(ULTRASOUND_PIN,HIGH);
  }
  else
  { // else - command is to stop system. Full stop - turn off ultrasonic.
//    terminal.println("Stopping System");
//    terminal.flush();
    Serial.println("Stopping System");
    operationFlag &= ~STATUS_FLAG;
    digitalWrite(ULTRASOUND_PIN,LOW); //turn off ultrasound
    if(currentTemp>591){ // if temp is above 90 degrees
      digitalWrite(COOLING_PIN,HIGH);
      operationFlag |= TMONITOR_FLAG;
      tempLimit = 500; // tell system to cool
    }
  }
  
}
*/

/*
BLYNK_WRITE(V11)
{ 
  // Take desired set temperature value from Blynk app and send it to generate new goal value.
  int pinValue = param.asInt(); // assign incoming value as int
  tempLimitAnalog = pinValue;
  calculateTempLimit(pinValue);
}
*/

/*
BLYNK_WRITE(V12)
{
  // if you type "Start" into Terminal Widget - it will respond: "X"
  if (String("Start") == param.asStr()) {
    terminal.println("Start function not ready yet.");
  } 
  // Ensure everything is sent
  terminal.flush();
}
*/
/*
BLYNK_WRITE(V13)
{
  timeRemaining = param.asInt();
  Blynk.virtualWrite(V2, timeRemaining);
}
*/
// TIMERS --------------------------------------------------------------------------------//
// This function sends Arduino's up time every 15 seconds to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
/*
void timerEvent()
{
  int extrapolatedTemperature;
  extrapolatedTemperature = exp((float)(currentTemp+1547)/477);
  Blynk.virtualWrite(V1,extrapolatedTemperature);
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  if(operationFlag & STATUS_FLAG)
  {  
    runTimeTimerCount++;
    // every minute send time left in process
    if(runTimeTimerCount>=4){
      timeRemaining = timeRemaining - 1;
      if(timeRemaining < 1){
        operationFlag &= ~STATUS_FLAG;
        operationFlag |= END_OF_TEST_FLAG;
      }
      Blynk.virtualWrite(V2, timeRemaining);
      runTimeTimerCount=0;
      terminal.print("Sys: ");
      if(operationFlag & STATUS_FLAG){
        terminal.print("ON. ");
      }
      else
      {
        terminal.print("OFF. ");
      }
      terminal.print("ST: ");
      terminal.print(tempLimitAnalog);
      terminal.print(", CT: ");
      terminal.print(extrapolatedTemperature);
      terminal.print(". Time Left: ");
      terminal.print(timeRemaining);
      terminal.println(".");
      terminal.print("US: ");
      if(digitalRead(ULTRASOUND_PIN)){
        terminal.println("On.");
      }
      else{
        terminal.println("Off.");
      }
      terminal.flush();
//      
     }
  }
}
*/


// ISR ----------------------------------------------------------------------------------//

ISR(TIMER1_COMPA_vect) // Occurs once every 1 second(s)
{
  operationFlag |= TREAD_FLAG;
  secToMin = secToMin + 1;
  if (secToMin > 59){
    secToMin = 0;
    timeRemaining = timeRemaining - 1; //every minute remove a minute.
    if (timeRemaining < 1){
      operationFlag |= END_OF_TEST_FLAG;
    }
    operationFlag |= STATUS_FLAG;
  }
}

// TEMP FUNCTIONS
void checkForCommand() 
{
  byte strPosition = 0;
  int incomingByte = 0;
  while (Serial.available() > 0)
  {
      operationFlag |= DECODE_FLAG; // set decode flag
      
      incomingByte = Serial.read();
      cmdStr[strPosition] = incomingByte;
      if (cmdStr[strPosition] == '\r')
      {
        cmdStr[strPosition] = '\0';
        break;
      }    
      strPosition++; 
      delay(25);
  }
}

void decodeCommand()
{
  byte commandRecognized = 0;

  operationFlag &= ~DECODE_FLAG; // turn off decode flag
  
  Serial.println("Decoding Command");
  Serial.println(cmdStr);
  if (strcmp(cmdStr,"TON")==0)
  {
    commandRecognized = 1;
    Serial.println("Turning on Transducer");
    digitalWrite(13,HIGH);
  }
  if (strcmp(cmdStr,"TOFF")==0)
  {
    commandRecognized = 1;
    Serial.println("Turning off Transducer");
    digitalWrite(13,LOW);
  }
  if (strcmp(cmdStr,"TEMP")==0){
    commandRecognized = 1;
    Serial.println("Pausing Transducer.");
    digitalWrite(13,LOW);
    delay(100);
    delay(100);
    delay(100);
    delay(100);
    delay(100);
    readTemp();
    int extrapolatedTemperature;
    extrapolatedTemperature = exp((float)(currentTemp+1547)/477);
    Serial.print("Extrapolated Temp: ");
    Serial.println(extrapolatedTemperature);
    delay(50);
    digitalWrite(13,HIGH);  
  }
  if (commandRecognized == 0){
    Serial.println("Command Not Recognized");
  }
  
}
