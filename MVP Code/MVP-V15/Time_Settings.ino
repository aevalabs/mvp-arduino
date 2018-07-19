/**********************************************************************;
* Project           : Aeva Labs Demo Unit
*
* Program name      : Time_Settings.ino (Part of the MVP-V11 code)
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

void Time_Increase1(void)
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
  myGLCD.setBackColor(VGA_GRAY);
                                               
  if(hours<10)
  {      
     myGLCD.setColor(VGA_BLACK);
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.print(newmins1, 440, 325);               
    myGLCD.print(newmins2, 410, 325);
    myGLCD.fillCircle (400,340, 4);
    myGLCD.fillCircle (400,360, 4);                                        
    myGLCD.print(newhours, 355, 325);
    myGLCD.setColor(VGA_GRAY);
    myGLCD.fillRoundRect(325, 315, 355, 385);
    myGLCD.setColor(VGA_BLACK);
  } 
    
  else if((hours>=10)&&(hours<=99))
  {   
    myGLCD.setColor(VGA_BLACK);
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.print(newmins1, 440, 325);               
    myGLCD.print(newmins2, 410, 325);
    myGLCD.fillCircle (400,340, 4);
    myGLCD.fillCircle (400,360, 4);                                           
    myGLCD.print(newhours, 330, 325);
    myGLCD.setColor(VGA_GRAY);
    myGLCD.fillRoundRect(325, 315, 330, 385);
    myGLCD.setColor(VGA_BLACK);
  } 
  
  else if(hours>99)
  {  
    myGLCD.setColor(VGA_BLACK);
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.print(newmins1, 440, 325);               
    myGLCD.print(newmins2, 410, 325);
    myGLCD.fillCircle (400,340, 4);
    myGLCD.fillCircle (400,360, 4);                                            
    myGLCD.print(newhours, 330, 325);
  }
  
  P1_Hours = atoi(newhours);
  P1_Minutes1 = atoi(newmins2);
  P1_Minutes2 = atoi(newmins1);
  P1_Total_Minutes = (10 * P1_Minutes1)+P1_Minutes2;
  P1_TotalTime = ((P1_Hours*60) + P1_Total_Minutes); 
  P1_TotalTime=P1_TotalTime*60;                                                                        
}

void Time_Decrease1(void)
{
  Serial.println("Minus button pressed");
  minutes1=minutes1-1;
                     
  if(minutes1<0)
  {
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
  else if((hours>=10)&&(hours<=99))
  {                                              
    myGLCD.print(newhours, 330, 325);
    myGLCD.setColor(VGA_GRAY);
    myGLCD.fillRoundRect(325, 315, 330, 385);
    myGLCD.setColor(VGA_BLACK);
  } 
  else if(hours>99)
  {                                              
    myGLCD.print(newhours, 300, 325);
  }
  P1_Hours = atoi(newhours);
  P1_Minutes1 = atoi(newmins2);
  P1_Minutes2 = atoi(newmins1);
  P1_Total_Minutes = (10 * P1_Minutes1)+P1_Minutes2;
  P1_TotalTime = ((P1_Hours*60) + P1_Total_Minutes); 
  P1_TotalTime=P1_TotalTime*60;                                           
}

void Time_Increase2(void)
{
  Serial.println("Plus button pressed");
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
 
                                                 
    if(hours2<10)
    {                                              
      myGLCD.print(newhours2, 355, 325);
      myGLCD.setColor(VGA_GRAY);
      myGLCD.fillRoundRect(325, 315, 355, 385);
      myGLCD.setColor(VGA_BLACK);
      myGLCD.print(newmins3, 440, 325);                
      myGLCD.print(newmins4, 410, 325);
      myGLCD.fillCircle (400,340, 4);
      myGLCD.fillCircle (400,360, 4);
    }   
    
    else if((hours2>=10)&&(hours2<=99))
    {                                              
      myGLCD.print(newhours2, 330, 325);
      myGLCD.setColor(VGA_GRAY);
      myGLCD.fillRoundRect(325, 315, 335, 385);
      myGLCD.setColor(VGA_BLACK);
      myGLCD.print(newmins3, 440, 325);                
      myGLCD.print(newmins4, 410, 325);
      myGLCD.fillCircle (400,340, 4);
      myGLCD.fillCircle (400,360, 4);
    } 
    else if(hours2>99)
    {                                              
      myGLCD.print(newhours2, 300, 325);
      myGLCD.print(newmins3, 440, 325);                
      myGLCD.print(newmins4, 410, 325);
      myGLCD.fillCircle (400,340, 4);
      myGLCD.fillCircle (400,360, 4);
    }
    
    P2_Hours = atoi(newhours2);
    P2_Minutes1 = atoi(newmins4);
    P2_Minutes2 = atoi(newmins3);
    P2_Total_Minutes = (10 * P2_Minutes1)+P2_Minutes2;
    P2_TotalTime = ((P2_Hours*60) + P2_Total_Minutes); 
    P2_TotalTime=P2_TotalTime*60;                                       
}

void Time_Decrease2(void)
{
  Serial.println("Minus button pressed");
  minutes3=minutes3-1;
                     
  if(minutes3<0)
  {
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
  else if((hours2>=10)&&(hours2<=99))
  {                                              
    myGLCD.print(newhours2, 330, 325);
    myGLCD.setColor(VGA_GRAY);
    myGLCD.fillRoundRect(325, 315, 330, 385);
    myGLCD.setColor(VGA_BLACK);
  } 
  else if(hours2>99)
  {                                              
    myGLCD.print(newhours2, 300, 325);
  }
    
  P2_Hours = atoi(newhours2);
  P2_Minutes1 = atoi(newmins4);
  P2_Minutes2 = atoi(newmins3);
  P2_Total_Minutes = (10 * P2_Minutes1)+P2_Minutes2;
  P2_TotalTime = ((P2_Hours*60) + P2_Total_Minutes); 
  P2_TotalTime=P2_TotalTime*60;                              
}

void Max_Time_Decrease(void)
{
  if(Max_seconds<0)
  {
    Max_seconds=9;
    Max_seconds2=Max_seconds2-1;
                      
    if(Max_seconds2<0)
    {
      Max_seconds2=5;
      Max_minutes = Max_minutes - 1;
      
      if(Max_minutes<0)
      {
    Max_minutes=9;
    Max_minutes2=Max_minutes2-1;
                      
    if(Max_minutes2<0)
    {
      Max_minutes2=5;
      Max_hours = Max_hours - 1;
      
      if(Max_hours<0)
      {
      Max_seconds=0;
      Max_seconds2=0;
      Max_minutes=0;
      Max_minutes2=0;
      Max_hours=0;
      }
    }
  }
    }
  }                                                            
  sprintf(Max_newsecs, "%d", Max_seconds);
  sprintf(Max_newsecs2, "%d", Max_seconds2);
  sprintf(Max_newmins1, "%d", Max_minutes);
  sprintf(Max_newmins2, "%d", Max_minutes2);
  sprintf(Max_newhours, "%d", Max_hours);                                                           
  myGLCD.setBackColor(VGA_BACKGROUND);
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(SevenSegNumFont);
                                                                                             
  if(Max_hours<10)
  {                                              
    myGLCD.print(Max_newhours, 320, 285);
    myGLCD.setColor(VGA_BACKGROUND);
    myGLCD.fillRoundRect(290, 275, 320, 365);
    myGLCD.setColor(VGA_BLACK);
    myGLCD.print(Max_newmins1, 400, 285);                                                                          
    myGLCD.print(Max_newmins2, 370, 285);
    myGLCD.fillCircle (360,300, 4);
    myGLCD.fillCircle (360,320, 4);
    myGLCD.print(Max_newsecs, 480, 285);                                                                          
    myGLCD.print(Max_newsecs2, 450, 285);
    myGLCD.fillCircle (440,300, 4);
    myGLCD.fillCircle (440,320, 4);
  }   
  else if((Max_hours>=10)&&(Max_hours<=99))
  {                                              
    myGLCD.print(Max_newhours, 290, 285);
    myGLCD.setColor(VGA_BACKGROUND);
    myGLCD.fillRoundRect(290, 275, 300, 365);
    myGLCD.setColor(VGA_BLACK);
    myGLCD.print(Max_newmins1, 400, 285);                                                                          
    myGLCD.print(Max_newmins2, 370, 285);
    myGLCD.fillCircle (360,300, 4);
    myGLCD.fillCircle (360,320, 4);
    myGLCD.print(Max_newsecs, 480, 285);                                                                          
    myGLCD.print(Max_newsecs2, 450, 285);
    myGLCD.fillCircle (440,300, 4);
    myGLCD.fillCircle (440,320, 4);
  } 
  else if(Max_hours>99)
  {                                              
   myGLCD.print(Max_newhours, 300, 285);
   myGLCD.print(Max_newmins1, 440, 285);                                                                          
  myGLCD.print(Max_newmins2, 410, 285);
  myGLCD.fillCircle (400,300, 4);
  myGLCD.fillCircle (400,320, 4);
  myGLCD.print(Max_newsecs, 530, 285);                                                                          
  myGLCD.print(Max_newsecs2, 500, 285);
  myGLCD.fillCircle (490,300, 4);
  myGLCD.fillCircle (490,320, 4);
  }
  Check_Temp(); 
}

void Check_Time(void)
{
  TotalTime=P2_TotalTime+P1_TotalTime;
  if(TotalTime >3600)
  {
    Max_hours = TotalTime / 3600;
    TotalTime2 = TotalTime-(Max_hours * 3600);
    Max_minutes = TotalTime2/60;
    TotalTime2 = TotalTime2-(Max_minutes * 60);
    Max_seconds=TotalTime2;  

    if(Max_minutes >10)
    {
      Max_minutes2 = Max_minutes / 10;
      Max_minutes -= Max_minutes2 * 10;
    }
    else if(Max_minutes <10)
    {
      Max_minutes2 = 0;
      Max_minutes = Max_minutes * 1;
    }
  }
  else if(TotalTime <3600)
  {
    Max_hours = 0;
    Max_minutes = TotalTime/60;
    TotalTime2 = TotalTime-(Max_minutes * 60);
    Max_seconds=TotalTime2; 
  }
   if(Max_minutes >10)
    {
      Max_minutes2 = Max_minutes / 10;
      Max_minutes -= Max_minutes2 * 10;
    }
    else if(Max_minutes <10)
    {
      Max_minutes2 = 0;
      Max_minutes = Max_minutes * 1;
    }
}

