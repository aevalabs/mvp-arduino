

/*////////////////////////////////////////////////////////////////
// ------------------------------------------------------------
// AEVA LAB CODE
// DETAILS: MVP-V3
// ------------------------------------------------------------
///////////////////////////////////////////////////////////////*/

#include <stdint.h>
#include <UTFT.h>
#include <SPI.h>
#include <Wire.h>
#include <URTouch.h>
#include "macros.h"
#include <PID_v1.h>
#include <math.h>

/*uint8_t tempOfSystem = 860; // Room Temp*/
uint8_t i=0;
uint8_t temp = 0;
int8_t minutes1 = 0;
int8_t minutes2 = 0;
int8_t hours = 0;
uint8_t flag = 1;
uint8_t Start_flag = 0;
uint8_t Stop_flag = 0;
int runTimeTimerCount = 0;
 int previousMillis3=0;
int tempLimit = 468; // - digital limit calculated for temperature control. 
int tempLimitAnalog = 75; // - analog limit for informing users.
int currentTemp = 0; // 0-1023, not stored as F value.
volatile char operationFlag = 0;
volatile int timeRemaining = 240; // in mins
volatile int secToMin = 0;
volatile int setTemp=0;
volatile int Hours=0;
volatile int Minutes=0;
volatile int Minutes1=0;
volatile int Minutes2=0;
volatile unsigned int TotalTime=0;
volatile unsigned int TotalTime2=0;
volatile int stage=0;

char cmdStr[11]; //Commands can be up to 10 letters/number longs
char newtemp[10];
char newtemp2[10];
char newmins1[10];
char newmins2[10];
char newhours[10];
Countimer timer;

unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long previousMillis2 = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 60000;           // interval at which to blink (milliseconds)
volatile int val2 = 0;
//SoftwareSerial wifiSerial(7,8); //RX, TX
// Wifi chip needs a new line character after commands
uint8_t addr  = 0x38;  //CTP IIC ADDRESS

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t BigFont[];

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=2, Ki=5, Kd=1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int WindowSize = 5000;
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
    
    Serial.print("number of hit points = ");
    Serial.println( hitPoints );
    
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
  randomSeed(analogRead(0));
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
  pinMode     (FT5206_WAKE, INPUT);
  digitalWrite(FT5206_WAKE, HIGH );
  writeFT5206TouchRegister(0, eNORMAL); // device mode = Normal
  pinMode     (FT5206_INT, INPUT);
  //digitalWrite(FT5206_INT, HIGH ); 
  
  uint8_t periodMonitor = readFT5206TouchRegister(0x89);
  Serial.print("periodMonitor = ");
  Serial.println( periodMonitor, HEX );
  
  uint8_t  lenLibVersion = readFT5206TouchAddr(0x0a1, buf, 2 );
  if (lenLibVersion)
  {
    uint16_t libVersion = (buf[0] << 8) | buf[1];
    Serial.print("lib version = ");
    Serial.println( libVersion, HEX);
  }
  else
  {
    Serial.println("lib version length is zero");
  }
  
  uint8_t firmwareId = readFT5206TouchRegister( 0xa6 );
  Serial.print("firmware ID = ");
  Serial.println( firmwareId);
  
  windowStartTime = millis();

  //initialize the variables we're linked to
  Setpoint = setTemp;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

// MAIN WHILE LOOP
// -------------------------------------------------------------------------------------------------------------------  
void loop()
{ 
  Serial.print("Sys: Online");
  StartUpScreen(); 
    
}

