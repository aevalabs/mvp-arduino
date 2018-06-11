

/*////////////////////////////////////////////////////////////////
// ------------------------------------------------------------
// AEVA LAB CODE
// DETAILS: MVP-V6
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
uint8_t temp2 = 0;
uint8_t flag = 1;
uint8_t Start_flag = 0;
uint8_t P1_flag = 0;
uint8_t P2_flag = 0;
uint8_t Stop_flag = 0;

int8_t minutes1 = 0;
int8_t minutes2 = 0;
int8_t hours = 0;
int8_t minutes3 = 0;
int8_t minutes4 = 0;
int8_t hours2 = 0;
int8_t Max_minutes = 0;
int8_t Max_minutes2 = 0;
int8_t Max_hours = 0;

int runTimeTimerCount = 0;
int tempLimit = 468; // - digital limit calculated for temperature control. 
int tempLimitAnalog = 75; // - analog limit for informing users.
int currentTemp = 0; // 0-1023, not stored as F value.

volatile char operationFlag = 0;
volatile int timeRemaining = 240; // in mins
volatile int secToMin = 0;
volatile int setTemp=0;
volatile int setTemp2=0;
volatile int Hours=0;
volatile int Total_Minutes=0;
volatile int Total_Minutes2=0;

volatile int Minutes1=0;
volatile int Minutes2=0;
volatile int Hours2=0;
volatile int Minutes3=0;
volatile int Minutes4=0;

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
  //Serial.print("periodMonitor = ");
  //Serial.println( periodMonitor, HEX );
  
  uint8_t  lenLibVersion = readFT5206TouchAddr(0x0a1, buf, 2 );
  if (lenLibVersion)
  {
    uint16_t libVersion = (buf[0] << 8) | buf[1];
    Serial.print("lib version = ");
    Serial.println( libVersion, HEX);
  }
  else
  {
    //Serial.println("lib version length is zero");
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
  page=1;
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
 
Home_Page();

 while(flag)
   {  
      
      attention = digitalRead(FT5206_INT);
      
       /* Wait around for touch events */
      if (!attention && oldAttention ) 
      {   
        //Serial.println("Touch: ");
        uint8_t count = readFT5206TouchLocation( touchLocations, 5 );
  
         if (count)
          {
            static TouchLocation lastTouch = touchLocations[0];
            /*
            Serial.print("Time delta = ");
            Serial.print(thisTouchTime - lastTouchTime);
            Serial.print(", dist = ");
            Serial.println( dist(touchLocations[0],lastTouch) );
            */
            lastTouch = touchLocations[0];
            
            for (i = 0; i < count; i++)
            {
              if(page==1){
                if(touchLocations[i].x>=280&&touchLocations[i].x<=480&&touchLocations[i].y>=300&&touchLocations[i].y<=380 )
                {
                  Configure_Extraction();        
                }
              } 
              
               else if(page==2){
                 if(touchLocations[i].x>=0&&touchLocations[i].x<=100&&touchLocations[i].y>=55&&touchLocations[i].y<=85 )
                 {             
                    Home_Page();
                    page=1; 
                 }
                 else if(touchLocations[i].x>=600&&touchLocations[i].x<=800&&touchLocations[i].y>=380&&touchLocations[i].y<=400 )
                  {
                  Configure_Maturity();
                  page=3;             
                  }
                 else if(touchLocations[i].x>=490&&touchLocations[i].x<=540&&touchLocations[i].y>=190&&touchLocations[i].y<=230 )
                  {    
                  Temperature_Increase1();//(260, 200, 290, 230);
                  }
                  else if(touchLocations[i].x>=250&&touchLocations[i].x<=290&&touchLocations[i].y>=200&&touchLocations[i].y<=230 )
                  {    
                  Temperature_Decrease1();//(490, 200, 520, 230);
                  }
                  
                  else if(touchLocations[i].x>=500&&touchLocations[i].x<=550&&touchLocations[i].y>=320&&touchLocations[i].y<=370 )
                  {    
                  Time_Increase1(); //(260, 320, 310, 370); (500, 320, 550, 370);
                  }
                  else if(touchLocations[i].x>=250&&touchLocations[i].x<=290&&touchLocations[i].y>=320&&touchLocations[i].y<=370 )
                  {    
                  Time_Decrease1();
                  }//40, 250, 180, 300
                 /* else if(touchLocations[i].x>=40&&touchLocations[i].x<=180&&touchLocations[i].y>=250&&touchLocations[i].y<=300 )
                  {    
                  
                  }*/
               }
               
               else if(page==3){
                 if(touchLocations[i].x>=0&&touchLocations[i].x<=100&&touchLocations[i].y>=55&&touchLocations[i].y<=85 )
                 {             
                    Configure_Extraction(); 
                 }
                 else if(touchLocations[i].x>=600&&touchLocations[i].x<=800&&touchLocations[i].y>=380&&touchLocations[i].y<=400 )
                  {
                  Start_Page();           
                  }
                  else if(touchLocations[i].x>=490&&touchLocations[i].x<=540&&touchLocations[i].y>=190&&touchLocations[i].y<=230 )
                  {    
                  Temperature_Increase2();//(260, 200, 290, 230);
                  }
                  else if(touchLocations[i].x>=250&&touchLocations[i].x<=290&&touchLocations[i].y>=200&&touchLocations[i].y<=230 )
                  {    
                  Temperature_Decrease2();//(490, 200, 520, 230);
                  }
                  else if(touchLocations[i].x>=500&&touchLocations[i].x<=550&&touchLocations[i].y>=320&&touchLocations[i].y<=370 )
                  {    
                  Time_Increase2(); //(260, 320, 310, 370); (500, 320, 550, 370);
                  }
                  else if(touchLocations[i].x>=250&&touchLocations[i].x<=290&&touchLocations[i].y>=320&&touchLocations[i].y<=370 )
                  {    
                  Time_Decrease2();
                  }    
               }

               else if(page==4){
                 if(touchLocations[i].x>=0&&touchLocations[i].x<=100&&touchLocations[i].y>=55&&touchLocations[i].y<=85 )
                 {             
                   Configure_Maturity(); 
                 }
                 else if(touchLocations[i].x>=280&&touchLocations[i].x<=480&&touchLocations[i].y>=300&&touchLocations[i].y<=380 )
                  {
                  Start_flag=1;
                  P1_flag = 1;
                  Process_Page();           
                  }    
               }
               else if(page==5){
                 if(touchLocations[i].x>=10&&touchLocations[i].x<=160&&touchLocations[i].y>=60&&touchLocations[i].y<=120 )
                 { 
                  Stop_flag=1;
                  process_stop();            
                   Home_Page(); 
                 }
                 else if(touchLocations[i].x>=600&&touchLocations[i].x<=800&&touchLocations[i].y>=380&&touchLocations[i].y<=400 )
                  {
                  Completed_Page();            
                  } //550, 290, 700, 350
                   else if(touchLocations[i].x>=550&&touchLocations[i].x<=700&&touchLocations[i].y>=180&&touchLocations[i].y<=240 )
                  { 
                  P1_flag=1;
                  Start_flag=1;       
                  }//550, 180, 700, 240
                  else if(touchLocations[i].x>=550&&touchLocations[i].x<=700&&touchLocations[i].y>=290&&touchLocations[i].y<=350 )
                  {
                   Stop_flag=1;             
                  }
                  
               }
            
               else if(page==6){ //300, 250, 500, 330
                 if(touchLocations[i].x>=300&&touchLocations[i].x<=500&&touchLocations[i].y>=250&&touchLocations[i].y<=330 )
                 {             
                    Home_Page(); 
                 }
               }
             }
            }  
          }
          
           unsigned long currentMillis = millis();
          if (Start_flag == 1){
           
            if (currentMillis - previousMillis >= interval) {
              previousMillis = currentMillis;
              TotalTime = TotalTime - 1;
              Max_minutes=Max_minutes-1;
              Serial.println("Started"); 
              if(Max_minutes<0){
                Max_minutes=9;
                Max_minutes2=Max_minutes2-1;
                
                  if(Max_minutes2<0)
                  {
                  Max_minutes2=5;
                  Max_hours = Max_hours - 1;
                    if(Max_hours<0)
                    {
                    Max_minutes=0;
                    Max_minutes2=0;
                    Max_hours=0;
                    }
                }
              }
                                                          
              sprintf(Max_newmins1, "%d", Max_minutes);
              sprintf(Max_newmins2, "%d", Max_minutes2);
              sprintf(Max_newhours, "%d", Max_hours);
                                                          
              myGLCD.setBackColor(VGA_BACKGROUND);
              myGLCD.setColor(VGA_BLACK);
              myGLCD.setFont(SevenSegNumFont);
              myGLCD.print(Max_newmins1, 400, 285);
                                                                          
              myGLCD.print(Max_newmins2, 370, 285);
              myGLCD.fillCircle (360,300, 4);
              myGLCD.fillCircle (360,320, 4);
                                                                                         
              if(Max_hours<10)
              {                                              
                myGLCD.print(Max_newhours, 315, 285);
                myGLCD.setColor(VGA_BACKGROUND);
                myGLCD.fillRoundRect(285, 275, 295, 365);
                myGLCD.setColor(VGA_BLACK);
              }   
              else if(Max_hours>=10)
              {                                              
                myGLCD.print(Max_newhours, 285, 285);
              }  
              Serial.println(TotalTime);     
                                                
                                                
          if (TotalTime == 0)
          {
          Start_flag = 0;
          TotalTime = 0;
          Completed_Page();
          }
         }
        }

        if (P1_flag == 1){
          
          if (currentMillis - previousMillis2 >= interval)
          {
            previousMillis2 = currentMillis;
            P1_TotalTime = P1_TotalTime - 1;   
            Serial.println(P1_TotalTime); 
            if(P1_TotalTime > 1)
            {
            Running_P1();
            }
            else if(P1_TotalTime == 0)
            {/*
              Input = analogRead(TEMP_PIN);
              myPID.Compute();
              
              if (millis() - windowStartTime > WindowSize)
              { //time to shift the Relay Window
                windowStartTime += WindowSize;
              }
              
              if (currentMillis - previousMillis3 >= interval) {
                previousMillis3 = currentMillis;
                P2_TotalTime = P2_TotalTime - 1; 
                  
                if(P2_TotalTime > 1){
                  Running_P2();
                }
                
                else if(P2_TotalTime == 0){
                P2_flag=1;
                }
              }*/
              }
          }
        }

        if (Stop_flag == 1){
          
            Start_flag=0;
            P1_flag=0;
                
            if (currentMillis - previousMillis >= interval) {
              previousMillis = currentMillis;
              Serial.println("STOPPED");
              process_stop();
                
            myGLCD.setBackColor(VGA_BACKGROUND);
            myGLCD.setColor(VGA_BLACK);
            myGLCD.setFont(SevenSegNumFont);
            myGLCD.print(Max_newmins1, 400, 285);
                                                                        
            myGLCD.print(Max_newmins2, 370, 285);
            myGLCD.fillCircle (360,300, 4);
            myGLCD.fillCircle (360,320, 4);
                                                                                       
              if(Max_hours<10)
              {                                              
              myGLCD.print(Max_newhours, 315, 285);
              myGLCD.setColor(VGA_BACKGROUND);
              myGLCD.fillRoundRect(285, 275, 295, 365);
              myGLCD.setColor(VGA_BLACK);
              }   
              else if(Max_hours>=10)
              {                                              
              myGLCD.print(Max_newhours, 285, 285);
              }  
          }
        Stop_flag=0;
        }      
    }
}

