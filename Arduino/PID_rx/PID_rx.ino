#include "printf.h"
#include "drone.h"

double js_roll, js_pitch, js_yaw, js_throttle;

void setup() 
{
  Serial.begin(115200);
  PID_init();
  RADIO_init();
}


void loop() 
{

    RADIO_read();

    
    
    PID_loop(js_roll, js_pitch, js_yaw, js_throttle);


    /*
    Serial.print(right_back);
    Serial.print("  ");
    Serial.print(right_front);
    Serial.print("  ");
    Serial.print(left_back);
    Serial.print("  ");
    Serial.println(left_front);
    Serial.println(PWM_RB);
*/
}