// StartUp screen --------------------------------------------------------------------------------//
// This function displays the begining screen on the UI
void StartUpScreen(void)
{
  int buf[798];
  int x, x2;
  int y, y2;
  int r;

// Clear the screen and draw the frame
  myGLCD.clrScr();
  
  uint8_t flag = 1;
  static uint16_t w = 800;
  static uint16_t h = 480;
  float xScale = 1024.0F/w;
  float yScale = 1024.0F/h;
  uint8_t attention = digitalRead(FT5206_INT);
  static uint8_t oldAttention = 1;
  uint32_t thisTouchTime = millis();
  uint8_t i=0;   
  static uint32_t lastTouchTime = thisTouchTime;
 
  int drawScreen = 0;
  
  myGLCD.setColor(VGA_WHITE);
  myGLCD.fillRect(0, 0, 799, 479);
  myGLCD.setColor(0, 0, 0);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_TRANSPARENT);
  myGLCD.setColor(VGA_BLACK);

  myGLCD.print("AEVA LABS", CENTER, 200);  // First page
  delay(5000);
                 
  myGLCD.setColor(102, 153, 255);
  myGLCD.fillRect(0, 0, 799, 479); //Sets the background color
  
  /////////////////////////////// START PANELS //////////////////////////
  myGLCD.setColor(VGA_BLACK); // draw borders
  myGLCD.drawRect(9,249,231,471);
  myGLCD.drawRect(249,9,551,231);
  myGLCD.drawRect(249,249,551,471);
  myGLCD.drawRect(569,9,791,471);
  
  myGLCD.setColor(VGA_GRAY); 
  myGLCD.fillRect(10, 250, 230, 470); // draw box for Status Panel
  myGLCD.fillRect(250,10,550,230); // draw box for temp panel
  myGLCD.fillRect(250,250,550,470); // draw box for time panel
  myGLCD.fillRect(570,10,790,470);  // button panel
  /////////////////////////////// STOP PANELS //////////////////////////

  ////////////////////////////// START LOGO /////////////////////////
  // Logo x = 10 to x = 230, y = 10 to y = 230
  // x midpoint = 120, y midpoint = 120
  myGLCD.setBackColor(VGA_TRANSPARENT);
  myGLCD.setColor(VGA_WHITE);

  
  for(drawScreen=0; drawScreen <= 3; drawScreen++)
  {
  i = drawScreen;
  myGLCD.drawLine(50, 10+i, 120, 110+i); //top V
  myGLCD.drawLine(120, 110+i, 190, 10+i);
  myGLCD.drawLine(100,  70+i,  120, 100+i);
  myGLCD.drawLine(120, 100+i, 140, 70+i);
  
  myGLCD.drawLine(50, 230+i, 120, 130+i); // bottom V
  myGLCD.drawLine(120, 130+i, 190, 230+i);
  myGLCD.drawLine(100, 170+i, 120, 140+i);
  myGLCD.drawLine(120, 140+i, 140, 170+i); 
  }
  myGLCD.setFont(BigFont);
  myGLCD.print("AEVA  LABS", 38, 115);
  ////////////////////////////// STOP LOGO ///////////////////////// 
  ////////////////////////////// START STATUS /////////////////////////
  // status panel goes from x = 10 to x = 230 and y = 250 to y = 470
  // x midpoint = 120, y midpoint = 360
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(BigFont);
  myGLCD.print("CUR. STATUS:", 30, 290); // Set temp:
  myGLCD.setColor(100,245,111);
  myGLCD.print("ONLINE", 60, STATUS_Y);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("WARNINGS:", STATUS_X, 390); // Set temp:
  ////////////////////////////// STOP STATUS /////////////////////////
  
  /////////////////////////////// START TEMP //////////////////////////
  myGLCD.setBackColor(VGA_GRAY); 
  
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(BigFont);
  myGLCD.print("SET INTENSITY:", CENTER, 20); // Set temp:
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print("0", CENTER, 55); //set temp value
  myGLCD.setFont(BigFont);
  myGLCD.print("CURRENT TEMP:", CENTER, 120);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print(newtemp2, CENTER, 155);
  
  myGLCD.setBackColor(VGA_TRANSPARENT);
  
  myGLCD.setColor(169,169,169);
  myGLCD.fillCircle(280,75,15);
  myGLCD.setColor(0,0,0);
  myGLCD.drawCircle(280,75,15);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(BigFont);
  myGLCD.print("+", 272, 68);   //button that increases the temp

  myGLCD.setColor(169,169,169);
  myGLCD.fillCircle(520, 75, 15);
  myGLCD.setColor(0,0,0);
  myGLCD.drawCircle(520, 75, 15);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("-", 512, 68);   //button that decreases the temp
  /////////////////////////////// END TEMP //////////////////////////

  /////////////////////////////// START TIME //////////////////////////
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(BigFont);
  myGLCD.print("SET TIME:", CENTER, 260);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print("0", 340, SET_TIME_Y);
  myGLCD.fillCircle (390,315, 4);
  myGLCD.fillCircle (390,325, 4);
  myGLCD.print("00", 410, SET_TIME_Y);
  
  myGLCD.setFont(BigFont);
  myGLCD.print("TIME REMAINING:", CENTER, 380);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print("0", 340, TIME_REMAINING_Y);
  //colon
  myGLCD.fillCircle (390, 425, 4);
  myGLCD.fillCircle (390, 435, 4);
  myGLCD.print("00", 410, TIME_REMAINING_Y);

  myGLCD.setBackColor(VGA_TRANSPARENT);
  myGLCD.setColor(169,169,169);
  myGLCD.fillCircle(280,318,15);
  myGLCD.setColor(0,0,0);
  myGLCD.drawCircle(280,318,15);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(BigFont);
  myGLCD.print("+", 272, 311);   //button that reduces the time

  myGLCD.setColor(169,169,169);
  myGLCD.fillCircle(520, 318, 15);
  myGLCD.setColor(0,0,0);
  myGLCD.drawCircle(520, 318, 15);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("-", 512, 311);   //button that increases the time
 
  /////////////////////////////// END TIME //////////////////////////

  /////////////////////////////// START BUTTONS //////////////////////////

  myGLCD.setColor(VGA_BLACK);
  myGLCD.drawRoundRect(609, 59, 751, 101);
  myGLCD.drawRoundRect(609, 164, 751, 206);
  myGLCD.drawRoundRect(609, 269, 751, 311);
  myGLCD.drawRoundRect(609, 374, 751, 416);

  myGLCD.setColor(100,245,111);
  myGLCD.fillRoundRect(610, 60, 750, 100);
  myGLCD.setBackColor(100,245,111);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(BigFont);
  myGLCD.print("START P1", 620, 75); //start button

  myGLCD.setColor(VGA_GREEN);
  myGLCD.fillRoundRect(610, 165, 750, 205);
  myGLCD.setBackColor(VGA_GREEN);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(BigFont);
  myGLCD.print("START P2", 620, 180); //start button
  
  myGLCD.setColor(VGA_RED);
  myGLCD.fillRoundRect(610, 270, 750, 310);
  myGLCD.setBackColor(VGA_RED);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(BigFont);
  myGLCD.print("STOP",650, 285); //stop button
    
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRoundRect(610, 375, 750, 415);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(BigFont);
  myGLCD.print("OFF",660, 390); //off button