void Background(void){
myGLCD.setColor(VGA_ROWS);
myGLCD.fillRect(0, 0, 799, 50); //Sets the background color
myGLCD.setColor(VGA_BACKGROUND);
myGLCD.fillRect(0, 50, 799, 439); //Sets the background color
myGLCD.setColor(VGA_ROWS);
myGLCD.fillRect(0, 439, 799, 489); //Sets the background color        
myGLCD.setBackColor(VGA_BACKGROUND);   
}

void Add_Subtract_Buttons(void){
myGLCD.setFont(BigFont);
myGLCD.setBackColor(VGA_RED);
myGLCD.setColor(VGA_RED);
myGLCD.fillCircle(280,220,20);
myGLCD.setColor(VGA_BLACK);
//myGLCD.fillRoundRect(260, 200, 310, 250);
myGLCD.drawCircle(280,220,20);
myGLCD.setColor(VGA_WHITE);
myGLCD.setFont(BigFont);
myGLCD.print("-", 272, 213);   //button that increases the temp
            
myGLCD.setBackColor(VGA_GREEN);
myGLCD.setColor(VGA_GREEN);
myGLCD.fillCircle(520, 220, 20);
myGLCD.setColor(VGA_BLACK);
//myGLCD.fillRoundRect(500, 200, 550, 250);
myGLCD.drawCircle(520, 220, 20);
myGLCD.setColor(VGA_WHITE);
myGLCD.print("+", 512, 213);   //button that decreases the temp
            
myGLCD.setFont(BigFont);
myGLCD.setBackColor(VGA_RED);
myGLCD.setColor(VGA_RED);
myGLCD.fillCircle(280,350,20);
myGLCD.setColor(VGA_BLACK);
//myGLCD.fillRoundRect(260, 320, 310, 370);
myGLCD.drawCircle(280,350,20);
myGLCD.setColor(VGA_WHITE);
myGLCD.setFont(BigFont);
myGLCD.print("-", 272, 342);   //button that reduces the time
            
myGLCD.setBackColor(VGA_GREEN);
myGLCD.setColor(VGA_GREEN);
myGLCD.fillCircle(520, 350, 20);
myGLCD.setColor(VGA_BLACK);
//myGLCD.fillRoundRect(500, 320, 550, 370);
myGLCD.drawCircle(520, 350, 20);
myGLCD.setColor(VGA_WHITE);
myGLCD.print("+", 512, 342);   //button that increases the time
}

