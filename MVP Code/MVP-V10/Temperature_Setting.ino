/**********************************************************************;
* Project           : Aeva Labs Demo Unit
*
* Program name      : Temperature_Settings.ino (Part of the MVP-V10 code)
*
* Author            : wdcosta
*
* Date created      : 06/15/2018
*
* Purpose           : This code contains functions that increase and decrease
*                     the temperature using buttons on the UI on the Configure 
*                     Extraction and Configure Maturity pages.
*                     
*                     This code also contains the functions that read value from 
*                     the thermistors
*                     
|**********************************************************************/

void Temperature_Increase1(void){
Serial.println("Plus button pressed");
  if(temp<140)
  {
  temp=temp+1;
  sprintf(newtemp, "%d", temp);
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


void Temperature_Decrease1(void){
Serial.println("Minus button pressed");
  if(temp>0)
  {
    temp=temp-1;
    sprintf(newtemp, "%d", temp);
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


void Temperature_Increase2(void){
Serial.println("Plus button pressed");
  if(temp2<140)
  {
    temp2=temp2+1;
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

void Temperature_Decrease2(void){
Serial.println("Minus button pressed");
  if(temp2>0)
  {
    temp2=temp2-1;
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


float Thermistor(int RawADC)
{
  long Resistance;  
  float Temp;  // Dual-Purpose variable to save space.
  Resistance=pad*((1024.0 / RawADC) - 1); 
  Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      
  return Temp;                                      
}


void readTemp(void) 
{
  float temp;
  temp=Thermistor(analogRead(TEMP_PIN));       // read ADC and  convert it to Celsius
  currentTemp = (temp * 9.0)/ 5.0 + 32.0;                  // converts to  Fahrenheit
  Serial.print(", Fahrenheit: "); 
  Serial.print(currentTemp);                             // display  Fahrenheit
  Serial.println("");  
}
