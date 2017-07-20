#include "PID_v1.h"

#define maxPWM 2000
#define minPWM 1000
#define thrust 1500

double roll_setpoint, pitch_setpoint, yaw_setpoint, altitude_coeff;
double roll_angle, pitch_angle, yaw_angular_vel;
double err_roll, err_pitch, err_yaw;
double left_front, right_front, left_back, right_back;

double roll_kp=10, roll_ki=0, roll_kd=0;
double pitch_kp=10, pitch_ki=0, pitch_kd=0;
double yaw_kp=10, yaw_ki=0, yaw_kd=0;

PID roll_PID(&roll_angle, &err_roll, &roll_setpoint, roll_kp, roll_ki, roll_kd, DIRECT);
PID pitch_PID(&pitch_angle, &err_pitch, &pitch_setpoint, pitch_kp, pitch_ki, pitch_kd, DIRECT);
PID yaw_PID(&yaw_angular_vel, &err_yaw, &yaw_setpoint, yaw_kp, yaw_ki, yaw_kd, DIRECT);


void setup()  //void initPID()
{
  Serial.begin(115200);
  
  roll_setpoint = 0;    // these will be replaced by the outputs
  pitch_setpoint = 0;
  yaw_setpoint = 0;
  
  //  roll_PID.SetTunings(roll_kp, roll_ki, roll_kd);         This is for Ben's part for tunning
  //  pitch_PID.SetTunings(pitch_kp, pitch_ki, pitch_kd);     the PID values via the web server
  //  yaw_PID.SetTunings(yaw_kp, yaw_ki, yaw_kd);
  roll_PID.SetMode(AUTOMATIC);
  pitch_PID.SetMode(AUTOMATIC);
  yaw_PID.SetMode(AUTOMATIC);

}

void loop() 
{
    roll_setpoint = 0; // roll_left;
    pitch_setpoint = 0; // pitch_forward;
    yaw_setpoint = 32; // yaw_ccw;
    altitude_coeff = 15; // throttle_up;

    roll_angle = 0; //ypr[2] * 180/M_PI; 
    pitch_angle = 0; //ypr[1] * 180/M_PI;
    yaw_angular_vel = 0; //gyro[2];

    roll_PID.Compute();
    pitch_PID.Compute();
    yaw_PID.Compute();

    right_back = thrust*altitude_coeff - err_pitch + err_roll - err_yaw;
    right_front = thrust*altitude_coeff - err_pitch - err_roll + err_yaw;
    left_back = thrust*altitude_coeff + err_pitch + err_roll + err_yaw;
    left_front = thrust*altitude_coeff + err_pitch - err_roll - err_yaw;

    double PWM_RB = map(right_back, 1500, 50000, 1000, 2000);

    Serial.print(right_back);
    Serial.print("  ");
    Serial.print(right_front);
    Serial.print("  ");
    Serial.print(left_back);
    Serial.print("  ");
    Serial.println(left_front);
    Serial.println(PWM_RB);
    
    // left_front = thrust*altitude_coeff - pitch_setpoint*10 + roll_setpoint*10 - yaw_setpoint;
    // right_front = thrust*altitude_coeff - pitch_setpoint*10 - roll_setpoint*10 + yaw_setpoint;
    // left_back = thrust*altitude_coeff + pitch_setpoint*10 + roll_setpoint*10 + yaw_setpoint;
    // right_back = thrust*altitude_coeff + pitch_setpoint*10 - roll_setpoint*10 - yaw_setpoint;

       
    // set motor limits
    if (right_back > maxPWM) right_back = maxPWM;
    else if (right_back < minPWM) right_back = minPWM;      
        
    if (right_front > maxPWM) right_front = maxPWM;
    else if (right_front < minPWM) right_front = minPWM;    
        
    if (left_back > maxPWM) left_back = maxPWM;
    else if (left_back < minPWM) left_back = minPWM;        
        
    if (left_front > maxPWM) left_front = maxPWM;
    else if (left_front < minPWM) left_front = minPWM;


}
