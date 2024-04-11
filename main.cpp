#include <config.h>
#include <pathtest.h>
#include <iostream>
#include <fstream>
#include <filesystem>
using namespace std;

namespace fs = std::filesystem;

//include C header file
extern "C" {
#include "header.h"
}

#include <jsoncpp/json/json.h>

// In this Part the Programm is checking if hideshell is true or not
bool hideshellcheck(const Json::Value& root) {
    return root.get("hideshell", false).asBool();
}

int main(const int argc, const char **argv) {
    //commandlinearguments
  //  processOptions(argc, argv); @lion da gibt es einen Fehler beim Compilen 

    //Simples Beispiel zum auslesen einer json Datei
    if(argc != 2) {
        cerr << "Bitte eine Datei angeben!" << endl;
        return EXIT_FAILURE;
    }
    else {
        auto path = fs::weakly_canonical(argv[1]);

        if(!fs::exists(path)) {
            cerr << "Datei existiert nicht: " << path.string() << endl;
            return EXIT_FAILURE;
        }

        ifstream ifs(path.string());

        Json::Reader reader;
        Json::Value root;
        if(!reader.parse(ifs, root)) {
            cerr << "Datei ist ungueltig: " << path.string() << endl;
            return EXIT_FAILURE;
        }
        string dateiname = root["outputfile"].asString();
        ofstream batchFile(dateiname);
        batchFile << "irgendwas";

        // Überprüfen, ob die Shell ausgeblendet werden soll
        if (hideshellcheck(root)) {
            cout << "Die Shell soll ausgeblendet werden." << endl;
        } else {
            cout << "Die Shell soll nicht ausgeblendet werden." << endl;
        }

        //Zuerst wird das Land ausgeben
        cout << "Land: " << root["country"].asString() << endl;

        {
            //Werte aus dem Array
            const Json::Value persons = root["persons"];

            if(persons.isArray())
                for (Json::Value::ArrayIndex i = 0; i < persons.size(); ++i) {
                    auto person = persons[i];

                    cout << "Id: " << person["id"].asInt() << endl;
                    //.....
                }
        }
        
        return EXIT_SUCCESS;
    }
}