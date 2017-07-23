#ifndef DRONE_H_
#define DRONE_H_
#include <stdint.h>

#define byte uint8_t

struct data
{
  int X1, Y1, X2, Y2;
};

#define maxPWM 2000
#define minPWM 1000
#define thrust 1500




void PID_init();
void PID_loop(data joystick);
void RADIO_init();
data RADIO_read();

#endif /* DRONE_H_ */
