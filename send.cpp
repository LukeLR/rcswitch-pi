/*
 Usage: ./send <systemCode> <unitCode> <command>
 Command is 0 for OFF and 1 for ON
 */

#include "RCSwitch.h"
#include "send.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //getopt-method for command line flags parsing

int main(int argc, char *argv[]) {
    
    /*
     output PIN is hardcoded for testing purposes
     see https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     for pin mapping of the raspberry pi GPIO connector
     */
    int PIN = 0;

    /**
     * Wheter to use binary mode or not.
     * See bottom of file for an explaination of binary mode.
     */
    short binaryMode = 0;

    char c; //Temporary character for getopt();

    /**
     * Read command line flags.
     * Possible flags:
     *  -b: enable binary mode
     *  -?: show help
     */
    while ((c = getopt(argc, argv, "bh")) == -1){
      switch(c){
      case 'b': puts("Enabling binary mode..."); binaryMode = 1; break;
      case 'h': print_help(); return 0;
      }
    }

    argc -= optind; //Reduce argc by <optind> parsed options
    argv += optind; //Shift array of options by <optind> parsed options

    if (argc < 3){
        fprintf(stderr,"Please provide three mandatory arguments:\n");
        fprintf(stderr,"SystemCode: The System Code set for all sockets\n");
        fprintf(stderr,"UnitCode: The Unit Code for the individual socket\n");
        fprintf(stderr,"Command: The Command to send, where 1 means ON and 0 means OFF\n");
        return 1;
    }
    char* systemCode = argv[0];
    int unitCode = atoi(argv[1]);
    int command  = atoi(argv[2]);
    
    if (wiringPiSetup () == -1) return 1;
	printf("sending systemCode[%s] unitCode[%i] command[%i]\n", systemCode, unitCode, command);
	RCSwitch mySwitch = RCSwitch();
	mySwitch.enableTransmit(PIN);
    
    switch(command) {
        case 1:
            mySwitch.switchOn(systemCode, unitCode);
            break;
        case 0:
            mySwitch.switchOff(systemCode, unitCode);
            break;
        default:
            printf("command[%i] is unsupported\n", command);
            return -1;
    }
	return 0;
}

void print_help(){
  puts("This is send, a small binary to send 433 MHz signals on a Raspberry Pi");
  puts("in order to control remote controlled power sockets.");
  puts("");
  puts("The following options are available:");
  puts(" -b: adress DIP-Sockets in binary mode instead of traditional mode.");
  puts("     this offers full usage of the 5-bit adress space the unit code");
  puts("     provides, and makes 32 units per system possible (instead of the");
  puts("     traditional 5.");
  puts(" -?: print this help.");
  puts("");
  puts("Please provide three mandatory arguments:");
  puts("SystemCode: The System Code set for all sockets");
  puts("UnitCode: The Unit Code for the individual socket");
  puts("Command: The Command to send, where 1 means ON and 0 means OFF");
}