void Home_Page(void){
Background();
myGLCD.setBackColor(VGA_TRANSPARENT);
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(BigFont);
  int drawScreen = 0;
  for(drawScreen=0; drawScreen <= 5; drawScreen++)
  {
  i = drawScreen;
  myGLCD.drawLine(315, 95+i, 400, 180+i); //top V
  myGLCD.drawLine(400, 180+i, 485, 95+i);
  myGLCD.drawLine(370,  130+i,  400, 160+i);
  myGLCD.drawLine(400, 160+i, 430, 130+i);
  
  myGLCD.drawLine(315, 265+i, 400, 180+i); // bottom V
  myGLCD.drawLine(400, 180+i, 485, 265+i);
  myGLCD.drawLine(430, 230+i, 400, 200+i);
  myGLCD.drawLine(400, 200+i, 370, 230+i); 
  }
  
  myGLCD.setFont(Inconsola);
  myGLCD.print("AEVA  LABS", CENTER, 175);
 
  myGLCD.setColor(124, 146, 74);
  myGLCD.fillRoundRect(300, 300, 500, 380);
  myGLCD.setBackColor(124, 146, 74);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(BigFont);
  myGLCD.print("START AGING", 310, 320); //start button
  myGLCD.print("PROCESS", 335, 350); //start button
  page=1;  
}