/////////////////////////////// END BUTTONS //////////////////////////

  myGLCD.setFont(SmallFont);

  while(flag)
   {  
      
      attention = digitalRead(FT5206_INT);
      
       /* Wait around for touch events */
      if (!attention && oldAttention ) 
      {   
        Serial.println("Touch: ");
        uint8_t count = readFT5206TouchLocation( touchLocations, 5 );
  
         if (count)
          {
            static TouchLocation lastTouch = touchLocations[0];
            
            Serial.print("Time delta = ");
            Serial.print(thisTouchTime - lastTouchTime);
            Serial.print(", dist = ");
            Serial.println( dist(touchLocations[0],lastTouch) );
            
            lastTouch = touchLocations[0];
            
            for (i = 0; i < count; i++)
            {
              if(touchLocations[i].x>=265&&touchLocations[i].x<=305&&touchLocations[i].y>=60&&touchLocations[i].y<=90 )
              {
                Serial.println("Plus button pressed");
                if(temp<140)
                {
                  temp=temp+1;
                  sprintf(newtemp, "%d", temp);
                  Serial.println(newtemp);
                  myGLCD.setColor(VGA_GRAY);
                  myGLCD.fillRect(340,40,450,110);
                  myGLCD.setBackColor(VGA_GRAY);
                  myGLCD.setColor(VGA_WHITE);
                  myGLCD.setFont(SevenSegNumFont);
                  myGLCD.print(newtemp, CENTER, 55);
                  setTemp = atoi(newtemp);
                  Serial.println(setTemp);
                }
              }
      
              else if(touchLocations[i].x>=505&&touchLocations[i].x<=535&&touchLocations[i].y>=60&&touchLocations[i].y<=90 )
              {
                Serial.println("Minus button pressed");
                if(temp>0)
                {
                  temp=temp-1;
                  sprintf(newtemp, "%d", temp);
                  Serial.println(newtemp);
                  myGLCD.setColor(VGA_GRAY);
                  myGLCD.fillRect(340,40,450,110);
                  myGLCD.setBackColor(VGA_GRAY);
                  myGLCD.setColor(VGA_WHITE);
                  myGLCD.setFont(SevenSegNumFont);
                  myGLCD.print(newtemp, CENTER, 55);
                  setTemp = atoi(newtemp);
                  Serial.println(setTemp);
                }
             }
             ////// start of paste
              else if(touchLocations[i].x>=265&&touchLocations[i].x<=305&&touchLocations[i].y>=303&&touchLocations[i].y<=333 )
              {
               Serial.println("Plus button pressed");
               minutes1=minutes1+1;
                 if( minutes1>9)
                 {
                    minutes1=0;
                    minutes2=minutes2+1;  
                 }
      
                 if(minutes2>5){
                    minutes1=0;
                   minutes2=0;
                   hours=hours+1;
                 }
                sprintf(newmins1, "%d", minutes1);
                sprintf(newmins2, "%d", minutes2);
                sprintf(newhours, "%d", hours);
                
                //Serial.println(newmins1);
                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newmins1, 440, SET_TIME_Y);
                
                //Serial.println(newmins2);
                myGLCD.setColor(VGA_GRAY);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.print(newmins2, 410, SET_TIME_Y);
                myGLCD.fillCircle (390,315, 4);
                myGLCD.fillCircle (390,325, 4);
                                               
                //Serial.println(newhours);
                myGLCD.setColor(VGA_GRAY);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.print(newhours, 340, SET_TIME_Y);
                               
                Hours = atoi(newhours);
                Minutes1 = atoi(newmins2);
                Minutes2 = atoi(newmins1);
                Minutes = (10 * Minutes1)+Minutes2;
                TotalTime = ((Hours*60) + Minutes); 
                TotalTime2 = ((Hours*60) + Minutes); 
                Serial.println(TotalTime);

                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newmins1, 440, TIME_REMAINING_Y);
                
                //Serial.println(newmins2);
                myGLCD.setColor(VGA_GRAY);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newmins2, 410, TIME_REMAINING_Y);
                myGLCD.fillCircle (390,315, 4);
                myGLCD.fillCircle (390,325, 4);
                                
                //Serial.println(newhours);
                myGLCD.setColor(VGA_GRAY);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newhours, 340, TIME_REMAINING_Y);
              }
            
              else if(touchLocations[i].x>=505&&touchLocations[i].x<=535&&touchLocations[i].y>=303&&touchLocations[i].y<=333 )
              {
               Serial.println("Minus button pressed");
               minutes1=minutes1-1;
                   
                if(minutes1<0){
                   minutes1=9;
                   minutes2=minutes2-1;
                   
                  if(minutes2<0){
                     minutes2=5;
                     hours = hours - 1;
                   
                    if(hours<0){
                       minutes1=0;
                       minutes2=0;
                       hours=0;
                     }
                   }
                }
                
                sprintf(newmins1, "%d", minutes1);
                sprintf(newmins2, "%d", minutes2);
                sprintf(newhours, "%d", hours);
                     
                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newmins1, 440, SET_TIME_Y);
                
                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newmins2, 410, SET_TIME_Y);
                myGLCD.fillCircle (390,315, 4);
                myGLCD.fillCircle (390,325, 4);
                                
                //Serial.println(newhours);
                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newhours, 340, SET_TIME_Y);
                Hours = atoi(newhours);
                Minutes1 = atoi(newmins2);
                Minutes2 = atoi(newmins1);
                Minutes = (10 * Minutes1)+Minutes2;
                TotalTime = ((Hours*60) + Minutes); 
                TotalTime2 = ((Hours*60) + Minutes); 
                //Serial.println(TotalTime);

                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newmins1, 440, 410);
                
                //Serial.println(newmins2);
                myGLCD.setColor(VGA_GRAY);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newmins2, 410, TIME_REMAINING_Y);
                myGLCD.fillCircle (390,315, 4);
                myGLCD.fillCircle (390,325, 4);
                                
                //Serial.println(newhours);
                myGLCD.setColor(VGA_GRAY);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newhours, 340, TIME_REMAINING_Y);
              }    
              ///
              /// end of paste
              else if(touchLocations[i].x>=610 && touchLocations[i].x<=750 && touchLocations[i].y>=60 && touchLocations[i].y<=100 )
              {
                myGLCD.setColor(VGA_GRAY);
                myGLCD.fillRect(30,340,200,390);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setFont(BigFont);
                Serial.println("Start button pressed");
                Start_flag = 1;
                process1();
               
              }
              else if(touchLocations[i].x>=610 && touchLocations[i].x<=750 && touchLocations[i].y>=165 && touchLocations[i].y<=205 )
              {
                myGLCD.setColor(VGA_GRAY);
                myGLCD.fillRect(30,340,200,390);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setFont(BigFont);
                Serial.println("Start2 button pressed");
                Start_flag = 2;
                process2();
              }
              
              else if(touchLocations[i].x>=610 && touchLocations[i].x<=750 && touchLocations[i].y>=270 && touchLocations[i].y<=310 )
              {
                myGLCD.setColor(VGA_GRAY);
                myGLCD.fillRect(30,340,200,390);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setFont(BigFont);
                Serial.println("Stop button pressed");
                Stop_flag = 1;
              }
              
              else if(touchLocations[i].x>=610 && touchLocations[i].x<=750 && touchLocations[i].y>=375 && touchLocations[i].y<=415 )
              {
                myGLCD.setColor(VGA_GRAY);
                myGLCD.fillRect(30,340,200,390);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setFont(BigFont);
                Serial.println("Off button pressed");
                myGLCD.print("OFFLINE", 60, STATUS_Y);
                screen_off();
              }  
              
            }
          }
        }  
      else
      {
      }
      
      oldAttention = attention;
      unsigned long currentMillis = millis();

      if (Start_flag == 1){
        if (currentMillis - previousMillis >= interval) {
          // save the last time you blinked the LED
          previousMillis = currentMillis;
          TotalTime = TotalTime - 1;
          Serial.println(TotalTime);
          minutes1=minutes1-1;
                         
                      if(minutes1<0){
                         minutes1=9;
                         minutes2=minutes2-1;
                         
                        if(minutes2<0){
                           minutes2=5;
                           hours = hours - 1;
                         
                          if(hours<0){
                             minutes1=0;
                             minutes2=0;
                             hours=0;
                           }
                         }
                      }
                      
                      sprintf(newmins1, "%d", minutes1);
                      sprintf(newmins2, "%d", minutes2);
                      sprintf(newhours, "%d", hours);
                           
                      //Serial.println(newmins1);
                      myGLCD.setBackColor(VGA_GRAY);
                      myGLCD.setColor(VGA_WHITE);
                      myGLCD.setFont(SevenSegNumFont);
                      myGLCD.print(newmins1, 440, TIME_REMAINING_Y);
                      
                      //Serial.println(newmins2);
                      myGLCD.setColor(VGA_GRAY);
                      myGLCD.setColor(VGA_WHITE);
                      myGLCD.setFont(SevenSegNumFont);
                      myGLCD.print(newmins2, 410, TIME_REMAINING_Y);
                      myGLCD.fillCircle (390,315, 4);
                      myGLCD.fillCircle (390,325, 4);
                                            
                      //Serial.println(newhours);
                      myGLCD.setColor(VGA_GRAY);
                      myGLCD.setColor(VGA_WHITE);
                      myGLCD.setFont(SevenSegNumFont);
                      myGLCD.print(newhours, 340, TIME_REMAINING_Y);
      
      
                     if (TotalTime == 0){
                        digitalWrite(ULTRASOUND_PIN, LOW); //Turn on ultrasound
                        digitalWrite(COOLING_PIN, LOW);    //Turn on cooling
                        myGLCD.setColor(VGA_WHITE);
                        myGLCD.setBackColor(VGA_GRAY);
                        myGLCD.setFont(BigFont);
                        myGLCD.print("P1 COMPLETE", STATUS_X, STATUS_Y);
                        delay(5000);
                        myGLCD.print("START P2   ", STATUS_X, STATUS_Y); 
                        Start_flag = 0;
                        TotalTime = 0;
                      }
          }
    }

