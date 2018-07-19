/*////////////////////////////////////////////////////////////////
// ------------------------------------------------------------
// AEVA LAB CODE
// DETAILS: Pages
// ------------------------------------------------------------
///////////////////////////////////////////////////////////////*/

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
 


myGLCD.setColor(243, 128, 97);
myGLCD.fillRoundRect(550, 230, 700, 290);
myGLCD.setBackColor(243, 128, 97);
myGLCD.setColor(VGA_WHITE);
myGLCD.print("PAUSE", 580, 250); //start button
page=5;  
}

void Paused_Process_Page(void){
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

myGLCD.setFont(BigFont);
myGLCD.setColor(243, 128, 97);
myGLCD.fillRoundRect(10, 60, 140, 120);
myGLCD.setBackColor(243, 128, 97);
myGLCD.setColor(VGA_WHITE);
myGLCD.print("CANCEL", 20, 80); //start button 
page=7;  
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

void Default_Configure_Extraction(void){
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
sprintf(default_newtemp, "%d", default_temp);
myGLCD.print(default_newtemp, CENTER, 190);
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
sprintf(default_newmins1, "%d", default_minutes1);
sprintf(default_newmins2, "%d", default_minutes2);
sprintf(default_newhours, "%d", default_hours);
myGLCD.print(default_newhours, 355, 325); //set temp value
myGLCD.fillCircle (400,340, 4);
myGLCD.fillCircle (400,360, 4);
myGLCD.print(default_newmins1, 440, 325);
myGLCD.print(default_newmins2, 410, 325);
           
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
page=8; 
}

void Default_Configure_Maturity(void){
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
page=9;                         
}


void Clear_Array(void){
  P1_Hours=0;
  P1_Total_Minutes=0;
  P1_Minutes1=0;
  P1_Minutes2=0;
  P1_TotalTime=0;
  P2_Hours=0;
  P2_Total_Minutes=0;
  P2_Minutes1=0;
  P2_Minutes2=0;
  P2_TotalTime=0;
  Minutes1=0;
  Minutes2=0;
  Hours2=0;
  Minutes3=0;
  Minutes4=0;
  minutes1 = 0;
  minutes2 = 0;
  hours = 0;
  minutes3 = 0;
  minutes4 = 0;
  hours2 = 0;
  Max_minutes = 0;
  Max_minutes2 = 0;
  Max_hours = 0;
  memset(newtemp,0,sizeof(newtemp));
  memset(newtemp2,0,sizeof(newtemp2));
  memset(newmins1,0,sizeof(newmins1));
  memset(newmins2,0,sizeof(newmins2));
  memset(newhours,0,sizeof(newhours));
  memset(newmins3,0,sizeof(newmins3));
  memset(newmins4,0,sizeof(newmins4));
  memset(newhours2,0,sizeof(newhours2));
  memset(Max_newmins1,0,sizeof(Max_newmins1));
  memset(Max_newmins2,0,sizeof(Max_newmins2));
  memset(Max_newhours,0,sizeof(Max_newhours));
}