///////Configure_Extraction
void Configure_Extraction(void){
Background();
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(Arial_round_16x24); 
myGLCD.print("Configure Extraction", CENTER, 80); // Set temp:
myGLCD.setColor(VGA_GRAY);
myGLCD.drawLine(250, 120, 560, 120);
            
myGLCD.setFont(BigFont);
myGLCD.setColor(VGA_BLACK);
myGLCD.print("INTENSITY:", CENTER, 155); // Set temp:
            
myGLCD.setColor(VGA_GRAY);
myGLCD.setBackColor(VGA_GRAY);
myGLCD.fillRoundRect(320, 180, 485, 250);
myGLCD.setFont(SevenSegNumFont);
myGLCD.setColor(VGA_BLACK);
myGLCD.print("0", CENTER, 190);
myGLCD.print(newtemp, CENTER, 190);
Add_Subtract_Buttons();           
myGLCD.setBackColor(VGA_BACKGROUND);
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(BigFont); 
myGLCD.print("LENGTH:", CENTER, 290); // Set temp:
myGLCD.setColor(VGA_GRAY);
myGLCD.setBackColor(VGA_GRAY);
myGLCD.fillRoundRect(320, 315, 485, 385);
myGLCD.setFont(SevenSegNumFont);
myGLCD.setColor(VGA_BLACK);
myGLCD.print("0", 355, 325); //set temp value
myGLCD.print(newhours, 355, 325); //set temp value
myGLCD.fillCircle (400,340, 4);
myGLCD.fillCircle (400,360, 4);
myGLCD.print("0", 440, 325);
myGLCD.print(newmins1, 440, 325);
myGLCD.print("0", 410, 325);
myGLCD.print(newmins2, 410, 325);
           
myGLCD.setColor(120, 170, 179);
myGLCD.fillRoundRect(40, 250, 180, 300);
myGLCD.setBackColor(120, 170, 179);
myGLCD.setColor(VGA_WHITE);
myGLCD.setFont(BigFont);
myGLCD.print("DEFAULT", 50, 265); //start button
                      
myGLCD.setColor(VGA_GREEN);
myGLCD.setBackColor(VGA_BACKGROUND);
myGLCD.setFont(Various_Symbols_32x32);
myGLCD.print("S",740, 390); //off button
            
myGLCD.setFont(BigFont);
myGLCD.setColor(VGA_BLACK);
myGLCD.print("NEXT", 660, 400); //start button

myGLCD.setColor(VGA_GREEN);
myGLCD.setBackColor(VGA_BACKGROUND);
myGLCD.setFont(Various_Symbols_32x32);
myGLCD.print("R",20, 65); //off button
            
myGLCD.setFont(BigFont);
myGLCD.setColor(VGA_BLACK);
myGLCD.print("BACK", 60, 75); //start button 
page=2; 
}

