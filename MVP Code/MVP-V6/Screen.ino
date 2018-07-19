/*////////////////////////////////////////////////////////////////
// ------------------------------------------------------------
// AEVA LAB CODE
// DETAILS: Screen
// ------------------------------------------------------------
///////////////////////////////////////////////////////////////*/

// StartUp screen --------------------------------------------------------------------------------//
// This function displays the begining screen on the UI
void StartUpScreen(void)
{
  int buf[798];
  int x, x2;
  int y, y2;
  int r;
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
                   if(touchLocations[i].x>=0&&touchLocations[i].x<=200&&touchLocations[i].y>=55&&touchLocations[i].y<=200 )
                   {  //Back
                      Clear_Array();          
                      Home_Page();
                      page=1; 
                   }
                   else if(touchLocations[i].x>=550&&touchLocations[i].x<=800&&touchLocations[i].y>=300&&touchLocations[i].y<=400 )
                    { //Next
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
                     else if(touchLocations[i].x>=40&&touchLocations[i].x<=180&&touchLocations[i].y>=250&&touchLocations[i].y<=300 )
                     {    
                      Default_Configure_Extraction();
                     }
                 }
                 
                 else if(page==3){
                   if(touchLocations[i].x>=0&&touchLocations[i].x<=200&&touchLocations[i].y>=55&&touchLocations[i].y<=200 )
                   {  //Back         
                      Configure_Extraction(); 
                   }
                   else if(touchLocations[i].x>=550&&touchLocations[i].x<=800&&touchLocations[i].y>=300&&touchLocations[i].y<=400 )
                    { //Next
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
                   {  //Back           
                     Configure_Maturity(); 
                   }
                   else if(touchLocations[i].x>=280&&touchLocations[i].x<=480&&touchLocations[i].y>=300&&touchLocations[i].y<=380 )
                    { //Start
                      Start_flag=1;
                      P1_flag = 1;
                      Process_Page();           
                    }    
                 }
                 else if(page==5){
                   if(touchLocations[i].x>=550&&touchLocations[i].x<=700&&touchLocations[i].y>=230&&touchLocations[i].y<=290)
                   { //Pause
                     Stop_flag=1;
                     Paused_Process_Page();          
                   } 
                 }
              
                 else if(page==6){ //300, 250, 500, 330
                   if(touchLocations[i].x>=300&&touchLocations[i].x<=500&&touchLocations[i].y>=250&&touchLocations[i].y<=330 )
                   {   //Return          
                      Home_Page(); 
                      Clear_Array();
                   }
                 }
                 else if(page==7){ //300, 250, 500, 330
                     if(touchLocations[i].x>=10&&touchLocations[i].x<=160&&touchLocations[i].y>=60&&touchLocations[i].y<=120 )
                   {  //Cancel
                      Stop_flag=1;
                      Clear_Array();
                      Background();
                      Home_Page();  
                   } //550, 290, 700, 350
                   else if(touchLocations[i].x>=550&&touchLocations[i].x<=700&&touchLocations[i].y>=180&&touchLocations[i].y<=240 )
                   { //Resume
                    P1_flag=1;
                    Start_flag=1;
                    Process_Page();                  
                   }//550, 180, 700, 240
                   else if(touchLocations[i].x>=550&&touchLocations[i].x<=700&&touchLocations[i].y>=290&&touchLocations[i].y<=350 )
                   {//Pause(for debugging)
                     Stop_flag=1;          
                   } 
                 }
                 else if(page==8){
                   if(touchLocations[i].x>=0&&touchLocations[i].x<=200&&touchLocations[i].y>=55&&touchLocations[i].y<=200 )
                   {  //Back
                      Clear_Array();          
                      Home_Page();
                      page=1; 
                   }
                   else if(touchLocations[i].x>=550&&touchLocations[i].x<=800&&touchLocations[i].y>=300&&touchLocations[i].y<=400 )
                    { //Next
                      Default_Configure_Maturity();
                      default_temp = 75;
                      page=3;             
                    }
                   else if(touchLocations[i].x>=490&&touchLocations[i].x<=540&&touchLocations[i].y>=190&&touchLocations[i].y<=230 )
                    {  
                      Default_Temperature_Increase1();//(260, 200, 290, 230);
                    }
                    else if(touchLocations[i].x>=250&&touchLocations[i].x<=290&&touchLocations[i].y>=200&&touchLocations[i].y<=230 )
                    {    
                      Default_Temperature_Decrease1();//(490, 200, 520, 230);
                    }
                    else if(touchLocations[i].x>=500&&touchLocations[i].x<=550&&touchLocations[i].y>=320&&touchLocations[i].y<=370 )
                    {    
                      Default_Time_Increase1(); //(260, 320, 310, 370); (500, 320, 550, 370);
                    }
                    else if(touchLocations[i].x>=250&&touchLocations[i].x<=290&&touchLocations[i].y>=320&&touchLocations[i].y<=370 )
                    {    
                      Default_Time_Decrease1();
                    }//40, 250, 180, 300
                     else if(touchLocations[i].x>=40&&touchLocations[i].x<=180&&touchLocations[i].y>=250&&touchLocations[i].y<=300 )
                     {    
                      Default_Configure_Extraction();
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
                Max_Time_Decrease();   
                                  
                if (TotalTime == 0)
                {
                Start_flag = 0;
                TotalTime = 0;
                Completed_Page();
                Clear_Array();
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
              {
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
                }
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
            }
          Stop_flag=0;
         }      
      }
}


