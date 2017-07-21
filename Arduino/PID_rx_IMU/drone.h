#ifndef DRONE_H_
#define DRONE_H_
#include <stdint.h>

#define byte uint8_t

struct motor {
  double PWM_RB, PWM_RF, PWM_LB, PWM_LF;
};


#define maxPWM 2000
#define minPWM 1000
#define thrust 1500




void PID_init();
motor PID_loop(double js_roll, double js_pitch, double js_yaw, double js_throttle, int16_t IMUyaw, float IMUpitch, float IMUroll);
void RADIO_init();
void RADIO_read(int16_t* ax_pos, int16_t* but_pos);

#endif /* DRONE_H_ */