void Configure_Maturity(void){
Background();
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(Arial_round_16x24); 
myGLCD.print("Configure Maturity", CENTER, 80); // Set temp:
myGLCD.setColor(VGA_BLACK);
myGLCD.drawLine(250, 120, 560, 120);
            
myGLCD.setFont(BigFont);
myGLCD.setColor(VGA_BLACK);
myGLCD.print("INTENSITY:", CENTER, 155); // Set temp:
            
myGLCD.setColor(VGA_GRAY);
myGLCD.setBackColor(VGA_GRAY);
myGLCD.fillRoundRect(325, 180, 480, 250);
myGLCD.setFont(SevenSegNumFont);
myGLCD.setColor(VGA_BLACK);
myGLCD.print("0", CENTER, 190);
myGLCD.print(newtemp2, CENTER, 190);
            
Add_Subtract_Buttons();
            
myGLCD.setBackColor(VGA_BACKGROUND);
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(BigFont); 
myGLCD.print("LENGTH:", CENTER, 290); // Set temp:
myGLCD.setColor(VGA_GRAY);
myGLCD.setBackColor(VGA_GRAY);
myGLCD.fillRoundRect(325, 315, 480, 385);
myGLCD.setFont(SevenSegNumFont);
myGLCD.setColor(VGA_BLACK);
myGLCD.print("0", 355, 325); //set temp value
myGLCD.print(newhours2, 355, 325); //set temp value
myGLCD.fillCircle (400,340, 4);
myGLCD.fillCircle (400,360, 4);
myGLCD.print("0", 440, 325);
myGLCD.print(newmins3, 440, 325);
myGLCD.print("0", 410, 325);
myGLCD.print(newmins4, 410, 325);
           
myGLCD.setColor(120, 170, 179);
myGLCD.fillRoundRect(40, 250, 180, 300);
myGLCD.setBackColor(120, 170, 179);
myGLCD.setColor(VGA_WHITE);
myGLCD.setFont(BigFont);
myGLCD.print("DEFAULT", 50, 265); //start button
                        
myGLCD.setColor(VGA_GREEN);
myGLCD.setBackColor(VGA_BACKGROUND);
myGLCD.setFont(Various_Symbols_32x32);
myGLCD.print("S",740, 390); //off button
            
myGLCD.setFont(BigFont);
myGLCD.setColor(VGA_BLACK);
myGLCD.print("NEXT", 660, 400); //start button

myGLCD.setColor(VGA_GREEN);
myGLCD.setBackColor(VGA_BACKGROUND);
myGLCD.setFont(Various_Symbols_32x32);
myGLCD.print("R",20, 65); //off button
            
myGLCD.setFont(BigFont);
myGLCD.setColor(VGA_BLACK);
myGLCD.print("BACK", 60, 75); //start button 
page=3;                         
}

void Start_Page(void){ 
Background();
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(Arial_round_16x24); 
myGLCD.print("Press Start To Run Machine:", CENTER, 175);
  
myGLCD.setColor(164, 220, 141);
myGLCD.fillRoundRect(320, 300, 480, 380);
myGLCD.setBackColor(164, 220, 141);
myGLCD.setColor(VGA_WHITE);
myGLCD.setFont(BigFont);
myGLCD.print("START", CENTER, 330); //start button

myGLCD.setColor(VGA_GREEN);
myGLCD.setBackColor(VGA_BACKGROUND);
myGLCD.setFont(Various_Symbols_32x32);
myGLCD.print("R",20, 65); //off button
            
myGLCD.setFont(BigFont);
myGLCD.setColor(VGA_BLACK);
myGLCD.print("BACK", 60, 75); //start button
page=4;  
}

void Process_Page(void){
Background();
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(Arial_round_16x24); 
myGLCD.print("Aging In Progress", CENTER, 90);
myGLCD.setColor(VGA_GRAY);
myGLCD.drawLine(250, 130, 560, 130);

sprintf(Max_newmins1, "%d", Max_minutes);
sprintf(Max_newmins2, "%d", Max_minutes2);
sprintf(Max_newhours, "%d", Max_hours);
                
myGLCD.setBackColor(VGA_BACKGROUND); 
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(SevenSegNumFont);
myGLCD.print(Max_newmins1, 400, 285);
                
myGLCD.print(Max_newmins2, 370, 285);
myGLCD.fillCircle (360,300, 4);
myGLCD.fillCircle (360,320, 4);
                                               
if(Max_hours<10)
{                                              
myGLCD.print(Max_newhours, 315, 285);
myGLCD.setColor(VGA_BACKGROUND);
myGLCD.fillRoundRect(285, 275, 295, 365);
myGLCD.setColor(VGA_BLACK);
}   
else if(Max_hours>=10)
{                                              
myGLCD.print(Max_newhours, 285, 285);
} 
                               
Hours = atoi(Max_newhours);
Minutes1 = atoi(Max_newmins2);
Minutes2 = atoi(Max_newmins1);
Total_Minutes = (10 * Minutes1)+Minutes2;
TotalTime = ((Hours*60) + Total_Minutes);

//sprintf(Process_Time, "%d", Time_Remaining);
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(Arial_round_16x24); 
myGLCD.print("Time Remaining:", 50, 300); 
myGLCD.print("Current Temp:", 50, 200); 

  if(currentTemp < 60){
  myGLCD.print("NORMAL", 280, 200);  
  }
  else if((currentTemp >= 60)&&(currentTemp < 140)){
  myGLCD.print("NORMAL", 280, 200);  
  }
  else if(currentTemp < 140){
  myGLCD.print("ABOVE NORMAL LIMIT", 280, 200);  
  }
 
myGLCD.drawRoundRect(550, 180, 700, 240);
myGLCD.drawRoundRect(550, 290, 700, 350);

myGLCD.setColor(243, 128, 97);
myGLCD.fillRoundRect(550, 290, 700, 350);
myGLCD.setBackColor(243, 128, 97);
myGLCD.setColor(180, 180, 180);
myGLCD.fillRoundRect(550, 180, 700, 240);
myGLCD.setBackColor(180, 180, 180);
myGLCD.setColor(VGA_WHITE);
myGLCD.setFont(BigFont);
myGLCD.print("RESUME", 580, 200); //start button

myGLCD.setColor(243, 128, 97);
myGLCD.fillRoundRect(550, 290, 700, 350);
myGLCD.setBackColor(243, 128, 97);
myGLCD.setColor(VGA_WHITE);
myGLCD.print("PAUSE", 580, 310); //start button

myGLCD.setColor(243, 128, 97);
myGLCD.fillRoundRect(10, 60, 140, 120);
myGLCD.setBackColor(243, 128, 97);
myGLCD.setColor(VGA_WHITE);
myGLCD.print("CANCEL", 20, 80); //start button

myGLCD.setColor(VGA_GREEN);
myGLCD.setBackColor(VGA_BACKGROUND);
myGLCD.setFont(Various_Symbols_32x32);
myGLCD.print("S",740, 390); //off button
            
myGLCD.setFont(BigFont);
myGLCD.setColor(VGA_BLACK);
myGLCD.print("NEXT", 660, 400); //start button
page=5;  
}


