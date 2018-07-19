/**********************************************************************;
* Project           : Aeva Labs Demo Unit
*
* Program name      : MVP-V11.ino
*
* Author            : wdcosta
*
* Date created      : 06/15/2018
*
* Date edited      : 06/29/2018
*
* Purpose           : Main page for the MVP code
*
|**********************************************************************/
#include <math.h>
#include <stdint.h>
#include <UTFT.h>
#include <SPI.h>
#include <Wire.h>
#include <URTouch.h>
#include "macros.h"
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>

/*uint8_t tempOfSystem = 860; // Room Temp*/
uint8_t i=0;
uint8_t temp=0;
uint8_t temp2 = 0;

uint8_t Start_flag = 0;
uint8_t P_flag = 0;
uint8_t P1_flag = 0;
uint8_t P2_flag = 0;
uint8_t Stop_flag = 0;

int8_t minutes1 = 0;
int8_t minutes2 = 0;
int8_t hours = 0;

int8_t minutes3 = 0;
int8_t minutes4 = 0;
int8_t hours2 = 0;
int8_t Max_seconds = 0;
int8_t Max_seconds2 = 0;
int8_t Max_minutes = 0;
int8_t Max_minutes2 = 0;
int8_t Max_hours = 0;
int8_t Max_minutes3 = 0;
int8_t Max_minutes4 = 0;
int8_t Max_hours2 = 0;

int runTimeTimerCount = 0;
int tempLimit = 468; // - digital limit calculated for temperature control. 
int tempLimitAnalog = 75; // - analog limit for informing users.
int currentTemp = 0; // 0-1023, not stored as F value.

volatile char operationFlag = 0;
volatile int Set_Extraction_Temp=0;
volatile int Set_Maturation_Temp=0;

volatile int Total_Minutes=0;

volatile int P1_Hours=0;
volatile int P1_Total_Minutes=0;
volatile int P1_Minutes1=0;
volatile int P1_Minutes2=0;
volatile int P1_TotalTime=0;

volatile int P2_Hours=0;
volatile int P2_Total_Minutes=0;
volatile int P2_Minutes1=0;
volatile int P2_Minutes2=0;
volatile int P2_TotalTime=0;

volatile unsigned int TotalTime=0;
volatile unsigned int TotalTime2=0;
volatile unsigned int Time_Remaining=0;

volatile int stage=0;
volatile int page=0;

char cmdStr[11]; //Commands can be up to 10 letters/number longs
char newtemp[10];
char newtemp2[10];
char newmins1[10];
char newmins2[10];
char newhours[10];
char newmins3[10];
char newmins4[10];
char newhours2[10];
char Display_Current_Temp[10];

char Max_newsecs[10];
char Max_newsecs2[10];
char Max_newmins1[10];
char Max_newmins2[10];
char Max_newhours[10];

char Process_Time[10];
//Countimer timer;

unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long previousMillis2 = 0;        // will store last time LED was updated
unsigned long previousMillis3 = 0;        // will store last time LED was updated
unsigned long previousMillis4 = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)
volatile int val2 = 0;
//SoftwareSerial wifiSerial(7,8); //RX, TX
// Wifi chip needs a new line character after commands
uint8_t addr  = 0x38;  //CTP IIC ADDRESS

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t BigFont[];
extern uint8_t Arial_round_16x24[];
extern uint8_t Inconsola[];
extern uint8_t Various_Symbols_32x32[];


double Input, Output,currentWindowPidOutput; 
double Setpoint; //EDIT Set initial Setpoint
float kp = 16.16;    //User inserts PID values obtaines from Autotune here
float ki = 0.14;    //
float kd = 480.10;  //

PID myPID(&Input, &Output, &Setpoint,kp,ki,kd, DIRECT);
//PID_ATune aTune(&Input, &Output);


int WindowSize = 15000;
unsigned long windowStartTime;

float vcc = 4.91;                       // only used for display purposes, if used
float pad = 9850;                       // balance/pad resistor value, set this to
float thermr = 10000;                   // thermistor nominal resistance

// LCD variables
// -------------------------------------------------------------------------------------------------------------------  
UTFT myGLCD(SSD1963_800480,38,39,40,41);  //(byte model, int RS, int WR, int CS, int RST, int SER)
uint16_t tx, ty;

enum {
  eNORMAL = 0,
  eTEST   = 0x04,
  eSYSTEM = 0x01
};

struct TouchLocation
{
  uint16_t x;
  uint16_t y;
};

TouchLocation touchLocations[5];

uint8_t readFT5206TouchRegister( uint8_t reg );
uint8_t readFT5206TouchLocation( TouchLocation * pLoc, uint8_t num );
uint8_t readFT5206TouchAddr( uint8_t regAddr, uint8_t * pBuf, uint8_t len );

uint32_t dist(const TouchLocation & loc);
uint32_t dist(const TouchLocation & loc1, const TouchLocation & loc2);

bool sameLoc( const TouchLocation & loc, const TouchLocation & loc2 );


uint8_t buf[30];

uint8_t readFT5206TouchRegister( uint8_t reg )
{
  Wire.beginTransmission(addr);
  Wire.write( reg );  // register 0
  uint8_t retVal = Wire.endTransmission();
  
  uint8_t returned = Wire.requestFrom(addr, uint8_t(1) );    // request 6 uint8_ts from slave device #2
  
  if (Wire.available())
  {
    retVal = Wire.read();
  }
  
  return retVal;
}