if (Start_flag == 2){
  Input = analogRead(TEMP_PIN);
  myPID.Compute();

  /************************************************
   * turn the output pin on/off based on pid output
   ***********************************************/
      if (millis() - windowStartTime > WindowSize)
      { //time to shift the Relay Window
        windowStartTime += WindowSize;
      }
  if (currentMillis - previousMillis2 >= interval) {
      previousMillis2 = currentMillis;
      TotalTime2 = TotalTime2 - 1;
      Serial.println(TotalTime2);
      
      minutes1=minutes1-1;
                     
      if(minutes1<0){
         minutes1=9;
         minutes2=minutes2-1;
                     
         if(minutes2<0){
            minutes2=5;
            hours = hours - 1;
                     
            if(hours<0){
               minutes1=0;
               minutes2=0;
               hours=0;
             }
           }
         }
                  
         sprintf(newmins1, "%d", minutes1);
         sprintf(newmins2, "%d", minutes2);
         sprintf(newhours, "%d", hours);
                       
         //Serial.println(newmins1);
         myGLCD.setBackColor(VGA_GRAY);
         myGLCD.setColor(VGA_WHITE);
         myGLCD.setFont(SevenSegNumFont);
         myGLCD.print(newmins1, 440, TIME_REMAINING_Y);
                  
         //Serial.println(newmins2);
         myGLCD.setBackColor(VGA_GRAY);
         myGLCD.setColor(VGA_WHITE);
         myGLCD.setFont(SevenSegNumFont);
         myGLCD.print(newmins2, 410, TIME_REMAINING_Y);
         myGLCD.fillCircle (390,315, 4);
         myGLCD.fillCircle (390,325, 4);
                                   
         //Serial.println(newhours);
         myGLCD.setBackColor(VGA_GRAY);
         myGLCD.setColor(VGA_WHITE);
         myGLCD.setFont(SevenSegNumFont);
         myGLCD.print(newhours, 340, TIME_REMAINING_Y);
  
         if (TotalTime2 == 0){
             myGLCD.setFont(BigFont);
             myGLCD.print("P2 COMPLETE", STATUS_X, STATUS_Y);
             delay(5000);
             myGLCD.print("ALCOHOL AGING", 20, 320);
             myGLCD.print("IS COMPLETE", 20, 340);  
             Start_flag = 0;
             }
  }
}

