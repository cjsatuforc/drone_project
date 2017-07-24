
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
MPU6050 mpu;
uint8_t mpuIntStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
float ypr[3];
unsigned long time; 

void setup() {

Serial.begin(115200);

    Wire.begin();
    Wire.setClock(400000L);
    mpu.initialize();
    mpu.dmpInitialize();
    mpu.setXAccelOffset(-1343);
    mpu.setYAccelOffset(-1155);
    mpu.setZAccelOffset(1033);
    mpu.setXGyroOffset(19);
    mpu.setYGyroOffset(-27);
    mpu.setZGyroOffset(16);
    mpu.setDMPEnabled(true);
    packetSize = mpu.dmpGetFIFOPacketSize();

    


}


void loop() {
  while (fifoCount < packetSize) {
    //do stuff
      time = millis();
      Serial.println(time);
    fifoCount = mpu.getFIFOCount();
  }

  if (fifoCount == 1024) mpu.resetFIFO(); 
  
  else {
      fifoCount = mpu.getFIFOCount();
      
      mpu.getFIFOBytes(fifoBuffer, packetSize);


      mpu.resetFIFO(); 
      
      fifoCount -= packetSize;


        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        
       
        Serial.print("ypr\t");
        Serial.print(ypr[0]*180/PI);
        Serial.print("\t");
        Serial.print(ypr[1]*180/PI);
        Serial.print("\t");
        Serial.print(ypr[2]*180/PI);
        Serial.println();
    
  } 


}
