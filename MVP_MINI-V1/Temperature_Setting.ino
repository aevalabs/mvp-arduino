/**********************************************************************;
* Project           : Aeva Labs Demo Unit
*
* Program name      : Temperature_Settings.ino (Part of the MVP-V14 code)
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

void Temperature_Increase1(void)
{
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
  Set_Extraction_Temp = atoi(newtemp);
  Serial.println(Set_Extraction_Temp);
  }
}


void Temperature_Decrease1(void)
{
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
    Set_Extraction_Temp = atoi(newtemp);
    Serial.println(Set_Extraction_Temp);
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
  float temp=0;
  float temp0=0;
  float temp1=0;
  float temp2=0;
  float temp3=0;
  float temp4=0;
  float temp5=0;
  temp0=analogRead(TEMP_PIN);       // read ADC and  convert it to Celsius
  temp1=analogRead(TEMP_PIN);
  temp2=analogRead(TEMP_PIN);
  temp3=analogRead(TEMP_PIN);
  temp4=analogRead(TEMP_PIN);
  temp5=Thermistor(analogRead(TEMP_PIN));       // read ADC and  convert it to Celsius
  temp=hermistor((temp1+temp2+temp3+temp4+temp5)/5);
  currentTemp = (temp * 9.0)/ 5.0 + 32.0;               // converts to  Fahrenheit
  Serial.print("Fahrenheit: "); 
  Serial.print(currentTemp);                             // display  Fahrenheit
  Serial.println("");  
}

void Check_Temp(void) 
{
  sprintf(Display_Current_Temp, "%d", currentTemp);
  
  if(currentTemp>=100)
  {                                              
    myGLCD.print(Display_Current_Temp, 280, 190);
  }   
  else if(currentTemp<99)
  { 
    myGLCD.setColor(VGA_BACKGROUND);
    myGLCD.fillRoundRect(250, 180, 320, 250);
    myGLCD.setColor(VGA_BLACK);                                             
    myGLCD.print(Display_Current_Temp, 310, 190);
  }    
}

double Celcius2Fahrenheit(double celsius)
{
  return 1.8 * celsius + 32;
}
