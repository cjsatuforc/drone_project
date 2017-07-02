#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include "definitions.h"

using namespace std;
int16_t divide_func(int16_t iposition, int16_t position_old);

RF24 radio(22,0);
bool radioNumber = 1;

const uint8_t pipes[][6] = {"1Node","2Node"};
int16_t position_old, iposition;

int main(int argc, char** argv){
//Setting up radio stuff
	cout << "RF24/examples/GettingStarted/\n";
	radio.begin();

	radio.setRetries(15,15);
	radio.printDetails();

	radio.openWritingPipe(pipes[1]);
	radio.openReadingPipe(1,pipes[0]);
		
	joystick.button = JS1_X;
	joystick.position = 1;

//Controller stuff
	int joy_fd, num_of_axis=0, num_of_buttons=0;
	char name_of_joystick[80];
	struct js_event js, oldjs;

	if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
	{
		printf( "Couldn't open joystick\n" );
		return -1;
	}

	ioctl( joy_fd, JSIOCGAXES, &num_of_axis );
	ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons );
	ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick );

	//axis = (int *) calloc( num_of_axis, sizeof( int ) );
	//button = (char *) calloc( num_of_buttons, sizeof( char ) );

	printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
		, name_of_joystick
		, num_of_axis
		, num_of_buttons );

	fcntl( joy_fd, F_SETFL, O_NONBLOCK );	/* use non-blocking mode */



	while(1)
	{
		read(joy_fd, &js, sizeof(struct js_event)); //read js state

		if((js.type & ~JS_EVENT_INIT) == JS_EVENT_AXIS)
		{
			switch (js.number)
			{
				case 0:
					joystick.button = JS1_X;
   					iposition = js.value;
   					joystick.position = divide_func(iposition, position_old);
					break;
				case 1:
					joystick.button = JS1_Y;
   					iposition = js.value;
   					joystick.position = divide_func(iposition, position_old);
					break;
				case 2:
					joystick.button = LT;
   					iposition = js.value;
   					joystick.position = divide_func(iposition, position_old);
					break;
				case 3:
					joystick.button = JS2_X;
   					iposition = js.value;
   					joystick.position = divide_func(iposition, position_old);
					break;
				case 4:
					joystick.button = JS2_Y;
   					iposition = js.value;
   					joystick.position = divide_func(iposition, position_old);
					break;
				case 5:
					joystick.button = RT;
   					iposition = js.value;
   					joystick.position = divide_func(iposition, position_old);
					break;
				case 6:
					joystick.button = CP_X;
   					iposition = js.value;
   					joystick.position = divide_func(iposition, position_old);
					break;
				case 7:
					joystick.button = CP_Y;
   					iposition = js.value;
   					joystick.position = divide_func(iposition, position_old);
					break;
			} 
		position_old = iposition;
		}
		

		else if((js.type & ~JS_EVENT_INIT) == JS_EVENT_BUTTON)
		{
			switch (js.number)
			{
				case 0:
					joystick.button = BUTTON_A;
   					joystick.position = js.value;
   					printf("BUTTON_A   %u   \n", js.value);
					break;
				case 1:
					joystick.button = BUTTON_B;
   					joystick.position = js.value;
					break;
				case 2:
					joystick.button = BUTTON_X;
   					joystick.position = js.value;
					break;
				case 3:
					joystick.button = BUTTON_Y;
   					joystick.position = js.value;
					break;
				case 4:
					joystick.button = BUTTON_LB;
   					joystick.position = js.value;
					break;
				case 5:
					joystick.button = BUTTON_RB;
   					joystick.position = js.value;
					break;
				case 6:
					joystick.button = BUTTON_BACK;
   					joystick.position = js.value;
					break;
				case 7:
					joystick.button = BUTTON_START;
   					joystick.position = js.value;
					break;
				case 8:
					joystick.button = BUTTON_XBOX;
   					joystick.position = js.value;
					break;
			} 
		}












		//printf("Now sending...\n");
		if(js.number != oldjs.number)
		{
			bool ok = radio.write( &joystick, sizeof(joystick) );
			if (!ok)
			{
				printf("failed.\n   ");
				
			}
			else if (ok)
			{
				printf("sent!  ");
				printf("%u    \n" , joystick.button  );}
				
			}
		oldjs = js;

	}

	close( joy_fd );
	return 0;
}

int16_t divide_func(int16_t iposition, int16_t position_old)
{
	if(abs((iposition - position_old) < 200 ))
	{
		joystick.position = joystick.position;
	}

	if((iposition > (-4000)) && (iposition < (0)))
	{
		joystick.position = 0; 
	}
	else if((iposition < 4000) && (iposition > (0)))
	{
		joystick.position = 0; 
	}

	else
	{
		joystick.position = iposition/1000; 
	}

	return joystick.position;
}