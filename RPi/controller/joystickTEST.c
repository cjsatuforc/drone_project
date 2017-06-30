#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include "definitions.h"


#define JOY_DEV "/dev/input/js0"

int main()
{
	int joy_fd, *axis=NULL, num_of_axis=0, num_of_buttons=0, x;
	char *button=NULL, name_of_joystick[80];
	struct js_event js;

	if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
	{
		printf( "Couldn't open joystick\n" );
		return -1;
	}

	ioctl( joy_fd, JSIOCGAXES, &num_of_axis );
	ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons );
	ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick );

	axis = (int *) calloc( num_of_axis, sizeof( int ) );
	button = (char *) calloc( num_of_buttons, sizeof( char ) );

	printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
		, name_of_joystick
		, num_of_axis
		, num_of_buttons );

	fcntl( joy_fd, F_SETFL, O_NONBLOCK );	/* use non-blocking mode */

	while( 1 ) 	/* infinite loop */
	{

			/* read the joystick state */
		read(joy_fd, &js, sizeof(struct js_event));
		
			/* see what to do with the event */
		switch (js.type & ~JS_EVENT_INIT)
		{
			case JS_EVENT_AXIS:
				axis   [ js.number ] = js.value;
				break;
			case JS_EVENT_BUTTON:
				button [ js.number ] = js.value;
				break;
		}

			/* print the results */
		printf( "X1: %6d  Y1: %6d  ", axis[0], axis[1] );
		printf( "X2: %6d  Y2: %6d  ", axis[3], axis[4] );
		printf( "LT: %6d  RT: %6d  ", axis[2], axis[5] );
		printf( "PADX: %6d  PADY: %6d  ", axis[6], axis[7] );
		

		printf("A: %d  ", button[0] );
		printf("B: %d  ", button[1] );
		printf("X: %d  ", button[2] );
		printf("Y: %d  ", button[3] );
		printf("LB: %d  ", button[4] );
		printf("RB: %d  ", button[5] );
		printf("BACK: %d  ", button[6] );
		printf("START: %d  ", button[7] );
		printf("XBOX: %d  ", button[8] );
		printf("STICK1: %d  ", button[9] );
		printf("STICK2: %d  ", button[10] );

		printf("  \r");
		fflush(stdout);
	}

	close( joy_fd );	/* too bad we never get here */
	return 0;
}