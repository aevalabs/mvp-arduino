void Default_Time_Increase1(void){
Serial.println("Plus button pressed");
default_minutes1=default_minutes1+1;

  if( default_minutes1>9)
  {
    default_minutes1=0;
    default_minutes2=default_minutes2+1;  
  }
        
  if(default_minutes2>5){
    default_minutes1=0;
    default_minutes2=0;
    default_hours=default_hours+1;
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

sprintf(newmins1, "%d", default_minutes1);
sprintf(newmins2, "%d", default_minutes2);
sprintf(newhours, "%d", default_hours);               
myGLCD.setBackColor(VGA_GRAY);
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(SevenSegNumFont);
myGLCD.print(newmins1, 440, 325);               
myGLCD.print(newmins2, 410, 325);
myGLCD.fillCircle (400,340, 4);
myGLCD.fillCircle (400,360, 4);
                                               
  if(default_hours<10)
  {                                              
  myGLCD.print(newhours, 355, 325);
  myGLCD.setColor(VGA_GRAY);
  myGLCD.fillRoundRect(325, 315, 355, 385);
  myGLCD.setColor(VGA_BLACK);
  }   
  else if(default_hours>=10)
  {                                              
  myGLCD.print(newhours, 325, 325);
  } 
  P1_Hours = atoi(newhours);
  P1_Minutes1 = atoi(newmins2);
  P1_Minutes2 = atoi(newmins1);
  P1_Total_Minutes = (10 * P1_Minutes1)+P1_Minutes2;
  P1_TotalTime = ((P1_Hours*60) + P1_Total_Minutes);                              
  }                            


void Default_Time_Decrease1(void){
Serial.println("Minus button pressed");
default_minutes1=default_minutes1-1;
                   
if(default_minutes1<0){
   default_minutes1=9;
   default_minutes2=default_minutes2-1;
                   
   if(default_minutes2<0)
   {
      default_minutes2=5;
      default_hours = default_hours - 1;
                   
      if(default_hours<0)
      {
         default_minutes1=0;
         default_minutes2=0;
         default_hours=0;
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
                
sprintf(newmins1, "%d", default_minutes1);
sprintf(newmins2, "%d", default_minutes2);
sprintf(newhours, "%d", default_hours);               
myGLCD.setBackColor(VGA_GRAY);
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(SevenSegNumFont);
myGLCD.print(newmins1, 440, 325);               
myGLCD.print(newmins2, 410, 325);
myGLCD.fillCircle (400,340, 4);
myGLCD.fillCircle (400,360, 4);

if(default_hours<10)
{                                              
myGLCD.print(newhours, 355, 325);
myGLCD.setColor(VGA_GRAY);
myGLCD.fillRoundRect(325, 315, 355, 385);
myGLCD.setColor(VGA_BLACK);
}   
else if(default_hours>=10)
{                                              
myGLCD.print(newhours, 325, 325);
} 
P1_Hours = atoi(newhours);
P1_Minutes1 = atoi(newmins2);
P1_Minutes2 = atoi(newmins1);
P1_Total_Minutes = (10 * P1_Minutes1)+P1_Minutes2;
P1_TotalTime = ((P1_Hours*60) + P1_Total_Minutes);
Serial.println(P1_TotalTime);   
}

void Default_Time_Increase2(void){
Serial.println("Plus button pressed");
default_minutes3=default_minutes3+1;

  if( default_minutes3>9)
  {
    default_minutes3=0;
    default_minutes4=default_minutes4+1;  
  }
        
  if(default_minutes4>5){
    default_minutes3=0;
    default_minutes4=0;
    default_hours2=default_hours2+1;
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

sprintf(newmins3, "%d", default_minutes3);
sprintf(newmins4, "%d", default_minutes4);
sprintf(newhours2, "%d", default_hours2);               
myGLCD.setBackColor(VGA_GRAY);
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(SevenSegNumFont);
myGLCD.print(newmins3, 440, 325);              
myGLCD.print(newmins4, 410, 325);
myGLCD.fillCircle (400,340, 4);
myGLCD.fillCircle (400,360, 4);
                                               
if(default_hours2<10)
{                                              
  myGLCD.print(newhours2, 355, 325);
  myGLCD.setColor(VGA_GRAY);
  myGLCD.fillRoundRect(325, 315, 355, 385);
  myGLCD.setColor(VGA_BLACK);
}   
else if(default_hours2>=10)
{                                              
myGLCD.print(newhours2, 325, 325);
}   
P2_Hours = atoi(newhours2);
P2_Minutes1 = atoi(newmins4);
P2_Minutes2 = atoi(newmins3);
P2_Total_Minutes = (10 * P2_Minutes1)+P2_Minutes2;
P2_TotalTime = ((P2_Hours*60) + P2_Total_Minutes);                                 
}           


void Default_Time_Decrease2(void){
Serial.println("Minus button pressed");
default_minutes3=default_minutes3-1;
                   
if(default_minutes3<0){
   default_minutes3=9;
   default_minutes4=default_minutes4-1;
                   
   if(default_minutes4<0)
   {
      default_minutes4=5;
      default_hours2 = default_hours2 - 1;
                   
      if(default_hours2<0)
      {
         default_minutes3=0;
         default_minutes4=0;
         default_hours2=0;
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
                
sprintf(newmins3, "%d", default_minutes3);
sprintf(newmins4, "%d", default_minutes4);
sprintf(newhours2, "%d", default_hours2);               
myGLCD.setBackColor(VGA_GRAY);
myGLCD.setColor(VGA_BLACK);
myGLCD.setFont(SevenSegNumFont);
myGLCD.print(newmins3, 440, 325);              
myGLCD.print(newmins4, 410, 325);
myGLCD.fillCircle (400,340, 4);
myGLCD.fillCircle (400,360, 4);
                                               
if(default_hours2<10)
{                                              
  myGLCD.print(newhours2, 355, 325);
  myGLCD.setColor(VGA_GRAY);
  myGLCD.fillRoundRect(325, 315, 355, 385);
  myGLCD.setColor(VGA_BLACK);
}   
else if(default_hours2>=10)
{                                              
myGLCD.print(newhours2, 325, 325);
}   
P2_Hours = atoi(newhours2);
P2_Minutes1 = atoi(newmins4);
P2_Minutes2 = atoi(newmins3);
P2_Total_Minutes = (10 * P2_Minutes1)+P2_Minutes2;
P2_TotalTime = ((P2_Hours*60) + P2_Total_Minutes);                                 
}    


void Default_Temperature_Increase1(void){
Serial.println("Plus button pressed");
 
  if(default_temp<140)
  {
  default_temp=default_temp+1;
  sprintf(newtemp, "%d", default_temp);
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

void Default_Temperature_Increase2(void){
Serial.println("Plus button pressed");
 
  if(default_temp2<140)
  {
  default_temp2=default_temp2+1;
  sprintf(newtemp2, "%d", default_temp2);
  Serial.println(newtemp2);
  myGLCD.setColor(VGA_GRAY);
  myGLCD.fillRoundRect(325, 180, 480, 250);
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print(newtemp2, CENTER, 190);
  setTemp2 = atoi(newtemp2);
  Serial.println(setTemp2);
  }
}

void Default_Temperature_Decrease1(void){
Serial.println("Minus button pressed");
  if(default_temp>0)
  {
  default_temp=default_temp-1;
  sprintf(newtemp, "%d", default_temp);
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

void Default_Temperature_Decrease2(void){
Serial.println("Minus button pressed");
  if(default_temp2>0)
  {
  default_temp2=default_temp2-1;
  sprintf(newtemp2, "%d", temp2);
  Serial.println(newtemp2);
  myGLCD.setColor(VGA_GRAY);
  myGLCD.fillRoundRect(325, 180, 480, 250);
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print(newtemp2, CENTER, 190);
  setTemp2 = atoi(newtemp2);
  Serial.println(setTemp2);
  }
}
