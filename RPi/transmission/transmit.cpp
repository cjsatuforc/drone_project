#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>

using namespace std;

RF24 radio(22,0);
bool radioNumber = 1;

const uint8_t pipes[][6] = {"1Node","2Node"};


int main(int argc, char** argv){


	cout << "RF24/examples/GettingStarted/\n";
	radio.begin();

	radio.setRetries(15,15);
	radio.printDetails();


	radio.openWritingPipe(pipes[1]);
	radio.openReadingPipe(1,pipes[0]);

	while (1)
	{
		printf("Now sending...\n");
		unsigned long time = millis();

		bool ok = radio.write( &time, sizeof(unsigned long) );

		if (!ok){
			printf("failed.\n");
		}
		else if (ok){
			printf("sent!\n");
		}
	}

  return 0;
}

