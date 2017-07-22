#include <Servo.h>

Servo esc1; 
Servo esc2; 
Servo esc3; 
Servo esc4;

void setup() {
  delay(5000);
  esc1.attach(9,1000,2000);
  esc2.attach(6,1000,2000);
  esc3.attach(5,1000,2000);
  esc4.attach(3,1000,2000);

delay(500);
  
  esc1.writeMicroseconds(1000);                  
  esc2.writeMicroseconds(1000);
  esc3.writeMicroseconds(1000);                  
  esc4.writeMicroseconds(1000);                  
  delay(1000);  
  esc1.writeMicroseconds(1050);                  
  esc2.writeMicroseconds(1050);
  esc3.writeMicroseconds(1050);                  
  esc4.writeMicroseconds(1050);
  delay(5000);
}

void loop() {
  
  esc1.writeMicroseconds(1080);                  
  esc2.writeMicroseconds(1080);            

   
}