if (Stop_flag == 1){
    Start_flag=0;
    myGLCD.setFont(BigFont);
    myGLCD.setColor(VGA_WHITE);
    myGLCD.print("  STOPPED", 40, STATUS_Y);
   
   if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    process_stop();
                     
      if(minutes1<0){
         minutes1=9;
         minutes2=minutes2-1;
                   
         if(minutes2<0){
            minutes2=5;
            hours = hours - 1;
                   
            if(hours<0){
               minutes1=0;
               minutes2=0;
               hours=0;
              }
           }
        }
                
     sprintf(newmins1, "%d", minutes1);
     sprintf(newmins2, "%d", minutes2);
     sprintf(newhours, "%d", hours);
                     
     myGLCD.setBackColor(VGA_GRAY);
     myGLCD.setColor(VGA_WHITE);
     myGLCD.setFont(SevenSegNumFont);
     myGLCD.print(newmins1, 440, TIME_REMAINING_Y);
                
     myGLCD.setBackColor(VGA_GRAY);
     myGLCD.setColor(VGA_WHITE);
     myGLCD.setFont(SevenSegNumFont);
     myGLCD.print(newmins2, 410, TIME_REMAINING_Y);
     myGLCD.fillCircle (390,315, 4);
     myGLCD.fillCircle (390,325, 4);
             
     myGLCD.setBackColor(VGA_GRAY);
     myGLCD.setColor(VGA_WHITE);
     myGLCD.setFont(SevenSegNumFont);
     myGLCD.print(newhours, 340, TIME_REMAINING_Y);
    }
    Stop_flag=0;
   }
  
   if (millis() - previousMillis3 >= 5000) {
      previousMillis3 = currentMillis;
      readTemp();
 }
   }
}