uint8_t readFT5206TouchAddr( uint8_t regAddr, uint8_t * pBuf, uint8_t len )
{
  Wire.beginTransmission(addr);
  Wire.write( regAddr );  // register 0
  uint8_t retVal = Wire.endTransmission();
  
  uint8_t returned = Wire.requestFrom(addr, len);    // request 1 bytes from slave device #2
  
  uint8_t i;
  for (i = 0; (i < len) && Wire.available(); i++)
  {
    pBuf[i] = Wire.read();
  }
  
  return i;
}

uint8_t readFT5206TouchLocation( TouchLocation * pLoc, uint8_t num )
{
  uint8_t retVal = 0;
  uint8_t i;
  uint8_t k;
  
  do
  {
    if (!pLoc) break; // must have a buffer
    if (!num)  break; // must be able to take at least one
    
    uint8_t status = readFT5206TouchRegister(2);
    
    static uint8_t tbuf[40];
    
    if ((status & 0x0f) == 0) break; // no points detected
    
    uint8_t hitPoints = status & 0x0f;
    
   
    
    readFT5206TouchAddr( 0x03, tbuf, hitPoints*6);
    
    for (k=0,i = 0; (i < hitPoints*6)&&(k < num); k++, i += 6)
    {
      pLoc[k].x = (tbuf[i+0] & 0x0f) << 8 | tbuf[i+1];
      pLoc[k].y = (tbuf[i+2] & 0x0f) << 8 | tbuf[i+3];
    }
    
    retVal = k;
    
  } while (0);
  
  return retVal;
}

void writeFT5206TouchRegister( uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(addr);
  Wire.write( reg );  // register 0
  Wire.write( val );  // value
  
  uint8_t retVal = Wire.endTransmission();  
}

void readOriginValues(void)
{
  writeFT5206TouchRegister(0, eTEST);
  delay(1);
  //uint8_t originLength = readFT5206TouchAddr(0x08, buf, 8 );
  uint8_t originXH = readFT5206TouchRegister(0x08);
  uint8_t originXL = readFT5206TouchRegister(0x09);
  uint8_t originYH = readFT5206TouchRegister(0x0a);
  uint8_t originYL = readFT5206TouchRegister(0x0b);
  
  uint8_t widthXH  = readFT5206TouchRegister(0x0c);
  uint8_t widthXL  = readFT5206TouchRegister(0x0d);
  uint8_t widthYH  = readFT5206TouchRegister(0x0e);
  uint8_t widthYL  = readFT5206TouchRegister(0x0f);
  
  //if (originLength)
  {
    Serial.print("Origin X,Y = ");
    Serial.print( uint16_t((originXH<<8) | originXL) );
    Serial.print(", ");
    Serial.println( uint16_t((originYH<<8) | originYL) );
    
    Serial.print("Width X,Y = ");
    Serial.print( uint16_t((widthXH<<8) | widthXL) );
    Serial.print(", ");
    Serial.println( uint16_t((widthYH<<8) | widthYL) );
  }
  
}

uint32_t dist(const TouchLocation & loc)
{
  uint32_t retVal = 0;
  
  uint32_t x = loc.x;
  uint32_t y = loc.y;
  
  retVal = x*x + y*y;
  
  return retVal;
}
uint32_t dist(const TouchLocation & loc1, const TouchLocation & loc2)
{
  uint32_t retVal = 0;
  
  uint32_t x = loc1.x - loc2.x;
  uint32_t y = loc1.y - loc2.y;
  
  retVal = sqrt(x*x + y*y);
  
  return retVal;
}

bool sameLoc( const TouchLocation & loc, const TouchLocation & loc2 )
{
  return dist(loc,loc2) < 50;
}

// SETUP/INITIALIZATION
// -------------------------------------------------------------------------------------------------------------------  
void setup()
{
  //andomSeed(analogRead(0));
  Serial.begin(115200);
  Wire.begin();        // join i2c bus (address optional for master)
  
// Setup the LCD
 myGLCD.InitLCD();
 
// -------------------------------------------------------------
  pinMode(BACKLIGHT, OUTPUT);  //backlight 
  digitalWrite(BACKLIGHT, HIGH);//on
  pinMode(TEMP_PIN, INPUT);
  pinMode(ULTRASOUND_PIN, OUTPUT);
  digitalWrite(ULTRASOUND_PIN, LOW);
  pinMode(COOLING_PIN, OUTPUT);
  digitalWrite(COOLING_PIN, LOW);
  pinMode(HEATING_PIN, OUTPUT);
  digitalWrite(HEATING_PIN, LOW);
  readOriginValues();
  pinMode(FT5206_WAKE, INPUT);
  digitalWrite(FT5206_WAKE, HIGH );
  writeFT5206TouchRegister(0, eNORMAL); // device mode = Normal
  pinMode(FT5206_INT, INPUT);
  //digitalWrite(FT5206_INT, HIGH ); 
  
  uint8_t periodMonitor = readFT5206TouchRegister(0x89);  
  uint8_t  lenLibVersion = readFT5206TouchAddr(0x0a1, buf, 2 );
  if (lenLibVersion)
  {
    uint16_t libVersion = (buf[0] << 8) | buf[1];
  }
  else
  {
    //Serial.println("lib version length is zero");
  }
  
  uint8_t firmwareId = readFT5206TouchRegister( 0xa6 );
  
  windowStartTime = millis();

  //initialize the variables we're linked to
  Setpoint = 49;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

}

// MAIN WHILE LOOP
// -------------------------------------------------------------------------------------------------------------------  
void loop()
{ 
Serial.print("Sys: Online");  
 digitalWrite(ULTRASOUND_PIN, HIGH); //Turn on ultrasound
//StartUpScreen();   
}

