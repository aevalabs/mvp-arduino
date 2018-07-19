/*////////////////////////////////////////////////////////////////
// ------------------------------------------------------------
// AEVA LAB CODE
// DETAILS: Process
// ------------------------------------------------------------
///////////////////////////////////////////////////////////////*/

void Running_P1(void){
  Serial.print("Running P1");
  if(currentTemp >75)
  {
    digitalWrite(ULTRASOUND_PIN, LOW); //Turn on ultrasound
    digitalWrite(COOLING_PIN, LOW);    //Turn on cooling  
  }
  else if (currentTemp <=70)
  {
    digitalWrite(ULTRASOUND_PIN, HIGH); //Turn on ultrasound
    digitalWrite(COOLING_PIN, HIGH);    //Turn on cooling    
  }
}

void Running_P2(void){
  if (Output < millis() - windowStartTime){
    digitalWrite(HEATING_PIN,HIGH);
    Serial.print("Heating System On"); 
    Serial.println(currentTemp);
  }
  else {
    digitalWrite(HEATING_PIN, LOW);
    if((currentTemp >= (setTemp - 20)) && (currentTemp < (setTemp + 10))){
    digitalWrite(COOLING_PIN,HIGH);
    Serial.print("Cooling System On");
    }
    else if (currentTemp < setTemp)
    {
    digitalWrite(COOLING_PIN,LOW);
    Serial.print("Cooling System Off");
    }
  }
}

// STOP ----------------------------------------------------------------------------------//
// This function STOPS are ongoing processes.
void process_stop(void){
  digitalWrite(ULTRASOUND_PIN, LOW); //Turn on ultrasound
  digitalWrite(COOLING_PIN, LOW);    //Turn on cooling
  digitalWrite(HEATING_PIN, LOW); //Turn on ultrasound
  myGLCD.setBackColor(VGA_TRANSPARENT);
  myGLCD.setColor(VGA_TRANSPARENT);
}
