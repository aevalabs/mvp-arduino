/*////////////////////////////////////////////////////////////////
// ------------------------------------------------------------
// AEVA LAB CODE
// DETAILS: MVP
// ------------------------------------------------------------
///////////////////////////////////////////////////////////////*/

#define TEMP_PIN            A0
#define ULTRASOUND_PIN      2
#define HEATING_PIN         3
#define COOLING_PIN         4
#define DECODE_FLAG         0x01
#define TREAD_FLAG          0x02
#define TMONITOR_FLAG       0x04
#define STATUS_FLAG         0x08
#define END_OF_TEST_FLAG    0x10
#define FT5206_WAKE 11
#define FT5206_INT   48  

#include <stdint.h>
#include <UTFT.h>
#include <SPI.h>
#include <Wire.h>
#include <URTouch.h>
//#include <EEPROM.h>
#include "Countimer.h"

/*uint8_t tempOfSystem = 860; // Room Temp*/
uint8_t i=0;
uint8_t temp = 0;
int8_t minutes1 = 0;
int8_t minutes2 = 3;
int8_t hours = 1;
uint8_t flag = 1;
uint8_t Start_flag = 0;
uint8_t Stop_flag = 0;
int runTimeTimerCount = 0;

int tempLimit = 468; // - digital limit calculated for temperature control. 
int tempLimitAnalog = 75; // - analog limit for informing users.
int currentTemp = 0; // 0-1023, not stored as F value.
volatile char operationFlag = 0;
volatile int timeRemaining = 240; // in mins
volatile int secToMin = 0;
volatile int setTemp=0;
volatile int Hours=0;
volatile int Minutes=0;
volatile int Seconds=0;
volatile int TotalTime=0;
volatile int stage=0;

char cmdStr[11]; //Commands can be up to 10 letters/number longs
char newtemp[10];
char newmins1[10];
char newmins2[10];
char newhours[10];
Countimer timer;

volatile int val2 = 0;
//SoftwareSerial wifiSerial(7,8); //RX, TX
// Wifi chip needs a new line character after commands
uint8_t addr  = 0x38;  //CTP IIC ADDRESS

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t BigFont[];
// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41



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
  pinMode(8, OUTPUT);  //backlight 
  digitalWrite(8, HIGH);//on
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
  

  // Set up count down timer with 10s and call method onComplete() when timer is complete.
  // 00h:00m:10s
  timer.setCounter(0, 0, TotalTime, timer.COUNT_DOWN, onComplete);

  // Print current time every 1s on serial port by calling method refreshClock().
  timer.setInterval(refreshClock, 1000);
}

