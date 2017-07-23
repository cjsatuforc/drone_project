#include "drone.h"
#include "PID_v1.h"
#include "RF24.h"
#include "definitions.h"
#include <SPI.h>


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
  radio.setChannel(1);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  radio.startListening();
}

void PID_loop(data joystick)
{
    roll_setpoint = (joystick.X2 -512)/10; // roll_left;
    pitch_setpoint = (joystick.Y2 -538)/10; // pitch_forward;
    yaw_setpoint = (joystick.X1 -500)/10; // yaw_ccw;
    altitude_coeff = (joystick.Y1 -505)/10; // throttle_up;

    roll_angle = 0; //ypr[2] * 180/M_PI; 
    pitch_angle = 0; //ypr[1] * 180/M_PI;
    yaw_angular_vel = 0; //gyro[2];

    roll_PID.Compute();
    pitch_PID.Compute();
    yaw_PID.Compute();

    right_back = thrust*altitude_coeff - 10*err_pitch + 10*err_roll - 10*err_yaw;
    right_front = thrust*altitude_coeff - 10*err_pitch - 10*err_roll + 10*err_yaw;
    left_back = thrust*altitude_coeff + 10*err_pitch + 10*err_roll + 10*err_yaw;
    left_front = thrust*altitude_coeff + 10*err_pitch - 10*err_roll - 10*err_yaw;
    


//        Serial.print(altitude_coeff);
//        Serial.print("   ");
//        Serial.print(yaw_setpoint);
//        Serial.print("   ");
//        Serial.print(pitch_setpoint);
//        Serial.print("   ");
//        Serial.println(roll_setpoint);


    right_back = map(right_back, 0, 77000, 1000, 2000);
    right_front = map(right_front, 0, 77000, 1000, 2000);
    left_back = map(left_back, 0, 77000, 1000, 2000);
    left_front = map(left_front, 0, 77000, 1000, 2000);
    
        Serial.print(right_back);
        Serial.print("   ");
        Serial.print(right_front);
        Serial.print("   ");
        Serial.print(left_back);
        Serial.print("   ");
        Serial.println(left_front);
    
    // set motor limits

    //set PWM
    
    
}

data RADIO_read()
{
   data joystick;
      if( radio.available())
      {
                                                                                
        radio.read( &joystick, sizeof(joystick) ); 
      
//        Serial.print(joystick.X1);
//        Serial.print("   ");
//        Serial.print(joystick.Y1);
//        Serial.print("   ");
//        Serial.print(joystick.X2);
//        Serial.print("   ");
//        Serial.println(joystick.Y2);
//  
//        got_time = millis();
//        Serial.println(got_time);
        
      }
  return joystick;
}
