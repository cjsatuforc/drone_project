#include "printf.h"
#include "drone.h"

void setup() 
{
  Serial.begin(115200);
  PID_init();
  RADIO_init();
}


void loop() 
{
    data joystick;
    unsigned long time;
    
    joystick = RADIO_read();
    
    PID_loop(joystick);
    //time = millis();
    //Serial.println(time);

}


