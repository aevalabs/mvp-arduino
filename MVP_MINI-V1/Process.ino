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
    myGLCD.setFont(BigFont);
    myGLCD.print("P1:U", 200, 400); 
    Serial.println("P1:ULTRASOUND");
    digitalWrite(ULTRASOUND_PIN, HIGH); //Turn on ultrasound
}

// STOP ----------------------------------------------------------------------------------//
// This function STOPS are ongoing processes.
void process_stop(void)
{
  digitalWrite(ULTRASOUND_PIN, LOW); //Turn oFF ultrasound
  digitalWrite(COOLING_PIN, LOW);    //Turn oFF cooling
  digitalWrite(HEATING_PIN, LOW); //Turn oFF HEATING
}
