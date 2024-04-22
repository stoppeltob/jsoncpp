#include "options.h"
#include <stdio.h>
#include <getopt.h>
#include <cstdlib>

/**
 * Help text + command line options made by Lion Wicki
*/

// Help text for the program
const char * const Helptxt = {
    "Create a Windows Batch file based on the provided JSON configuration.\r\n" \
    "Usage: ./JsoninBatch [OPTIONS] JSON_FILE_PATHS\r\n" \
    "Options: \r\n" \
    "	-h, --help         Help information\r\n" \
    "	-v, --version             Show version information\r\n" \
    "Examples:\r\n" \
    "  ./JsoninBatch2023 ../VSCodeMinGW.json ../MinGW.json      # Convert the 'config.json' file to a Batch file\r\n" \
    "  Converting more than on file is possible\r\n" \
    "\r\n" \
    "Author team: \r\n" 
	"David Prinz david.prinz1123@gmail.com\r\n"
	"Tobias Stoppelkamp stoppelkamp05@gmail.com\r\n"
	"Lion Wicki lionwicki@gmail.com\r\n"
	"Philipp Assfalg philippassfalg2005@gmail.com\r\n"
};

// Version information for the program
const char * Version ="Version 1.0\r\n";  

// Function to print help text
void printHelptxt() {
    printf("%s", Helptxt);
}

// Function to print version information
void printVersion() {
    printf("%s", Version);	
}

// Function to process command-line options
void processOptions(int argc, char **argv) {
    int option_index = 0;

    //Defines longopt command-line arguments 
    static const struct option longopts[] = 
    {
        // Options with no arguments
        { "help", no_argument, NULL, 'h' }, 
        { "version", no_argument, NULL, 'v' }, 
        {0,0,0,0} // Termination
    };

    // Loop through command-line options
    while ((option_index = getopt_long(argc, argv, "hv", longopts, NULL)) != -1) {
        switch (option_index) {
            case 'h':
                printHelptxt();
                exit(0);
            case 'v':
                printVersion();
                exit(0);
            case '?': //Error if invalid command
                fprintf(stderr, "Invalid command-line arguments. Use --help or -h for usage information.\r\n");
                exit(1);
            default:
            // No action needed for other cases
                break;
        }
    }
}
