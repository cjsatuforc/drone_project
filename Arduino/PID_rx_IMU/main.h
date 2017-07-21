#ifndef _MAIN_H
#define _MAIN_H


#define M_PI   3.14159265358979323846264338327950288

#define INTERRUPT_PIN 2

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
int16_t gyro[3];
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

int16_t IMUyaw;
float IMUpitch, IMUroll;


double js_roll, js_pitch, js_yaw, js_throttle;
motor PWMmotor;
int16_t ax_pos[8] = {0,0,0,0,0,0,0,0};
int16_t but_pos[9] = {0,0,0,0,0,0,0,0};

#endif /* _MAIN_H */
