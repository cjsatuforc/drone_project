#include "printf.h"
#include "drone.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "MPU6050.h"
#include "Wire.h"
#include "main.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu; //MPU6050 mpu(0x69); <-- use for AD0 high



void dmpDataReady() 
{
    mpuInterrupt = true;
}


void setup() 
{
  Serial.begin(115200);
  PID_init();
  RADIO_init();
  IMU_init();
}


void loop() 
{
    if (!dmpReady) return;

   
      RADIO_read(ax_pos, but_pos);
      
      js_throttle = ax_pos[1];
      js_yaw = ax_pos[0];
      js_pitch = ax_pos[3];
      js_roll = ax_pos[4];

      IMUyaw  = gyro[2]* -1;
      IMUpitch = ypr[1] * 180/M_PI;
      IMUroll = ypr[2] * 180/M_PI;

            
      PWMmotor = PID_loop(js_roll, js_pitch, js_yaw, js_throttle, IMUyaw, IMUpitch, IMUroll);
      //SET SERVOS PWM    
        
    IMU_read(ypr);
    unsigned long time = millis();
    //Serial.print(time);
    /*
            Serial.print("ypr\t");
            //Serial.print(ypr[0] * 180/M_PI);
            Serial.print(gyro[2]);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);
     */
    
    Serial.print(PWMmotor.PWM_RB);
    Serial.print("  ");
    Serial.print(PWMmotor.PWM_RF);
    Serial.print("  ");
    Serial.print(PWMmotor.PWM_LB);
    Serial.print("  ");
    Serial.println(PWMmotor.PWM_LF); 


}






void IMU_init()
{
    Wire.begin();
    Wire.setClock(400000);

    while (!Serial);

    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) 
    {
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } 
    
    else 
    {

        Serial.print(F("DMP Initialization failed (code "));        
          // 1 = initial memory load failed
          // 2 = DMP configuration updates failed
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}


void IMU_read(float* ypr)
{
    mpuInterrupt = false; // reset interrupt flag and get INT_STATUS byte
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) 
    {
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));
    } 
    
    else if (mpuIntStatus & 0x02) 
    {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;

            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            mpu.dmpGetGyro(gyro,fifoBuffer);
           
            /*
            Serial.print("ypr\t");
            //Serial.print(ypr[0] * 180/M_PI);
            Serial.print(gyro[2]);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);               
            */
    }

  }
