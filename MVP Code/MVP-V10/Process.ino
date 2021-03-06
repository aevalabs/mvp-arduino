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
  if(currentTemp >setTemp)
  {
    Serial.println("Running P1");
    digitalWrite(ULTRASOUND_PIN, LOW); //Turn on ultrasound
    digitalWrite(COOLING_PIN, HIGH);    //Turn on cooling  
  }
  else if (currentTemp <=setTemp)
  {
    Serial.println("Running P1");
    digitalWrite(ULTRASOUND_PIN, HIGH); //Turn on ultrasound
    digitalWrite(COOLING_PIN, LOW);    //Turn on cooling    
  }
  Serial.println("Running P1");
}

void Running_P2(void)
{
  if (Output < millis() - windowStartTime)
  {
    digitalWrite(HEATING_PIN,HIGH);
    Serial.print("Heating System On"); 
    Serial.println(currentTemp);
  }
  else 
  {
    digitalWrite(HEATING_PIN, LOW);
    Serial.print("Heating System Off"); 
    if((currentTemp >= (setTemp2 - 20)) && (currentTemp < (setTemp2 + 10)))
    {
      digitalWrite(COOLING_PIN,HIGH);
      Serial.print("Cooling System On");
    }
    else if (currentTemp < setTemp2)
    {
      digitalWrite(COOLING_PIN,LOW);
      Serial.print("Cooling System Off");
    }
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
