#include <Servo.h>

Servo esc1; 
Servo esc2; 


void setup() {
  delay(5000);
  esc1.attach(9,1000,2000);
  esc2.attach(10,1000,2000);
  
  pinMode(13, OUTPUT); 

delay(500);
  
  esc1.writeMicroseconds(1000);                  
  esc2.writeMicroseconds(1000);                  
  delay(1000);  
  esc1.writeMicroseconds(105 0);                  
  esc2.writeMicroseconds(1050);                  
  delay(1000);  
    
}

void loop() {
  
  digitalWrite(13,1);            
  delay(250);
  digitalWrite(13,0);            
  delay(250);
   
}
