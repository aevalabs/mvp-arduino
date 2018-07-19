/**********************************************************************;
* Project           : Aeva Labs Demo Unit
*
* Program name      : Process.ino (Part of the MVP-V15 code)
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
  PID_error = Setpoint - currentTemp;
  //Calculate the P value
  PID_p = kp * PID_error;
  //Calculate the I value in a range on +-3
  if(-3 < PID_error <3)
  {
    PID_i = PID_i + (ki * PID_error);
  }

  //For derivative we need real time to calculate speed change rate
  timePrev = Time;                            // the previous time is stored before the actual time read
  Time = millis();                            // actual time read
  elapsedTime = (Time - timePrev) / 1000; 
  //Now we can calculate the D calue
  PID_d = kd*((PID_error - previous_error)/elapsedTime);
  //Final total PID value is the sum of P + I + D
  PID_value = PID_p + PID_i + PID_d;
  
   if (PID_value > millis() - elapsedTime)
  {
    myGLCD.setFont(Arial_round_16x24); 
    myGLCD.print("P2:ONN", 200, 400); 
    Serial.println("Heating System On");
    Serial.println(PID_value); 
    digitalWrite(HEATING_PIN,HIGH);
  }
  else 
  {
    myGLCD.setFont(Arial_round_16x24); 
    myGLCD.print("P2:OFF", 200, 400); 
    Serial.println(PID_value);
    digitalWrite(HEATING_PIN, LOW);
    Serial.println("Heating System Off"); 
  } 

   if(currentTemp >Setpoint)
  { 
   myGLCD.setFont(Arial_round_16x24); 
   myGLCD.print("P2:OFF", 200, 400); 
   Serial.println(PID_value);
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
