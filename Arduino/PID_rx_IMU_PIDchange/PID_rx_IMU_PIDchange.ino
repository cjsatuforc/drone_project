#include "printf.h"
#include "drone.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include "MPU6050.h"

//#define M_PI 3.14159265358979323846264338327950288

MPU6050 mpu;

uint8_t mpuIntStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
float ypr[3];
int16_t gyro[3];
unsigned long time;
uint8_t devStatus; 

int16_t IMUyaw;
float IMUpitch, IMUroll;


void setup() 
{
  Serial.begin(115200);
  PID_init();
  RADIO_init();
  IMU_init();
  motors_init();
}


void loop() 
{
//  if (devStatus)
//  {
//    while(1)
//    {}
//  }
  fifoCount = mpu.getFIFOCount();
  if (fifoCount < packetSize)
  {
      data joystick;
      joystick = RADIO_read();

      IMUyaw  = 0;//gyro[2]* -1;
      IMUpitch = ypr[1] * 180/M_PI;
      IMUroll = ypr[2] * 180/M_PI;

//      Serial.print("ypr\t");
//      Serial.print(IMUyaw);
//      Serial.print("\t");
//      Serial.print(IMUpitch);
//      Serial.print("\t");
//      Serial.print(IMUroll);
//      Serial.println();

      PID_loop(joystick, IMUyaw, IMUpitch, IMUroll);
    
      //time = millis();
      //Serial.println(time);
      fifoCount = mpu.getFIFOCount();
  }
  else if (fifoCount == 1024) mpu.resetFIFO(); 
  
  else if (fifoCount > packetSize)
  {
      data joystick;
      joystick = RADIO_read();

      IMUyaw  = 0; //gyro[2]* -1;
      IMUpitch = ypr[1] * 180/M_PI;
      IMUroll = ypr[2] * 180/M_PI;

//      Serial.print("ypr\t");
//      Serial.print(IMUyaw);
//      Serial.print("\t");
//      Serial.print(IMUpitch);
//      Serial.print("\t");
//      Serial.print(IMUroll);
//      Serial.println();

      PID_loop(joystick, IMUyaw, IMUpitch, IMUroll);
    
      //time = millis();
      //Serial.println(time);
      fifoCount = mpu.getFIFOCount();
      
      mpu.getFIFOBytes(fifoBuffer, packetSize);

      
       mpu.resetFIFO();
      fifoCount -= packetSize;
     

      mpu.dmpGetQuaternion(&q, fifoBuffer);
      //mpu.dmpGetGyro(gyro,fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
     
     
//      Serial.print("ypr\t");
//      Serial.print(ypr[0]*180/PI);
//      Serial.print("\t");
//      Serial.print(ypr[1]*180/PI);
//      Serial.print("\t");
//      Serial.print(ypr[2]*180/PI);
//      Serial.println();
  } 

}


void IMU_init()
{
    Wire.begin();
    //Wire.setClock(400000L);
    mpu.initialize();
    devStatus = mpu.dmpInitialize();
    //Serial.print("IMU initialise code: ");
    //Serial.println(devStatus);
    
    mpu.setXAccelOffset(-1343);
    mpu.setYAccelOffset(-1155);
    mpu.setZAccelOffset(1033);
    mpu.setXGyroOffset(19);
    mpu.setYGyroOffset(-27);
    mpu.setZGyroOffset(16);
    mpu.setDMPEnabled(true);
    packetSize = mpu.dmpGetFIFOPacketSize();
}
