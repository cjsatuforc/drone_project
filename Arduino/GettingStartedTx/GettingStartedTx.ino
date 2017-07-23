#include <SPI.h>
#include "RF24.h"

RF24 radio(7,8);

byte addresses[][6] = {"1Node","2Node"};

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);

  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
}


 unsigned long i = 0; 
void loop() {
 
    for (i=0; i<100; i++)
    {                        
      radio.write( &i, sizeof(unsigned long));   
    }

} 