void Completed_Page(void){ 
Background();
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(Inconsola);
myGLCD.print("Aging Complete!", CENTER, 150);
  
myGLCD.setColor(111, 225, 142);
myGLCD.fillRoundRect(270, 250, 530, 330);
myGLCD.setBackColor(111, 225, 142);
myGLCD.setColor(VGA_WHITE);
myGLCD.setFont(BigFont);
myGLCD.print("RETURN TO HOME", CENTER, 270); //start button
myGLCD.print("SCREEN", CENTER, 290); //start button

myGLCD.setBackColor(VGA_BACKGROUND);
myGLCD.setColor(VGA_RED);
myGLCD.setFont(BigFont);
myGLCD.print("Warning:Contents May Be Hot", CENTER, 390); //start button
page=6;  
}

// STOP ----------------------------------------------------------------------------------//
// This function STOPS are ongoing processes.
void process_stop(void){
digitalWrite(ULTRASOUND_PIN, LOW); //Turn on ultrasound
digitalWrite(COOLING_PIN, LOW);    //Turn on cooling
digitalWrite(HEATING_PIN, LOW); //Turn on ultrasound
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
}


void Temperature_Increase1(void){
Serial.println("Plus button pressed");
  if(temp<140)
  {
  temp=temp+1;
  sprintf(newtemp, "%d", temp);
  Serial.println(newtemp);
  myGLCD.setColor(VGA_GRAY);
  myGLCD.fillRoundRect(325, 180, 480, 250);
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print(newtemp, CENTER, 190);
  setTemp = atoi(newtemp);
  Serial.println(setTemp);
  }
}


void Temperature_Decrease1(void){
Serial.println("Minus button pressed");
  if(temp>0)
  {
  temp=temp-1;
  sprintf(newtemp, "%d", temp);
  Serial.println(newtemp);
  myGLCD.setColor(VGA_GRAY);
  myGLCD.fillRoundRect(325, 180, 480, 250);
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print(newtemp, CENTER, 190);
  setTemp = atoi(newtemp);
  Serial.println(setTemp);
  }
}


void Default_Time_Increase1(void){
Serial.println("Plus button pressed");
minutes1=0;
minutes2=3;
hours=5;
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

Max_minutes=Max_minutes+1;
if( Max_minutes>9)
{
Max_minutes=0;
Max_minutes2=Max_minutes2+1;  
}
      
if(Max_minutes2>5){
Max_minutes=0;
Max_minutes2=0;
Max_hours=Max_hours+1;
}

sprintf(newmins1, "%d", minutes1);
sprintf(newmins2, "%d", minutes2);
sprintf(newhours, "%d", hours);
                
myGLCD.setBackColor(VGA_GRAY);
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(SevenSegNumFont);
myGLCD.print(newmins1, 440, 325);
                
myGLCD.print(newmins2, 410, 325);
myGLCD.fillCircle (400,340, 4);
myGLCD.fillCircle (400,360, 4);
                                               
if(hours<10)
{                                              
myGLCD.print(newhours, 355, 325);
myGLCD.setColor(VGA_GRAY);
myGLCD.fillRoundRect(325, 315, 355, 385);
myGLCD.setColor(VGA_BLACK);
}   
else if(hours>=10)
{                                              
myGLCD.print(newhours, 325, 325);
} 
P1_Hours = atoi(newhours);
P1_Minutes1 = atoi(newmins2);
P1_Minutes2 = atoi(newmins1);
P1_Total_Minutes = (10 * P1_Minutes1)+P1_Minutes2;
P1_TotalTime = ((P1_Hours*60) + P1_Total_Minutes);                              
}