// MAIN WHILE LOOP
// -------------------------------------------------------------------------------------------------------------------  
void loop()
{ 
  Serial.print("Sys: Online");
  
  StartUpScreen(); 

  if(Start_flag == 1){
    if(stage == 1){
      process1(); 
    }
    else if(stage == 2){
      process2();  
    }
    else if(stage == 3){
      process3();  
    }
  }

  if(Stop_flag == 1){
    process_stop(); 
  }
  
  // Run timer
  timer.run();
  
  // Now timer is running and listening for actions.
  // If you want to start the timer, you have to call start() method.
  if(!timer.isCounterCompleted()) {
    timer.start(); 
    }
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
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(0, 0, 799, 479);
  myGLCD.setColor(0, 0, 0);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(255, 255, 255);
  myGLCD.print("AEVA LABS", CENTER, 200);  // First page
  delay(1000);
                 
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
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(BigFont);
  myGLCD.print("CUR. STATUS:", 30, 270); // Set temp:
  myGLCD.print("STAND-BY", 60, 360);

  ////////////////////////////// STOP STATUS /////////////////////////
  
  /////////////////////////////// START TEMP //////////////////////////
  myGLCD.setBackColor(VGA_GRAY); 
  
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(BigFont);
  myGLCD.print("SET INTENSITY:", CENTER, 20); // Set temp:
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print("0", CENTER, 55); //set temp value
  myGLCD.setFont(BigFont);
  myGLCD.print("CURRENT TEMP:", CENTER, 120);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print("72", CENTER, 155);
  
  myGLCD.setBackColor(VGA_TRANSPARENT);
  
  myGLCD.setColor(169,169,169);
  myGLCD.fillCircle(280,75,15);
  myGLCD.setColor(0,0,0);
  myGLCD.drawCircle(280,75,15);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(BigFont);
  myGLCD.print("+", 272, 68);   //button that increases the temp

  myGLCD.setColor(169,169,169);
  myGLCD.fillCircle(520, 75, 15);
  myGLCD.setColor(0,0,0);
  myGLCD.drawCircle(520, 75, 15);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("-", 512, 68);   //button that decreases the temp
  /////////////////////////////// END TEMP //////////////////////////

  /////////////////////////////// START TIME //////////////////////////
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(BigFont);
  myGLCD.print("SET TIME:", CENTER, 260);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print("0", 340, 300);
  myGLCD.fillCircle (390,315, 4);
  myGLCD.fillCircle (390,325, 4);
  myGLCD.print("00", 410, 300);
  
  myGLCD.setFont(BigFont);
  myGLCD.print("TIME REMAINING:", CENTER, 380);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print("0", 340, 420);
  //colon
  myGLCD.fillCircle (390, 435, 4);
  myGLCD.fillCircle (390, 445, 4);
  myGLCD.print("00", 410, 420);

  myGLCD.setBackColor(VGA_TRANSPARENT);
  myGLCD.setColor(169,169,169);
  myGLCD.fillCircle(280,318,15);
  myGLCD.setColor(0,0,0);
  myGLCD.drawCircle(280,318,15);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(BigFont);
  myGLCD.print("+", 272, 311);   //button that reduces the time

  myGLCD.setColor(169,169,169);
  myGLCD.fillCircle(520, 318, 15);
  myGLCD.setColor(0,0,0);
  myGLCD.drawCircle(520, 318, 15);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("-", 512, 311);   //button that increases the time
 
  /////////////////////////////// END TIME //////////////////////////

  /////////////////////////////// START BUTTONS //////////////////////////

  myGLCD.setColor(VGA_BLACK);
  myGLCD.drawRoundRect(609, 59, 751, 101);
  myGLCD.drawRoundRect(609, 219, 751, 261);
  myGLCD.drawRoundRect(609, 379, 751, 421);

  myGLCD.setColor(VGA_GREEN);
  myGLCD.fillRoundRect(610, 60, 750, 100);
  myGLCD.setBackColor(VGA_GREEN);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(BigFont);
  myGLCD.print("START", 640, 75); //start button
  
  myGLCD.setColor(VGA_RED);
  myGLCD.fillRoundRect(610, 220, 750, 260);
  myGLCD.setBackColor(VGA_RED);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(BigFont);
  myGLCD.print("STOP",650, 235); //stop button
    
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRoundRect(610, 380, 750, 420);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(BigFont);
  myGLCD.print("OFF",660, 395); //off button

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
                if(temp<100)
                {
                  temp=temp+1;
                  sprintf(newtemp, "%d", temp);
                  Serial.println(newtemp);
                  myGLCD.setColor(VGA_GRAY);
                  myGLCD.fillRect(340,40,450,110);
                  myGLCD.setBackColor(VGA_GRAY);
                  myGLCD.setColor(255,255,255);
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
                  myGLCD.setColor(255,255,255);
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
                
                Serial.println(newmins1);
                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setColor(255,255,255);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newmins1, 440, 300);
                
                Serial.println(newmins2);
                myGLCD.setColor(VGA_GRAY);
                myGLCD.setColor(255,255,255);
                myGLCD.print(newmins2, 410, 300);
                myGLCD.setColor(255,255,255);
                myGLCD.fillCircle (390,315, 4);
                myGLCD.setColor(255,255,255);
                myGLCD.fillCircle (390,325, 4);
                myGLCD.setColor(255,255,255);
                               
                Serial.println(newhours);
                myGLCD.setColor(VGA_GRAY);
                myGLCD.setColor(255,255,255);
                myGLCD.print(newhours, 340, 300);
                convert_to_seconds();
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
                     
                Serial.println(newmins1);
                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setColor(255,255,255);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newmins1, 440, 300);
                
                Serial.println(newmins2);
                myGLCD.setColor(VGA_GRAY);
                myGLCD.setColor(255,255,255);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newmins2, 410, 300);
                myGLCD.setColor(255,255,255);
                myGLCD.fillCircle (390,315, 4);
                myGLCD.setColor(255,255,255);
                myGLCD.fillCircle (390,325, 4);
                myGLCD.setColor(255,255,255);
                
                Serial.println(newhours);
                myGLCD.setColor(VGA_GRAY);
                myGLCD.setColor(255,255,255);
                myGLCD.setFont(SevenSegNumFont);
                myGLCD.print(newhours, 340, 300);
                convert_to_seconds();
              }    
              ////// end of paste
              else if(touchLocations[i].x>=610 && touchLocations[i].x<=750 && touchLocations[i].y>=60 && touchLocations[i].y<=100 )
              {
                myGLCD.setColor(VGA_GRAY);
                myGLCD.fillRect(30,340,200,390);
                myGLCD.setColor(255,255,255);
                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setFont(BigFont);
                Serial.println("Start button pressed");
                Start_flag = 1;
                stage = 1;
                myGLCD.print("RUNNING", 60, 360);
              }
              
              else if(touchLocations[i].x>=610 && touchLocations[i].x<=750 && touchLocations[i].y>=220 && touchLocations[i].y<=260 )
              {
                myGLCD.setColor(VGA_GRAY);
                myGLCD.fillRect(30,340,200,390);
                myGLCD.setColor(255,255,255);
                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setFont(BigFont);
                Serial.println("Stop button pressed");
                Stop_flag = 1;
                myGLCD.print("STAND-BY", 60, 360);
              }
              
              else if(touchLocations[i].x>=610 && touchLocations[i].x<=750 && touchLocations[i].y>=380 && touchLocations[i].y<=420 )
              {
                myGLCD.setColor(VGA_GRAY);
                myGLCD.fillRect(30,340,200,390);
                myGLCD.setColor(255,255,255);
                myGLCD.setBackColor(VGA_GRAY);
                myGLCD.setFont(BigFont);
                Serial.println("Off button pressed");
                myGLCD.print("OFFLINE", 60, 360);
              }  
            }
          }
        }  
      else
      {
      }
      
      oldAttention = attention;
    }
}

