#include <SPI.h>
#include "RF24.h"
#include "printf.h"
#include "definitions.h"

bool radioNumber = 0;
RF24 radio(7,8);
byte addresses[][6] = {"1Node","2Node"};
int16_t ax_pos[8] = {0,0,0,0,0,0,0,0};
int16_t but_pos[9] = {0,0,0,0,0,0,0,0};

void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
 
  printf_begin();
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  
    radio.startListening();
}



void loop() {

  
  if( radio.available()){
                                                          // Variable for the received timestamp
    while (radio.available()) 
    {                                   // While there is data ready
      radio.read( &joystick, sizeof(joystick) );             // Get the payload
    }
  //Serial.print(joystick.button);
  //Serial.print("  ");
  //Serial.println(joystick.position);

  switch (joystick.button)
        {
          case JS1_X:
            ax_pos[0] = joystick.position;
            break;
          case JS1_Y:
            ax_pos[1] = joystick.position;
            break;
          case JS2_X:
            ax_pos[3] = joystick.position;
            break;
          case JS2_Y:
            ax_pos[4] = joystick.position;
            break;
          case LT:
            ax_pos[2] = joystick.position;
            break;
          case RT:
            ax_pos[5] = joystick.position;
            break;
          case CP_X:
            ax_pos[6] = joystick.position;
            break;
          case CP_Y:
            ax_pos[7] = joystick.position;
            break;

          case BUTTON_A:
            but_pos[0] = joystick.position;
            break;
          case BUTTON_B:
            but_pos[1] = joystick.position;
            break;
          case BUTTON_X:
            but_pos[2] = joystick.position;
            break;
          case BUTTON_Y:
            but_pos[3] = joystick.position;
            break;
          case BUTTON_LB:
            but_pos[4] = joystick.position;
            break;
          case BUTTON_RB:
            but_pos[5] = joystick.position;
            break;
          case BUTTON_BACK:
            but_pos[6] = joystick.position;
            break;
          case BUTTON_START:
            but_pos[7] = joystick.position;
            break;
          case BUTTON_XBOX:
            but_pos[8] = joystick.position;
            break;          
        } 
      
      Serial.print(ax_pos[0]); Serial.print("   ");
      Serial.print(ax_pos[1]); Serial.print("   ");
      Serial.print(ax_pos[3]); Serial.print("   ");
      Serial.print(ax_pos[4]); Serial.print("   ");
      Serial.print(ax_pos[2]); Serial.print("   ");
      Serial.print(ax_pos[5]); Serial.print("   ");
      Serial.print(ax_pos[6]); Serial.print("   ");
      Serial.print(ax_pos[7]); Serial.print("   ");
      
      Serial.print(but_pos[0]); Serial.print("   ");
      Serial.print(but_pos[1]); Serial.print("   ");
      Serial.print(but_pos[2]); Serial.print("   ");
      Serial.print(but_pos[3]); Serial.print("   ");
      Serial.print(but_pos[4]); Serial.print("   ");
      Serial.print(but_pos[5]); Serial.print("   ");
      Serial.print(but_pos[6]); Serial.print("   ");
      Serial.print(but_pos[7]); Serial.print("   ");
      Serial.println(but_pos[8]); Serial.print("   ");
   }

//unsigned long time = millis();
//Serial.println(time);


}