float Thermistor(int RawADC) {
  long Resistance;  
  float Temp;  // Dual-Purpose variable to save space.

  Resistance=pad*((1024.0 / RawADC) - 1); 
  Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      
 // Uncomment this line for the function to return Fahrenheit instead.
  //temp = (Temp * 9.0)/ 5.0 + 32.0;                  // Convert to Fahrenheit
  return Temp;                                      // Return the Temperature
}

void readTemp() 
{
  float temp;
  temp=Thermistor(analogRead(TEMP_PIN));       // read ADC and  convert it to Celsius
  currentTemp = (temp * 9.0)/ 5.0 + 32.0;                  // converts to  Fahrenheit
  Serial.print(", Fahrenheit: "); 
  Serial.print(currentTemp);                             // display  Fahrenheit
  Serial.println("");  
  sprintf(newtemp2, "%d", currentTemp);                                                                     // Delay a bit... 
  myGLCD.setFont(SevenSegNumFont);
   myGLCD.setBackColor(VGA_GRAY);
  myGLCD.print(newtemp2, CENTER, 155);
}

void process1(void){
  //readTemp();
  if (Start_flag == 1){
    //Start timer    
      if(TotalTime > 1){
        myGLCD.print("RUNNING P1", STATUS_X, STATUS_Y);
        if(currentTemp >75){
          digitalWrite(ULTRASOUND_PIN, LOW); //Turn on ultrasound
          digitalWrite(COOLING_PIN, LOW);    //Turn on cooling  
          //PAUSE TIMER
          myGLCD.setColor(255,0,0);
          myGLCD.print("TEMP ERROR", STATUS_X, ERROR_Y);
        }
        else if (currentTemp <=70){
        digitalWrite(ULTRASOUND_PIN, HIGH); //Turn on ultrasound
        digitalWrite(COOLING_PIN, HIGH);    //Turn on cooling    
        }
      }
  }
}


