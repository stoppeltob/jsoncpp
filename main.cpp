#include <options.h>
#include <EnvironmentManager.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <cstdlib>
#include <jsoncpp/json/json.h>
#include <getopt.h>
#include <config.h>


using namespace std;
namespace fs = filesystem;

/**
 * Function to process command-line options.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * main function for the programm
*/

int main(const int argc, char **argv) {
    if (argc < 2) {
        cerr << "Please give at least one file!" << endl;
        return EXIT_FAILURE;
    }

    processOptions(argc, argv);
    
    /**
     * This block iterates over command-line arguments, reads JSON files, and processes their contents using an EnvironmentManager instance.
     * made by Lion Wicki & Tobias Stoppelkamp 
    */

    for (int i = 1; i < argc; ++i) { // Start at 1 to skip the program path
        auto path = fs::weakly_canonical(argv[i]);

        if (!fs::exists(path)) {
            cerr << "File does not exist: " << path.string() << endl;
            continue; 
        }

        ifstream ifs(path.string());

        Json::Reader reader;
        Json::Value root;
        if (!reader.parse(ifs, root)) {
            cerr << "File is invalid: " << path.string() << endl;
            continue; 
        }

        // Create an instance of the EnvironmentManager
        EnvironmentManager envManager;

        // Read and process entries from the JSON file
        envManager.readEntriesFromFile(path.string());
    }

    return EXIT_SUCCESS;
}
