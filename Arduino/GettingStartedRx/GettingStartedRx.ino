#include <SPI.h>
#include "RF24.h"

RF24 radio(7,8);

struct data
{
  int X1, Y1, X2, Y2;
} joystick;

byte addresses[][6] = {"1Node","2Node"};

void setup() {
  Serial.begin(115200);
  Serial.print("Serial");
  radio.begin();  
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_LOW);
  //radio.setCRCLength(RF24_CRC_DISABLED);
  
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  
  radio.startListening();
}

void loop() {
 
 //Serial.print("loop");
    unsigned long got_time;
    //unsigned long X1;
    
    if( radio.available()){
                                                                    // Variable for the received timestamp                     
        radio.read( &joystick, sizeof(joystick) );             // Get the payload

     
//      radio.stopListening();                                        // First, stop listening so we can talk   
//      radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.      
//      radio.startListening();                                       // Now, resume listening so we catch the next packets.     
//      Serial.print(F("Sent response "));
      Serial.print(joystick.X1);
      Serial.print("   ");
      Serial.print(joystick.Y1);
      Serial.print("   ");
      Serial.print(joystick.X2);
      Serial.print("   ");
      Serial.println(joystick.Y2);

      got_time = millis();
      Serial.println(got_time);
   }
 





} 

