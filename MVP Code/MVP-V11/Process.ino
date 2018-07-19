/**********************************************************************;
* Project           : Aeva Labs Demo Unit
*
* Program name      : Process.ino (Part of the MVP-V7 code)
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
    Serial.println("P1:Cooling");
    digitalWrite(ULTRASOUND_PIN, LOW); //Turn on ultrasound
    digitalWrite(COOLING_PIN, HIGH);    //Turn on cooling  
  }
  else if (currentTemp <=Set_Extraction_Temp)
  {
     Serial.println("P1:ULTRASOUND");
    digitalWrite(ULTRASOUND_PIN, HIGH); //Turn on ultrasound
    digitalWrite(COOLING_PIN, LOW);    //Turn on cooling    
  }
}

void Running_P2(void)
{
  
}

// STOP ----------------------------------------------------------------------------------//
// This function STOPS are ongoing processes.
void process_stop(void)
{
  digitalWrite(ULTRASOUND_PIN, LOW); //Turn oFF ultrasound
  digitalWrite(COOLING_PIN, LOW);    //Turn oFF cooling
  digitalWrite(HEATING_PIN, LOW); //Turn oFF HEATING
}
