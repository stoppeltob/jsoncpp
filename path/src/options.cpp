#include "options.h"
#include <stdio.h>
#include <getopt.h>
#include <cstdlib>

const char * const Helptxt = {
    "Create a Windows Batch file based on the provided JSON configuration.\r\n" \
    "Usage: options [OPTIONS] file ... \r\n" \
    "	-h, --help         Hier die Funktion, die beschrieben werden muessen\r\n" \
    "	-v, --version             Show version information\r\n" \
    "\r\n" \
    "Author team: \r\n" 
	"David Prinz david.prinz1123@gmail.com\r\n"
	"Tobias Stoppelkamp stoppelkamp05@gmail.com\r\n"
	"Lion Wicki lionwicki@gmail.com\r\n"
	"Philipp Assfalg philippassfalg2005@gmail.com\r\n"
};

const char * Version ="Version 1.0\r\n";  

void printHelptxt() {
    printf("%s", Helptxt);
}

void printVersion() {
    printf("%s", Version);	
}

void processOptions(int argc, char **argv) {
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
                fprintf(stderr, "Invalid command-line arguments. Use --help or -h for usage information.\r\n");
                exit(1);
            default:
            // No default defined maybe add something here
                break;
        }
    }
}
