#ifndef DRONE_H_
#define DRONE_H_
#include <stdint.h>

#define byte uint8_t




#define maxPWM 2000
#define minPWM 1000
#define thrust 1500




void PID_init();
void PID_loop(double js_roll, double js_pitch, double js_yaw, double js_throttle);
void RADIO_init();
void RADIO_read();

#endif /* DRONE_H_ */
