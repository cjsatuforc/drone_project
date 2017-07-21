#ifndef IMU_H_
#define IMU_H_
#include <stdint.h>

#define M_PI   3.14159265358979323846264338327950288

#define INTERRUPT_PIN 2

bool IMU_init();
void IMU_read();
bool dmpDataReady();

// MPU control/status vars




#endif /* IMU_H_ */
