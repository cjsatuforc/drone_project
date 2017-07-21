#include "printf.h"
#include "drone.h"
#include "IMU.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

double js_roll, js_pitch, js_yaw, js_throttle;
motor PWMmotor;
int16_t ax_pos[8] = {0,0,0,0,0,0,0,0};
int16_t but_pos[9] = {0,0,0,0,0,0,0,0};
bool dmpReady = false;



MPU6050 mpu; //MPU6050 mpu(0x69); <-- use for AD0 high

// MPU control/status vars

uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high


void setup() 
{
  Serial.begin(115200);
  PID_init();
  RADIO_init();
  dmpReady = IMU_init();
}


void loop() 
{

    if (!dmpReady) return;

    while (!mpuInterrupt && fifoCount < packetSize) {
    
      js_throttle = ax_pos[1];
      js_yaw = ax_pos[0];
      js_pitch = ax_pos[3];
      js_roll = ax_pos[4];
      
      PWMmotor = PID_loop(js_roll, js_pitch, js_yaw, js_throttle);
 
      Serial.print(PWMmotor.PWM_RB);
      Serial.print("  ");
      Serial.print(PWMmotor.PWM_RF);
      Serial.print("  ");
      Serial.print(PWMmotor.PWM_LB);
      Serial.print("  ");
      Serial.println(PWMmotor.PWM_LF); 

    }

    IMU_read();

}

void dmpDataReady() 
{
    mpuInterrupt = true;
}





void IMU_read()
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
            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);
            Serial.println(mpuInterrupt);
           
           
    }
}

bool IMU_init()
{
    bool dmpReady = false;  // set true if DMP init was successful
    
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
    return dmpReady;
}
