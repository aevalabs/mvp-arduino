#define TEMP_PIN    A0
#define TRANSD_RELAY_PIN 13
#define DECODE_FLAG         0x01
#define TEMP_FLAG           0x02

#include <SoftwareSerial.h>

int tempOfSystem = 860; // Room Temp
int i=0;
byte operationFlag = 0x00;
char cmdStr[11]; //Commands can be up to 10 letters/number longs

SoftwareSerial wifiSerial(7,8); //RX, TX
// Wifi chip needs a new line character after commands
void setup() {
  // put your setup code here, to run once:
  // Pin Sets - cant use PWM 9 and 10 - used by timer 1
  pinMode(TEMP_PIN, INPUT);
  pinMode(TRANSD_RELAY_PIN, OUTPUT);
  // Timer Setup
  TCCR1A = 0;
  TCCR1B = 0;
  
  OCR1A   = 62500; // Comparison Register set to 16,000,000 / 1024 / 0.25Hz (Once every 4 seconds) 
  TCCR1B |= (1 << WGM12); // Enable CTC
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Set clock divider to 1024
  TIMSK1 |= (1 << OCIE1A);
  TCNT1  = 0;
  
  //analogReadResolution(10); // Set resolution to 10bit.
  Serial.begin(9600); // Set serial comm to 115.2k
  while (!Serial){ 
    // bootup delay
  }
  
  
  wifiSerial.begin(9600);
  while (!wifiSerial){
    // wifi delay
  }
  Serial.println("Arduino Ready!");
  wifiSerial.println("AT"); // wont pop up on serial line till serial swap code in main.
  wifiSerial.println("AT+CIPSTAMAC?"); // Gets busy signal back - 60:01:94:5d:6e:ad
  while(wifiSerial.available()){
    Serial.write(wifiSerial.read());
  }
}

void loop() 
{
  
  //Serial.println("AT+UART=9600,8,1,0,3");
 // delay(2000);
 // Serial.println("AT+UART_DEF=9600,8,1,0,0");
  // put your main code here, to run repeatedly:
 
  if(operationFlag & TEMP_FLAG){ //If temperature ISR has run read temp
    readTemp();
  }
/* 
  checkForCommand();
  if(operationFlag & DECODE_FLAG){
    decodeCommand();
  }
  
  */
  if (wifiSerial.available()) {
    Serial.write(wifiSerial.read());
  }
  if (Serial.available()) {
    wifiSerial.write(Serial.read());
  }
  
}

void readTemp() 
{
  float tempRead = 0;
  float tempC = 0;

  operationFlag &= ~TEMP_FLAG; //turn off temp flag
  
  tempRead = analogRead(A0); //throwaway read so caps can fill
  tempRead = analogRead(A0);
  tempOfSystem = tempRead;
  
  Serial.print("ADC Val: ");  
  Serial.println(tempRead);  //print digital value
  tempC = (tempRead-1137)/-4.17;
  Serial.print("Temp in Fahrenheit: ");
  Serial.println(tempC); 
}

void checkForCommand() 
{
  byte strPosition = 0;
  int incomingByte = 0;
  while (Serial.available() > 0)
  {
      operationFlag |= DECODE_FLAG; // set decode flag
      
      incomingByte = Serial.read();
      cmdStr[strPosition] = incomingByte;
      if (cmdStr[strPosition] == '\r')
      {
        cmdStr[strPosition] = '\0';
        break;
      }    
      strPosition++; 
      delay(25);
  }
}

void decodeCommand()
{
  byte commandRecognized = 0;

  operationFlag &= ~DECODE_FLAG; // turn off decode flag
  
  Serial.println("Decoding Command");
  Serial.println(cmdStr);
  if (strcmp(cmdStr,"TON")==0)
  {
    commandRecognized = 1;
    Serial.println("Turning on Transducer");
    digitalWrite(13,HIGH);
  }
  if (strcmp(cmdStr,"TOFF")==0)
  {
    commandRecognized = 1;
    Serial.println("Turning off Transducer");
    digitalWrite(13,LOW);
  }

  if (commandRecognized == 0){
    Serial.println("Command Not Recognized");
  }
  
}

ISR(TIMER1_COMPA_vect) // Occurs once every 4 seconds
{
  operationFlag |= TEMP_FLAG;
}


