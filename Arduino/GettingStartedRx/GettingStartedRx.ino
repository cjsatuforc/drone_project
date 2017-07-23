#include <SPI.h>
#include "RF24.h"

RF24 radio(7,8);

byte addresses[][6] = {"1Node","2Node"};

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  
  radio.startListening();
}

void loop() {
 

    unsigned long got_time;
    
    if( radio.available()){
                                                                    // Variable for the received timestamp                     
        radio.read( &got_time, sizeof(unsigned long) );             // Get the payload

     
//      radio.stopListening();                                        // First, stop listening so we can talk   
//      radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.      
//      radio.startListening();                                       // Now, resume listening so we catch the next packets.     
//      Serial.print(F("Sent response "));
      Serial.println(got_time);  
   }
 





} 