//PROCESS2----------------------------------------------------------------------------------//
// This function starts process 2.
void process2(void){
  //readTemp();
  //start timer
  if (Start_flag == 2){
        if(TotalTime2 > 1){
          myGLCD.print("RUNNING P2", STATUS_X, STATUS_Y);
          if (Output < millis() - windowStartTime){
            //digitalWrite(RELAY_PIN, HIGH);
            digitalWrite(HEATING_PIN,HIGH);
            Serial.print("Heating System On"); 
            Serial.println(currentTemp);
          }
          else {
            digitalWrite(HEATING_PIN, LOW);
            if((currentTemp >= (setTemp - 20)) && (currentTemp < (setTemp + 10))){
            digitalWrite(COOLING_PIN,HIGH);
            Serial.print("Cooling System On");
            }
            else if (currentTemp < setTemp)
            {
            digitalWrite(COOLING_PIN,LOW);
            Serial.print("Cooling System Off");
            }
          }
        }
   }
}

// PROCESS3 ----------------------------------------------------------------------------------//
// This function starts process 3.
void process3(void){
      myGLCD.print("Aging Complete", 60, STATUS_Y); 
      Start_flag = 0;
      stage = 0;     
}

// STOP ----------------------------------------------------------------------------------//
// This function STOPS are ongoing processes.
void process_stop(void){
      digitalWrite(ULTRASOUND_PIN, LOW); //Turn on ultrasound
      digitalWrite(COOLING_PIN, LOW);    //Turn on cooling
      digitalWrite(HEATING_PIN, LOW); //Turn on ultrasound
}

void screen_off(void){
  digitalWrite(ULTRASOUND_PIN, LOW); //Turn on ultrasound
  digitalWrite(COOLING_PIN, LOW);    //Turn on cooling
  digitalWrite(HEATING_PIN, LOW); //Turn on ultrasound
  myGLCD.setColor(VGA_BLACK);
  myGLCD.fillRect(0, 0, 799, 479);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(VGA_BLACK);
  myGLCD.setBackColor(VGA_TRANSPARENT);
  myGLCD.setColor(VGA_WHITE);

  myGLCD.print("AEVA LABS", CENTER, 200);  // First page 
  delay(5000);
  myGLCD.clrScr(); 
   digitalWrite(BACKLIGHT, LOW);//oFF
}

