/**********************************************************************;
* Project           : Aeva Labs Demo Unit
*
* Program name      : Pages.ino (Part of the MVP-V7 code)
*
* Author            : wdcosta
*
* Date created      : 06/15/2018
*
* Purpose           : This code draws the multiple pages for the UI
*
|**********************************************************************/

void Background(void){
  myGLCD.setColor(VGA_ROWS);
  myGLCD.fillRect(0, 0, 799, 50); 
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
myGLCD.fillCircle(250,220,20);
myGLCD.setColor(VGA_BLACK);
//myGLCD.fillRoundRect(200, 190, 300, 250);
myGLCD.drawCircle(250,220,20);
myGLCD.setColor(VGA_WHITE);
myGLCD.setFont(BigFont);
myGLCD.print("-", 242, 213);   //button that increases the temp
            
myGLCD.setBackColor(VGA_GREEN);
myGLCD.setColor(VGA_GREEN);
myGLCD.fillCircle(550, 220, 20);
myGLCD.setColor(VGA_BLACK);
//myGLCD.fillRoundRect(500, 190, 600, 250);
myGLCD.drawCircle(550, 220, 20);
myGLCD.setColor(VGA_WHITE);
myGLCD.print("+", 542, 213);   //button that decreases the temp
            
myGLCD.setFont(BigFont);
myGLCD.setBackColor(VGA_RED);
myGLCD.setColor(VGA_RED);
myGLCD.fillCircle(250,350,20);
myGLCD.setColor(VGA_BLACK);
//myGLCD.fillRoundRect(200, 320, 300, 380);
myGLCD.drawCircle(250,350,20);
myGLCD.setColor(VGA_WHITE);
myGLCD.setFont(BigFont);
myGLCD.print("-", 242, 342);   //button that reduces the time
            
myGLCD.setBackColor(VGA_GREEN);
myGLCD.setColor(VGA_GREEN);
myGLCD.fillCircle(550, 350, 20);
myGLCD.setColor(VGA_BLACK);
//myGLCD.fillRoundRect(500, 320, 600, 380);
myGLCD.drawCircle(550, 350, 20);
myGLCD.setColor(VGA_WHITE);
myGLCD.print("+", 542, 342);   //button that increases the time
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
void Configure_Extraction(void)
{
  Background();
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(Arial_round_16x24); 
  myGLCD.print("Configure Extraction", CENTER, 80); // Set temp:
  myGLCD.setColor(VGA_GRAY);
  myGLCD.drawLine(250, 120, 560, 120);
  Page_Text();
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.setFont(SevenSegNumFont);
  sprintf(newtemp, "%d", temp);
  myGLCD.print(newtemp, CENTER, 190);
  Add_Subtract_Buttons();           
  myGLCD.setColor(VGA_GRAY);
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.fillRoundRect(300, 315, 500, 385);
  myGLCD.setColor(VGA_BLACK);
   myGLCD.setFont(SevenSegNumFont);
  sprintf(newmins1, "%d", minutes1);
  sprintf(newmins2, "%d", minutes2);
  sprintf(newhours, "%d", hours); 
  
    if(hours<10)
  {                                              
    myGLCD.print(newhours, 355, 325);
    myGLCD.setColor(VGA_GRAY);
    myGLCD.fillRoundRect(325, 315, 355, 385);
    myGLCD.setColor(VGA_BLACK);
  }   
  else if((hours>=10)&&(hours<99))
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
  myGLCD.fillCircle (400,340, 4);
  myGLCD.fillCircle (400,360, 4);
  myGLCD.print(newmins1, 440, 325);
  myGLCD.print(newmins2, 410, 325);             
  myGLCD.setColor(VGA_DEFAULT);
  myGLCD.fillRoundRect(40, 250, 180, 300);
  myGLCD.setBackColor(VGA_DEFAULT);
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

void Configure_Maturity(void)
{
  Background();
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(Arial_round_16x24); 
  myGLCD.print("Configure Maturity", CENTER, 80); // Set temp:
  myGLCD.setColor(VGA_BLACK);
  myGLCD.drawLine(250, 120, 560, 120);
  Page_Text();                        
  myGLCD.setColor(VGA_GRAY);
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.fillRoundRect(300, 180, 500, 250);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.setColor(VGA_BLACK);
  
  sprintf(newtemp2, "%d", temp2);
  myGLCD.print(newtemp2, CENTER, 190);
  Add_Subtract_Buttons();           
  myGLCD.setBackColor(VGA_BACKGROUND);
  myGLCD.setColor(VGA_GRAY);
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.fillRoundRect(300, 315, 500, 385);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.setColor(VGA_BLACK);
  sprintf(newmins3, "%d", minutes3);
  sprintf(newmins4, "%d", minutes4);
  sprintf(newhours2, "%d", hours2);
   
    if(hours2<10)
  {                                              
    myGLCD.print(newhours2, 355, 325);
    myGLCD.setColor(VGA_GRAY);
    myGLCD.fillRoundRect(325, 315, 355, 385);
    myGLCD.setColor(VGA_BLACK);
  }   
  else if((hours2>=10)&&(hours2<99))
  {                                              
    myGLCD.print(newhours2, 330, 325);
    myGLCD.setColor(VGA_GRAY);
    myGLCD.fillRoundRect(325, 315, 300, 385);
    myGLCD.setColor(VGA_BLACK);
  } 
  else if(hours2>99)
  {                                              
    myGLCD.print(newhours2, 300, 325);
  }
  myGLCD.fillCircle (400,340, 4);
  myGLCD.fillCircle (400,360, 4);
  myGLCD.print(newmins3, 440, 325);
  myGLCD.print(newmins4, 410, 325);
             
  myGLCD.setColor(VGA_DEFAULT);
  myGLCD.fillRoundRect(40, 250, 180, 300);
  myGLCD.setBackColor(VGA_DEFAULT);
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
  Check_Time();
  page=4;  
}

void Process_Page(void){
  Background();
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(Arial_round_16x24); 
  myGLCD.print("Aging In Progress", CENTER, 90);
  myGLCD.setColor(VGA_GRAY);
  myGLCD.drawLine(250, 130, 560, 130);
                  
  myGLCD.setBackColor(VGA_BACKGROUND); 
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(SevenSegNumFont);
if(Max_hours<10)
  {                                              
    myGLCD.print(Max_newhours, 315, 285);
    myGLCD.setColor(VGA_BACKGROUND);
    myGLCD.fillRoundRect(310, 275, 315, 365);
    myGLCD.setColor(VGA_BLACK);
    myGLCD.print(Max_newmins1, 400, 285);                                                                          
    myGLCD.print(Max_newmins2, 370, 285);
    myGLCD.fillCircle (360,300, 4);
    myGLCD.fillCircle (360,320, 4);
  }   
  else if((Max_hours>=10)&&(Max_hours<99))
  {                                              
    myGLCD.print(Max_newhours, 335, 285);
    myGLCD.setColor(VGA_BACKGROUND);
    myGLCD.fillRoundRect(310, 275, 335, 365);
    myGLCD.print(Max_newmins1, 450, 285);                                                                          
    myGLCD.print(Max_newmins2, 420, 285);
    myGLCD.fillCircle (410,300, 4);
    myGLCD.fillCircle (410,320, 4);
  } 
  else if(Max_hours>99)
  {                                              
   myGLCD.print(Max_newhours, 300, 285);
   myGLCD.print(Max_newmins1, 450, 285);                                                                          
  myGLCD.print(Max_newmins2, 420, 285);
  myGLCD.fillCircle (410,300, 4);
  myGLCD.fillCircle (410,320, 4);
  }                            
  //sprintf(Process_Time, "%d", Time_Remaining);
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(Arial_round_16x24); 
  myGLCD.print("Time Remaining:", 50, 300); 
  myGLCD.print("Current Temp:", 50, 200); 
   sprintf(Display_Current_Temp, "%d", currentTemp);
  myGLCD.print(Display_Current_Temp, 300, 190);  
  myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_PAUSE );
  myGLCD.fillRoundRect(550, 230, 700, 290);
  myGLCD.setBackColor(VGA_PAUSE );
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
                 
  myGLCD.setBackColor(VGA_BACKGROUND); 
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(SevenSegNumFont);
  if(Max_hours<10)
  {                                              
    myGLCD.print(Max_newhours, 315, 285);
    myGLCD.setColor(VGA_BACKGROUND);
    myGLCD.fillRoundRect(310, 275, 315, 365);
    myGLCD.setColor(VGA_BLACK);
    myGLCD.print(Max_newmins1, 400, 285);                                                                          
    myGLCD.print(Max_newmins2, 370, 285);
    myGLCD.fillCircle (360,300, 4);
    myGLCD.fillCircle (360,320, 4);
  }   
  else if((Max_hours>=10)&&(Max_hours<99))
  {                                              
    myGLCD.print(Max_newhours, 335, 285);
    myGLCD.setColor(VGA_BACKGROUND);
    myGLCD.fillRoundRect(310, 275, 335, 365);
    myGLCD.print(Max_newmins1, 450, 285);                                                                          
    myGLCD.print(Max_newmins2, 420, 285);
    myGLCD.fillCircle (410,300, 4);
    myGLCD.fillCircle (410,320, 4);
  } 
  else if(Max_hours>99)
  {                                              
   myGLCD.print(Max_newhours, 300, 285);
   myGLCD.print(Max_newmins1, 450, 285);                                                                          
  myGLCD.print(Max_newmins2, 420, 285);
  myGLCD.fillCircle (410,300, 4);
  myGLCD.fillCircle (410,320, 4);
  }
  //sprintf(Process_Time, "%d", Time_Remaining);
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(Arial_round_16x24); 
  myGLCD.print("Time Remaining:", 50, 300); 
  myGLCD.print("Current Temp:", 50, 200);
  
  myGLCD.setFont(SevenSegNumFont);
  sprintf(Display_Current_Temp, "%d", currentTemp);
  myGLCD.print(Display_Current_Temp, 300, 190); 
   
  myGLCD.drawRoundRect(550, 230, 700, 290);
  
  myGLCD.setColor(VGA_RESUME );
  myGLCD.fillRoundRect(550, 230, 700, 290);
  myGLCD.setBackColor(VGA_RESUME );
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(BigFont);
  myGLCD.print("RESUME", 580, 250); //start button
  
  myGLCD.setFont(BigFont);
  myGLCD.setColor(243, 128, 97);
  myGLCD.fillRoundRect(10, 60, 140, 120);
  myGLCD.setBackColor(243, 128, 97);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("CANCEL", 20, 80); //start button 
  page=7;  
}


void Completed_Page(void)
{ 
  Background();
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(Arial_round_16x24);
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

void Page_Text(void)
{
  myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_BLACK);
  myGLCD.print("INTENSITY:", CENTER, 155); // Set temp:            
  myGLCD.setColor(VGA_GRAY);
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.fillRoundRect(300, 180, 500, 250);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.setColor(VGA_BLACK); 
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(VGA_BACKGROUND);
  myGLCD.print("LENGTH:", CENTER, 290); // Set temp:
}

