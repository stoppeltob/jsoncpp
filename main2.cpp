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



int main(const int argc, char **argv) {
    if (argc < 2) {
        cerr << "Bitte mindestens eine Datei angeben!" << endl;
        return EXIT_FAILURE;
    }

    // Kommandozeilenargumente verarbeiten
    processOptions(argc, argv);

    for (int i = 1; i < argc; ++i) { // Starten bei 1, um den Programmpfad zu überspringen
        auto path = fs::weakly_canonical(argv[i]);

        if (!fs::exists(path)) {
            cerr << "Datei existiert nicht: " << path.string() << endl;
            continue; // Mit der nächsten Datei fortfahren
        }

        ifstream ifs(path.string());

        Json::Reader reader;
        Json::Value root;
        if (!reader.parse(ifs, root)) {
            cerr << "Datei ist ungueltig: " << path.string() << endl;
            continue; // Mit der nächsten Datei fortfahren
        }

        // Eine Instanz der EnvironmentManager-Klasse erstellen
        EnvironmentManager envManager;

        // Einträge aus der JSON-Datei lesen und verarbeiten
        envManager.readEntriesFromFile(path.string());
    }

    return EXIT_SUCCESS;
}
