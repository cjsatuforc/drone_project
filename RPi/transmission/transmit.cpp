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

  //bool role_ping_out = true, role_pong_back = false;
 // bool role = role_pong_back;

  cout << "RF24/examples/GettingStarted/\n";
  radio.begin();

  radio.setRetries(15,15);
  radio.printDetails();


/********* Role chooser ***********/

 // printf("\n ************ Role Setup ***********\n");
  //string input = "";
  //char myChar = {0};
 // cout << "Choose a role: Enter 0 for pong_back, 1 for ping_out (CTRL+C to exit) \n>";
  //getline(cin,input);

  //if(input.length() == 1) {
//	myChar = input[0];
	//if(myChar == '0'){
	//	cout << "Role: Pong Back, awaiting transmission " << endl << endl;
	//}else{  cout << "Role: Ping Out, starting transmission " << endl << endl;
	//	role = role_ping_out;
//	}
 // }
/***********************************/
  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.

  //  if ( !radioNumber )    {
    //  radio.openWritingPipe(pipes[0]);
    //  radio.openReadingPipe(1,pipes[1]);
  //  } else {
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);
  //  }
	
	//radio.startListening();
	
	// forever loop
	while (1)
	{
	//	if (role == role_ping_out)
	//	{
			// First, stop listening so we can talk.
		//	radio.stopListening();

			// Take the time, and send it.  This will block until complete

			printf("Now sending...\n");
			unsigned long time = millis();

			bool ok = radio.write( &time, sizeof(unsigned long) );

			if (!ok){
				printf("failed.\n");
			}
			else if (ok){
				printf("sent!\n");
			}


	} // forever loop

  return 0;
}

