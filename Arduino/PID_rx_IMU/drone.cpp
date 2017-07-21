#include "drone.h"
#include "PID_v1.h"
#include "RF24.h"
#include "definitions.h"
#include <SPI.h>


bool radioNumber = 0;

byte addresses[][6] = {"1Node","2Node"};



double left_front, right_front, left_back, right_back;  
double PWM_RB, PWM_RF, PWM_LB, PWM_LF;

double roll_setpoint, pitch_setpoint, yaw_setpoint, altitude_coeff;
double roll_angle, pitch_angle, yaw_angular_vel;
double err_roll, err_pitch, err_yaw;

double roll_kp = 10, roll_ki = 0, roll_kd = 0;
double pitch_kp = 10, pitch_ki = 0, pitch_kd = 0;
double yaw_kp = 10, yaw_ki = 0, yaw_kd = 0;


PID roll_PID(&roll_angle, &err_roll, &roll_setpoint, roll_kp, roll_ki, roll_kd, DIRECT);
PID pitch_PID(&pitch_angle, &err_pitch, &pitch_setpoint, pitch_kp, pitch_ki, pitch_kd, DIRECT);
PID yaw_PID(&yaw_angular_vel, &err_yaw, &yaw_setpoint, yaw_kp, yaw_ki, yaw_kd, DIRECT);

RF24 radio(7,8);





void PID_init()
{

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

void RADIO_init()
{
  
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  radio.startListening();
}

motor PID_loop(double js_roll, double js_pitch, double js_yaw, double js_throttle, int16_t IMUyaw, float IMUpitch, float IMUroll)
{
    motor PWMmotor;

    roll_setpoint = js_roll; // roll_left;
    pitch_setpoint = js_pitch; // pitch_forward;
    yaw_setpoint = js_yaw; // yaw_ccw;
    altitude_coeff = js_throttle; // throttle_up;

    roll_angle = IMUroll; //ypr[2] * 180/M_PI; 
    pitch_angle = IMUpitch; //ypr[1] * 180/M_PI;
    yaw_angular_vel = IMUyaw; //gyro[2];


    roll_PID.Compute();
    pitch_PID.Compute();
    yaw_PID.Compute();

    right_back = thrust*altitude_coeff - 10*err_pitch + 10*err_roll - 10*err_yaw;
    right_front = thrust*altitude_coeff - 10*err_pitch - 10*err_roll + 10*err_yaw;
    left_back = thrust*altitude_coeff + 10*err_pitch + 10*err_roll + 10*err_yaw;
    left_front = thrust*altitude_coeff + 10*err_pitch - 10*err_roll - 10*err_yaw;

    PWMmotor.PWM_RB = map(right_back, 1500, 50000, 1000, 2000);
    PWMmotor.PWM_RF = map(right_front, 1500, 50000, 1000, 2000);
    PWMmotor.PWM_LB = map(left_back, 1500, 50000, 1000, 2000);
    PWMmotor.PWM_LF = map(left_front, 1500, 50000, 1000, 2000);
    
    // left_front = thrust*altitude_coeff - pitch_setpoint*10 + roll_setpoint*10 - yaw_setpoint;
    // right_front = thrust*altitude_coeff - pitch_setpoint*10 - roll_setpoint*10 + yaw_setpoint;
    // left_back = thrust*altitude_coeff + pitch_setpoint*10 + roll_setpoint*10 + yaw_setpoint;
    // right_back = thrust*altitude_coeff + pitch_setpoint*10 - roll_setpoint*10 - yaw_setpoint;
    
    // set motor limits
    /* if (PWMmotor.PWM_RB > maxPWM) PWMmotor.PWM_RB = maxPWM;
    else if (PWMmotor.PWM_RB < minPWM) PWMmotor.PWM_RB = minPWM;      
        
    if (PWMmotor.PWM_RF > maxPWM) PWMmotor.PWM_RF = maxPWM;
    else if (PWMmotor.PWM_RF < minPWM) PWMmotor.PWM_RF = minPWM;    
        
    if (PWMmotor.PWM_LB > maxPWM) PWMmotor.PWM_LB = maxPWM;
    else if (PWMmotor.PWM_LB < minPWM) PWMmotor.PWM_LB = minPWM;        
        
    if (PWMmotor.PWM_LF > maxPWM) PWMmotor.PWM_LF = maxPWM;
    else if (PWMmotor.PWM_LF < minPWM) PWMmotor.PWM_LF = minPWM;
*/
    return PWMmotor;
    
}

void RADIO_read(int16_t* ax_pos, int16_t* but_pos)
{
  
  //int16_t ax_pos[8] = {0,0,0,0,0,0,0,0};
  //int16_t but_pos[9] = {0,0,0,0,0,0,0,0};
  
    if( radio.available()){
                                                          // Variable for the received timestamp
    while (radio.available()) 
    {                                   // While there is data ready
      radio.read( &joystick, sizeof(joystick) );             // Get the payload
    }
  //Serial.print(joystick.button);
  //Serial.print("  ");
  //Serial.println(joystick.position);

  switch (joystick.button)
        {
          case JS1_X:
            ax_pos[0] = joystick.position *-1;
            break;
          case JS1_Y:
            ax_pos[1] = joystick.position *-1;
            break;
          case JS2_X:
            ax_pos[3] = joystick.position;
            break;
          case JS2_Y:
            ax_pos[4] = joystick.position *-1;
            break;
          case LT:
            ax_pos[2] = joystick.position;
            break;
          case RT:
            ax_pos[5] = joystick.position;
            break;
          case CP_X:
            ax_pos[6] = joystick.position;
            break;
          case CP_Y:
            ax_pos[7] = joystick.position;
            break;

          case BUTTON_A:
            but_pos[0] = joystick.position;
            break;
          case BUTTON_B:
            but_pos[1] = joystick.position;
            break;
          case BUTTON_X:
            but_pos[2] = joystick.position;
            break;
          case BUTTON_Y:
            but_pos[3] = joystick.position;
            break;
          case BUTTON_LB:
            but_pos[4] = joystick.position;
            break;
          case BUTTON_RB:
            but_pos[5] = joystick.position;
            break;
          case BUTTON_BACK:
            but_pos[6] = joystick.position;
            break;
          case BUTTON_START:
            but_pos[7] = joystick.position;
            break;
          case BUTTON_XBOX:
            but_pos[8] = joystick.position;
            break;          
        }
    }
}
