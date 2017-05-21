#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

#include "mqtt/client.h"

extern "C" {
	#include "XboxInput.h"
}

int main(int argc, char* argv[]) {

	int xboxfd;
	char* path = (char*)"/dev/input/js2";
	int LX[] = {0,0,0,0,0};
	int LY[] = {0,0,0,0,0};
	// drive[0] = speed, drive[1] = angle
	double* drive = (double*)malloc(2 * sizeof(double));
	// buttons[0-14] are xbox buttons, 15 total
    int* buttons = (int*)malloc(15 * sizeof(int));


    printf("Setting up xbox controller...\n");
	xbox_setup(&xboxfd, path);
	printf("Setup completed!\n\n");
	

	const std::string TOPIC { "XboxControl" };

    // Create a client

    mqtt::client cli("localhost", "1");

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    // for converting double to stirng
    std::string str;

    cli.connect(connOpts);

	while(1) {
	    try {
	        // Connect to the client

	        

	        // read input
	        readInput(&xboxfd, LX, LY, drive, buttons);
	        str = "Speed: " + std::to_string(*drive) + "	Angle: " + std::to_string(*(drive+1));
	        //printf("speed: %f\n", (*drive));
	        //printf("string output: %s\n", str.c_str());

	        const char* PAYLOAD2 = str.c_str();


	        // Publish a message
	        cli.publish(TOPIC, PAYLOAD2, strlen(PAYLOAD2), 0, false);

	        // Disconnect
	        if((*buttons)) {
	        	cli.disconnect();
	        	return 0;
	        }
	        //str = "";
	    }
	    catch (const mqtt::exception& exc) {
	        std::cerr << "Error: " << exc.what() << " ["
	            << exc.get_reason_code() << "]" << std::endl;
	        return 1;
	    }
	}
    return 0;
}