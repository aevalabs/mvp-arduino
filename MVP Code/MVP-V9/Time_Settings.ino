/**********************************************************************;
* Project           : Aeva Labs Demo Unit
*
* Program name      : Time_Settings.ino (Part of the MVP-V7 code)
*
* Author            : wdcosta
*
* Date created      : 06/15/2018
*
* Purpose           : This code contains functions that increase and decrease
*                     the time using buttons on the UI on the Configure 
*                     Extraction and Configure Maturity pages.
*                     
*                     This code also contains functions that diplay
*                     the total time for which the process will run.
*                     
|**********************************************************************/

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

void Max_Time_Decrease(void){
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
myGLCD.fillRoundRect(285, 275, 315, 365);
myGLCD.setColor(VGA_BLACK);
}   
else if(Max_hours>=10)
{                                              
myGLCD.print(Max_newhours, 285, 285);
}  
}

