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

        {
            const Json::Value entries = root["entries"];

            //line 51- 83 Part from David Prinz 
            //Requirement Task 8
            string dateiname = root["outputfile"].asString();
            ofstream batchFile(dateiname);
            batchFile << "@ECHO OFF\n";

            //Requirement Task 9
            // Is trying, if hideshell is true or false
            if (hideshellcheck(root)) {
                batchFile << " /c";
            } else {
                batchFile << " /k";
            }

            if(entries.isArray()) {
                //Requirement Task 10
                for (const auto& entry : entries) {
                    //Requirement Task 11
                    if(entry["type"].asString()=="ENV") {
                        string key = entry["key"].asString();
                        string value = entry["value"].asString();
                    }
                    batchFile << entry["key"].asString()  << entry["value"].asString();
                    //Requirement Task 12
                    if(entry["type"].asString()=="EXE") {
                        string command = entry["command"].asString();
                    }
                    batchFile << entry["command"].asString();
                    //Requirement Task 13
                    if(entry["type"].asString()=="path") {
                        string path = entry["path"].asString();
                    }
                    batchFile << entry["path"].asString();


                }
            }
            return EXIT_SUCCESS;
        }
    }
}