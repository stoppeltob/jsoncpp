#include "options.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
//Options.c made by LW

// Helptext for the Programm - Requirement Task 1, 2, 3
const char * const Helptxt = {
    "Create a Windows Batch file based on the provided JSON configuration.\n" \
    "Usage: options [OPTIONS] file ... \n" \
    "	-h, --help         Hier die Funktion, die beschrieben werden muessen\n" \
    "	-o, --output=FILENAME     Set the output file name\n" \
    "	-v, --version             Show version information\n" \
    "\n" \
    "Author team: \n" 
	"David Prinz david.prinz1123@gmail.com\n"
	"Tobias Stoppelkamp tobias.stoppelkamp05@gmail.com\n"
	"Lion Wicki lionwicki@gmail.com\n"
	"Phillip Assfalg philippassfalg2005@gmail.com\n"
};

const char * Version ="Version 1.0\n";  

void printHelptxt() {
printf("%s", Helptxt);
}

void printVersion() {
printf(Version);	
}

//processes the Options given by main.cpp
void processOptions(int argc, char *argv[]) {
	
int option_index = 0;

//Defines longopt command-line arguments 
static const struct option longopts[] = 
{
	// takes no arguments --> hier koennte man das inputfile definieren !!!
	{ "help", no_argument, NULL, 'h' }, 
	{ "version", no_argument, NULL, 'v' }, 
	{0,0,0,0} // Termination
};

while ((option_index = getopt_long(argc, argv, "hv", longopts, NULL)) != -1) {
	switch (option_index) {
		case 'h':
			printHelptxt();
			exit(0);
		case 'v':
			printVersion();
			exit(0);
		case '?': //Error if invalid command
			fprintf(stderr, "Invalid command-line arguments. Use --help or -h for usage information.\n");
			exit(1);
		default:
		// No default defined maybe add something here
			break;
		}
	}
}
