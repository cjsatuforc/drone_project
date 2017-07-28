#ifndef DRONE_H_
#define DRONE_H_
#include <stdint.h>

#define byte uint8_t

struct data
{
  int X1, Y1, X2, Y2, PIDax;
  double PID;
};

#define maxPWM 2000
#define minPWM 1000
#define thrust 1500




void PID_init();
void PID_loop(data joystick, int16_t IMUyaw, float IMUpitch, float IMUroll);
void RADIO_init();
data RADIO_read();
void motors_init();
void come_down(double left_front, double right_front, double left_back, double right_back);

#endif /* DRONE_H_ */