// Thermister --------------------------------------------------------------------------------//
// This function reads the value from the thermister and sends it to the read temp function
double Thermister(int RawADC) {  //Function to perform the fancy math of the Steinhart-Hart equation
 double Temp;
 Temp = log(((10240000/RawADC) - 10000));
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;              // Convert Kelvin to Celsius
 Temp = (Temp * 9.0)/ 5.0 + 32.0; // Celsius to Fahrenheit - comment out this line if you need Celsius
 return Temp;
}

// ReadTemp --------------------------------------------------------------------------------//
// This function reads the current temperature and converts it into farnheit.
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
  val2 = (tempRead2+tempRead3+tempRead4+tempRead5)/4;
  currentTemp=Thermister(val2);   //Runs the fancy math on the raw analog value
  Serial.print("Temp read: ");
  Serial.println(currentTemp);
  operationFlag |= TREAD_FLAG; //turn off temp flag
  //terminal.flush();
}

// ISR 
//----------------------------------------------------------------------------------//
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

// PROCESS1----------------------------------------------------------------------------------//
// This function starts process 1.
void process1(void){
  readTemp();
  if (Start_flag == 1){
    //Start timer
    if(currentTemp >75){
      digitalWrite(ULTRASOUND_PIN, LOW); //Turn on ultrasound
      digitalWrite(COOLING_PIN, LOW);    //Turn on cooling  
      //PAUSE TIMER
      myGLCD.print("CAUTION:TEMP EXCEDED RANGE", 60, 360);
    }
    else if (currentTemp <=70){
    digitalWrite(ULTRASOUND_PIN, HIGH); //Turn on ultrasound
    digitalWrite(COOLING_PIN, HIGH);    //Turn on cooling    
    }

   /* if (  TotalTime == 0){
      digitalWrite(ULTRASOUND_PIN, LOW); //Turn on ultrasound
      digitalWrite(COOLING_PIN, LOW);    //Turn on cooling
      myGLCD.print("Process 1 complete", 60, 360);
      delay(5000);
       myGLCD.print("Starting Process 2", 60, 360); 
       Start_flag = 0;
    }*/
    Start_flag = 0;
    stage = 2;
  }
}

//PROCESS2----------------------------------------------------------------------------------//
// This function starts process 2.
void process2(void){
  readTemp();
  //start timer
  if (Start_flag == 1){
  digitalWrite(HEATING_PIN,HIGH);
  Serial.print("Heating System On"); 
  
    if((currentTemp >= (setTemp - 20)) && (currentTemp < (setTemp + 10))){
    digitalWrite(COOLING_PIN,HIGH);
    Serial.print("Cooling System On");
    }
    else if (currentTemp < setTemp)
    {
    digitalWrite(COOLING_PIN,LOW);
    Serial.print("Cooling System Off");
    }
    
/* if (TotalTime == 0){
   digitalWrite(HEATING_PIN,LOW);
   digitalWrite(COOLING_PIN, LOW);    //Turn on cooling
   myGLCD.print("Process 2 complete", 60, 360);
   Start_flag = 0;
}*/
  }
  Start_flag = 0;
  stage = 3;  
}

// PROCESS3 ----------------------------------------------------------------------------------//
// This function starts process 3.
void process3(void){
      myGLCD.print("Aging Complete", 60, 360); 
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

// TIMER FUNCTION ----------------------------------------------------------------------------------//
// ------------------------------------------------------------------------------------------------//
void refreshClock() {
  Serial.print("Current count time is: ");
    Serial.println(timer.getCurrentTime());
}

void onComplete() {
  Serial.println("Complete!!!");
}

void convert_to_seconds(void){
  Hours = atoi(newhours);
  Minutes = atoi(newmins2);
  Seconds = atoi(newmins1);
  TotalTime = ((Hours*3600) + (Minutes*60) + Seconds); 
}

