#include <SPI.h>
#include "RF24.h"

#define JOY_X1 A0
#define JOY_Y1 A1
#define JOY_X2 A2
#define JOY_Y2 A3
#define buttoninc 3
#define buttondec 4
#define buttonswap 2

RF24 radio(7,8);

struct data
{
  int X1, Y1, X2, Y2;
  int PIDax;
  double PID;
} joystick;

byte addresses[][6] = {"1Node","2Node"};
bool ok;
unsigned long i = 0; 
int butinc, butdec, oldbutinc, oldbutdec, butswap, oldbutswap;

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_LOW); ///MAX
  //radio.setCRCLength(RF24_CRC_DISABLED);
  pinMode(buttoninc, INPUT);
  pinMode(buttondec, INPUT);
  pinMode(buttonswap, INPUT);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  // radio.powerUp();
  joystick.PIDax = 1;
  
}


void loop() {
  joystick.X1 = analogRead(JOY_X1);
  joystick.Y1 = analogRead(JOY_Y1);
  joystick.X2 = analogRead(JOY_X2);
  joystick.Y2 = analogRead(JOY_Y2);
  butinc = digitalRead(buttoninc);
  butdec = digitalRead(buttondec);
  butswap = digitalRead(buttonswap);
  
  if(butinc == 0 & oldbutinc == 1)
    joystick.PID = joystick.PID + 0.1;
  if(butdec == 0 & oldbutdec == 1)
    joystick.PID = joystick.PID - 0.1;
  if(butswap == 0 & oldbutswap == 1)
  {
    joystick.PID = 0;
    if (joystick.PIDax == 3)
      joystick.PIDax = 1;
    else
      joystick.PIDax++;
  }
    

    
  oldbutinc = butinc;
  oldbutdec = butdec;
  oldbutswap = butswap;
  
  ok = radio.write( &joystick, sizeof(joystick)); 
//  Serial.print(joystick.X1);
//  Serial.print("\t");
//  Serial.print(joystick.Y1);  
//  Serial.print("\t");
//  Serial.print(joystick.X2);  
//  Serial.print("\t");
//  Serial.print(joystick.Y2);    
//  Serial.print("\t");
  Serial.print(joystick.PID);  
  Serial.print("\t");
  Serial.println(joystick.PIDax); 

} 

