#include <Servo.h>

Servo esc1; 
Servo esc2; 


void setup() {
  delay(5000);
  esc1.attach(9,1000,2000);

  
  pinMode(13, OUTPUT); 

delay(500);
  
  esc1.writeMicroseconds(2000);                                 
  delay(3000);  
  esc1.writeMicroseconds(1000);                             
  delay(3000);

}

void loop() {
  //  esc1.writeMicroseconds(1050);       
//  delay(3000);

   
}
