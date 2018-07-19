/**********************************************************************;
* Project           : Aeva Labs Demo Unit
*
* Program name      : Process.ino (Part of the MVP-V14 code)
*
* Author            : wdcosta
*
* Date created      : 06/15/2018
*
* Purpose           : This code runs the crucial process functions for P1 and P2
*
|**********************************************************************/

void Running_P1(void)
{
  if(currentTemp >Set_Extraction_Temp)
  {  
    myGLCD.setFont(Arial_round_16x24); 
    Serial.println("P1:Cooling");
    myGLCD.print("P1:C", 200, 400); 
    digitalWrite(ULTRASOUND_PIN, LOW); //Turn on ultrasound
    digitalWrite(COOLING_PIN, HIGH);    //Turn on cooling  
  }
  else if (currentTemp <=Set_Extraction_Temp)
  {
    myGLCD.setFont(Arial_round_16x24); 
    myGLCD.print("P1:U", 200, 400); 
     Serial.println("P1:ULTRASOUND");
    digitalWrite(ULTRASOUND_PIN, HIGH); //Turn on ultrasound
    digitalWrite(COOLING_PIN, LOW);    //Turn on cooling    
  }
}

void Running_P2(void)
{
   if (Output > millis() - windowStartTime)
  {
    myGLCD.setFont(Arial_round_16x24); 
    myGLCD.print("P2:ONN", 200, 400); 
    Serial.println("Heating System On");
    Serial.println(windowStartTime); 
    digitalWrite(HEATING_PIN,HIGH);
  }
  else 
  {
    myGLCD.setFont(Arial_round_16x24); 
    myGLCD.print("P2:OFF", 200, 400); 
    Serial.println("Output:");
    Serial.println(Output);
    Serial.println("windowStartTime:"); 
    Serial.println(windowStartTime); 
    digitalWrite(HEATING_PIN, LOW);
    Serial.println("Heating System Off"); 
  } 
 
  if(currentTemp >Setpoint)
  { 
   myGLCD.setFont(Arial_round_16x24); 
   myGLCD.print("P2:OFF", 200, 400); 
//   Serial.println(PID_value);
   digitalWrite(HEATING_PIN, LOW);
   Serial.println("Heating System Off");  
  }
}

// STOP ----------------------------------------------------------------------------------//
// This function STOPS are ongoing processes.
void process_stop(void)
{
  digitalWrite(ULTRASOUND_PIN, LOW); //Turn oFF ultrasound
  digitalWrite(COOLING_PIN, LOW);    //Turn oFF cooling
  digitalWrite(HEATING_PIN, LOW); //Turn oFF HEATING
}