void Time_Increase1(void){
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

Max_minutes=Max_minutes+1;
if( Max_minutes>9)
{
Max_minutes=0;
Max_minutes2=Max_minutes2+1;  
}
      
if(Max_minutes2>5){
Max_minutes=0;
Max_minutes2=0;
Max_hours=Max_hours+1;
}

sprintf(newmins1, "%d", minutes1);
sprintf(newmins2, "%d", minutes2);
sprintf(newhours, "%d", hours);
                
myGLCD.setBackColor(VGA_GRAY);
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(SevenSegNumFont);
myGLCD.print(newmins1, 440, 325);
                
myGLCD.print(newmins2, 410, 325);
myGLCD.fillCircle (400,340, 4);
myGLCD.fillCircle (400,360, 4);
                                               
if(hours<10)
{                                              
myGLCD.print(newhours, 355, 325);
myGLCD.setColor(VGA_GRAY);
myGLCD.fillRoundRect(325, 315, 355, 385);
myGLCD.setColor(VGA_BLACK);
}   
else if(hours>=10)
{                                              
myGLCD.print(newhours, 325, 325);
} 
P1_Hours = atoi(newhours);
P1_Minutes1 = atoi(newmins2);
P1_Minutes2 = atoi(newmins1);
P1_Total_Minutes = (10 * P1_Minutes1)+P1_Minutes2;
P1_TotalTime = ((P1_Hours*60) + P1_Total_Minutes);                              
}

void Time_Decrease1(void){
Serial.println("Minus button pressed");
minutes1=minutes1-1;
                   
if(minutes1<0){
   minutes1=9;
   minutes2=minutes2-1;
                   
   if(minutes2<0)
   {
      minutes2=5;
      hours = hours - 1;
                   
      if(hours<0)
      {
         minutes1=0;
         minutes2=0;
         hours=0;
       }
    }
}

Max_minutes=Max_minutes-1;
                   
if(Max_minutes<0){
   Max_minutes=9;
   Max_minutes2=Max_minutes2-1;
                   
   if(Max_minutes2<0)
   {
      Max_minutes2=5;
      Max_hours = Max_hours - 1;
                   
      if(Max_hours<0)
      {
         Max_minutes=0;
         Max_minutes2=0;
         Max_hours=0;
       }
    }
}
                
sprintf(newmins1, "%d", minutes1);
sprintf(newmins2, "%d", minutes2);
sprintf(newhours, "%d", hours);
                
myGLCD.setBackColor(VGA_GRAY);
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(SevenSegNumFont);
myGLCD.print(newmins1, 440, 325);
                
myGLCD.print(newmins2, 410, 325);
myGLCD.fillCircle (400,340, 4);
myGLCD.fillCircle (400,360, 4);

if(hours<10)
{                                              
myGLCD.print(newhours, 355, 325);
myGLCD.setColor(VGA_GRAY);
myGLCD.fillRoundRect(325, 315, 355, 385);
myGLCD.setColor(VGA_BLACK);
}   
else if(hours>=10)
{                                              
myGLCD.print(newhours, 325, 325);
} 
P1_Hours = atoi(newhours);
P1_Minutes1 = atoi(newmins2);
P1_Minutes2 = atoi(newmins1);
P1_Total_Minutes = (10 * P1_Minutes1)+P1_Minutes2;
P1_TotalTime = ((P1_Hours*60) + P1_Total_Minutes);                                            
}


void Temperature_Increase2(void){
Serial.println("Plus button pressed");
  if(temp2<140)
  {
  temp2=temp2+1;
  sprintf(newtemp2, "%d", temp2);
  Serial.println(newtemp2);
  myGLCD.setColor(VGA_GRAY);
  myGLCD.fillRoundRect(325, 180, 480, 250);
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print(newtemp2, CENTER, 190);
  setTemp = atoi(newtemp2);
  Serial.println(setTemp2);
  }
}

void Temperature_Decrease2(void){
Serial.println("Minus button pressed");
  if(temp2>0)
  {
  temp2=temp2-1;
  sprintf(newtemp2, "%d", temp2);
  Serial.println(newtemp2);
  myGLCD.setColor(VGA_GRAY);
  myGLCD.fillRoundRect(325, 180, 480, 250);
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print(newtemp2, CENTER, 190);
  setTemp = atoi(newtemp2);
  Serial.println(setTemp2);
  }
}            

