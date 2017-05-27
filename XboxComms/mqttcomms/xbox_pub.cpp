#include <iostream>
#include <cstdlib>
#include <string>

#include "mqtt/client.h"

extern "C" {
	#include "XboxInput.h"
}

char* PATH_TO_CONTROLLER = (char*)"/dev/input/js2";

const std::string ADDRESS("tcp://localhost");
const std::string CLIENTID("xbox_pub");
const std::string TOPIC("Controls");
const std::string TOPIC_DRIVE("Drive");
const std::string TOPIC_BUTTONS("Buttons");

int main(int argc, char* argv[]) {
	int xboxfd;
	int LX[] = {0,0,0,0,0};
	int LY[] = {0,0,0,0,0};
	// drive[0] = speed, drive[1] = angle
	double* drive = (double*)malloc(2 * sizeof(double));
	// buttons[0-14] are xbox buttons, 15 total
    int* buttons = (int*)malloc(15 * sizeof(int));
    double* inputs = (double*)malloc(17*sizeof(double));

    // for converting double to stirng
    std::string str;
    const char* PAYLOAD_DRIVE;
    const char* PAYLOAD_BUTTONS;
    char input = '\0';
    int start = 0;


    printf("Setting up xbox controller...\n");
	xbox_setup(&xboxfd, PATH_TO_CONTROLLER);
	printf("Setup completed!\n\n");

    // Create a client
    mqtt::client cli(ADDRESS, CLIENTID);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);


    cli.connect(connOpts);
    printf("Press 'g' to read input or BACK on the controller to stop reading. When not reading, press q to quit.\n");
	while(1) {
		if(start == 0) {
			input = std::tolower(std::cin.get());
			if(input == 'g') {
				start = 1;
				*(buttons+6) = 0;
			} else if(input == 'q') {
				cli.disconnect();
				return 0;
			} else {
				continue;
			}
		}


	    try {
	        readInput(&xboxfd, LX, LY, drive, buttons);
//#if 0
	        inputs[0] = *(drive);
	        inputs[1] = *(drive+1);
	        for(int i = 2; i < 17; i++) {
	        	inputs[i] = *(buttons+i-2);
	        }
	        cli.publish(TOPIC, (void*)inputs, 17*sizeof(double), 2, false);
//#endif

#if 0
	        // Turn drive to string
	        str = "Speed: " + std::to_string(*drive) + "	Angle: " + std::to_string(*(drive+1));
	        PAYLOAD_DRIVE = str.c_str();
	        // Publish it
	        cli.publish(TOPIC_DRIVE, PAYLOAD_DRIVE, strlen(PAYLOAD_DRIVE), 2, false);

	        // Turn buttons to string
	        /*
	        str = "A: " + std::to_string(*buttons) + "   B: " + std::to_string(*(buttons+1))
	        		+ "   X: " + std::to_string(*(buttons+2)) + "   Y: " + std::to_string(*(buttons+3))
	        		+ "   LB: " + std::to_string(*(buttons+4)) + "   RB: " + std::to_string(*(buttons+5))
	        		+ "   L: " + std::to_string(*(buttons+11)) + "   R: " + std::to_string(*(buttons+12))
	        		+ "   UP: " + std::to_string(*(buttons+13)) + "   DOWN: " + std::to_string(*(buttons+14));
	        		*/

	        // [A], [B], [X], [Y], [LB], [RB], [L], [R], [UP], [DOWN]
	        str = "[" + std::to_string(*buttons) + " " + std::to_string(*(buttons+1))
	        		+ " " + std::to_string(*(buttons+2)) + " " + std::to_string(*(buttons+3))
	        		+ " " + std::to_string(*(buttons+4)) + " " + std::to_string(*(buttons+5))
	        		+ " " + std::to_string(*(buttons+11)) + " " + std::to_string(*(buttons+12))
	        		+ " " + std::to_string(*(buttons+13)) + " " + std::to_string(*(buttons+14)) + "]";

			PAYLOAD_BUTTONS = str.c_str();
			// Publish it
	        cli.publish(TOPIC_BUTTONS, PAYLOAD_BUTTONS, strlen(PAYLOAD_BUTTONS), 2, false);
#endif
	        // Pause if BACK is pressed
	        if(*(buttons+6)) {
	        	start = 0;
	        }
	    }
	    catch (const mqtt::exception& exc) {
	        std::cerr << "Error: " << exc.what() << " ["
	            << exc.get_reason_code() << "]" << std::endl;
	        return 1;
	    }
	}
    return 0;
}