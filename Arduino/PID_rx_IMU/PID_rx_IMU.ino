#include "printf.h"
#include "drone.h"
#include "IMU.h"


double js_roll, js_pitch, js_yaw, js_throttle;
motor PWMmotor;
int16_t ax_pos[8] = {0,0,0,0,0,0,0,0};
int16_t but_pos[9] = {0,0,0,0,0,0,0,0};
bool dmpReady = false;







void setup() 
{
  Serial.begin(115200);
  PID_init();
  RADIO_init();
  dmpReady = IMU_init();
}


void loop() 
{

    if (!dmpReady) return;

    while (!mpuInterrupt && fifoCount < packetSize) {
    
      js_throttle = ax_pos[1];
      js_yaw = ax_pos[0];
      js_pitch = ax_pos[3];
      js_roll = ax_pos[4];
      
      PWMmotor = PID_loop(js_roll, js_pitch, js_yaw, js_throttle);
 
      Serial.print(PWMmotor.PWM_RB);
      Serial.print("  ");
      Serial.print(PWMmotor.PWM_RF);
      Serial.print("  ");
      Serial.print(PWMmotor.PWM_LB);
      Serial.print("  ");
      Serial.println(PWMmotor.PWM_LF); 

    }

    IMU_read();

}