void Time_Increase2(void){
Serial.println("Plus button pressed");
Max_minutes=Max_minutes+1;
if( Max_minutes>9)
{
Max_minutes=0;
Max_minutes2=Max_minutes2+1;  
}
      
if(Max_minutes2>5){
Max_minutes=0;
Max_minutes2=0;
Max_hours=Max_hours+1;
}

minutes3=minutes3+1;
if( minutes3>9)
{
minutes3=0;
minutes4=minutes4+1;  
}
      
if(minutes4>5){
minutes3=0;
minutes4=0;
hours2=hours2+1;
}
sprintf(newmins3, "%d", minutes3);
sprintf(newmins4, "%d", minutes4);
sprintf(newhours2, "%d", hours2);
                
myGLCD.setBackColor(VGA_GRAY);
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(SevenSegNumFont);
myGLCD.print(newmins3, 440, 325);
                
myGLCD.print(newmins4, 410, 325);
myGLCD.fillCircle (400,340, 4);
myGLCD.fillCircle (400,360, 4);
                                               
if(hours2<10)
{                                              
myGLCD.print(newhours2, 355, 325);
myGLCD.setColor(VGA_GRAY);
myGLCD.fillRoundRect(325, 315, 355, 385);
myGLCD.setColor(VGA_BLACK);
}   
else if(hours2>=10)
{                                              
myGLCD.print(newhours2, 325, 325);
}    
P2_Hours = atoi(newhours2);
P2_Minutes1 = atoi(newmins4);
P2_Minutes2 = atoi(newmins3);
P2_Total_Minutes = (10 * P2_Minutes1)+P2_Minutes2;
P2_TotalTime = ((P2_Hours*60) + P2_Total_Minutes);                                      
}

void Time_Decrease2(void){
Serial.println("Minus button pressed");
minutes3=minutes3-1;
                   
if(minutes3<0){
   minutes3=9;
   minutes4=minutes4-1;
                   
   if(minutes4<0)
   {
      minutes4=5;
      hours2 = hours2 - 1;
                   
      if(hours2<0)
      {
         minutes3=0;
         minutes4=0;
         hours2=0;
       }
    }
}

Max_minutes=Max_minutes-1;
                   
if(Max_minutes<0){
   Max_minutes=9;
   Max_minutes2=Max_minutes2-1;
                   
   if(Max_minutes2<0)
   {
      Max_minutes2=5;
      Max_hours = Max_hours - 1;
                   
      if(Max_hours<0)
      {
         Max_minutes=0;
         Max_minutes2=0;
         Max_hours=0;
       }
    }
}
                
sprintf(newmins3, "%d", minutes3);
sprintf(newmins4, "%d", minutes4);
sprintf(newhours2, "%d", hours2);
                
myGLCD.setBackColor(VGA_GRAY);
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(SevenSegNumFont);
myGLCD.print(newmins3, 440, 325);
                
myGLCD.print(newmins4, 410, 325);
myGLCD.fillCircle (400,340, 4);
myGLCD.fillCircle (400,360, 4);
                                               
if(hours2<10)
{                                              
myGLCD.print(newhours2, 355, 325);
myGLCD.setColor(VGA_GRAY);
myGLCD.fillRoundRect(325, 315, 355, 385);
myGLCD.setColor(VGA_BLACK);
}   
else if(hours2>=10)
{                                              
myGLCD.print(newhours2, 325, 325);
}   
P2_Hours = atoi(newhours2);
P2_Minutes1 = atoi(newmins4);
P2_Minutes2 = atoi(newmins3);
P2_Total_Minutes = (10 * P2_Minutes1)+P2_Minutes2;
P2_TotalTime = ((P2_Hours*60) + P2_Total_Minutes);                                 
}

void Running_P1(void){
  // myGLCD.print("RUNNING P1", STATUS_X, STATUS_Y);
  if(currentTemp >75)
  {
     digitalWrite(ULTRASOUND_PIN, LOW); //Turn on ultrasound
     digitalWrite(COOLING_PIN, LOW);    //Turn on cooling  
            //PAUSE TIMER
           // myGLCD.setColor(255,0,0);
           // myGLCD.print("TEMP ERROR", STATUS_X, ERROR_Y);
  }
  else if (currentTemp <=70)
  {
  digitalWrite(ULTRASOUND_PIN, HIGH); //Turn on ultrasound
  digitalWrite(COOLING_PIN, HIGH);    //Turn on cooling    
  }
}

void Running_P2(void){
//myGLCD.print("RUNNING P2", STATUS_X, STATUS_Y);
  if (Output < millis() - windowStartTime){
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

