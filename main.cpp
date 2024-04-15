#include <iostream>
#include <fstream>
#include <filesystem>
#include <jsoncpp/json/json.h>
#include <vector>
#include <string>
#include <cstdlib> // for getenv
#include <sstream>
#include <unistd.h> // for environ
#include <options.h> //commandlineArguments
#include <config.h>

using namespace std;
namespace fs = std::filesystem;

// In this Part the Program is checking if hideshell is true or not
bool hideshellcheck(const Json::Value& root) {
    return root.get("hideshell", false).asBool();
}

class EnvironmentEntry {
public:
    string name;
    string value;
    string type;

    EnvironmentEntry(const string& n, const string& v, const string& t)
        : name(n), value(v), type(t) {}
};

class EnvironmentManager {
private:
    vector<EnvironmentEntry> entries;

public:
    void populateEntries() {
        char** env = environ;  // Fixed: Access to global environ
        for (int i = 0; env[i] != nullptr; ++i) {
            string envVar(env[i]);
            size_t equalsPos = envVar.find('=');
            if (equalsPos != string::npos) {
                string name = envVar.substr(0, equalsPos);
                string value = envVar.substr(equalsPos + 1);
                entries.push_back(EnvironmentEntry(name, value, "ENV"));
            }
        }

        string pathEnv = getenv("PATH");
        istringstream pathStream(pathEnv);
        string path;
        while (getline(pathStream, path, fs::path::preferred_separator)) {
            for (const auto& entry : fs::directory_iterator(path)) {
                if (fs::is_regular_file(entry.path()) && 
                (fs::status(entry.path()).permissions() & fs::perms::owner_exec) != fs::perms::none) {
                    string appName = entry.path().filename().string();
                    string appPath = entry.path().string();
                    entries.push_back(EnvironmentEntry("application", appName + " " + appPath, "EXE"));
                }
            }
        }

        entries.push_back(EnvironmentEntry("PATH", pathEnv, "PATH"));
    }

    void printEntries() const {
        for (const auto& entry : entries) {
            cout << "Type: " << entry.type << ", Name: " << entry.name << ", Value: " << entry.value << "\n";
        }
    }

    void readEntriesFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file " << filename << endl;
            return;
        }

        string line;
        string outputfile;
        bool hideshell = false;

        while (getline(file, line)) {
            if (line.find("outputfile") != string::npos) {
                size_t pos = line.find(":");
                if (pos != string::npos)
                    outputfile = line.substr(pos + 1);
            }
            if (line.find("hideshell") != string::npos) {
                size_t pos = line.find(":");
                if (pos != string::npos)
                    hideshell = (line.substr(pos + 1) == "true") ? true : false;
            }
        }

        if (!outputfile.empty()) {
            createBatchFile(outputfile, "cmd.exe", hideshell);
        } else {
            cerr << "Error: 'outputfile' not found in the file." << endl;
        }
    }

    void createBatchFile(const string& filename, const string& consolePath, bool hideShell) const {
        ofstream batchFile(filename + ".bat");
        if (!batchFile.is_open()) {
            cerr << "Error: Unable to create batch file " << filename << ".bat" << endl;
            return;
        }

        batchFile << (hideShell ? "@ECHO OFF\n" : "@ECHO ON\n");
        batchFile << "start \"\" \"" << consolePath << "\"" << (hideShell ? " /B" : "") << "\n";

        if (hideShell)
            batchFile << "exit\n";

        batchFile.close();

        cout << "Batch file " << filename << ".bat" << " created successfully." << endl;
    }
};

int main(const int argc, const char **argv) {
    //commandlineArguments
    processOptions(argc,argv);

    // Simples Beispiel zum Auslesen einer JSON
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
        // Line 144 - 174 Part from David Prinz 
        // Requirement funtional Task 8
        // The file name of the batch file is written in this part of the code. The information about the name is taken from the "outputfile" of the json file. 
        const Json::Value entries = root["entries"];
        string dateiname = root["outputfile"].asString();
        ofstream batchFile(dateiname);
        batchFile << "@ECHO OFF\n";
        // Requirement funtional Task 9
        // In this part of the code, the information as to whether the shell should be hidden is taken from the "hideshell" entry in the json file. If the shell is to be hidden, "/c" is written, otherwise "/k" 
        if (hideshellcheck(root)) {
            batchFile << "C:\\Windows\\System32\\cmd.exe /c \""; //Path System32 hinzugefügt
        } else {
            batchFile << "C:\\Windows\\System32\\cmd.exe /k \""; //Path System32 hinzugefügt
        }
        // Requirement funtional Task 10
        // In this part of the code, a for-loop is used to read and save any number of entries under a JASON array "entries"
        if(entries.isArray()) {
            string batchCommands = "";
            for (const auto& entry : entries) {
                // Check entry type
                string type = entry["type"].asString();
                //Process based on entry type
                // Requirement funtional Task 11
                //In this part of the code, entries of type "ENV" are stored with their "key" and "value".
                if(entry["type"].asString()=="ENV") {
                    string key = entry["key"].asString();
                    string value = entry["value"].asString();
                    batchCommands += " && set " + key + "=" + value ;
                }
                // Requirement funtional Task 12
                //In this part of the code, entries of type "EXE" are stored with their call "command"
                if(entry["type"].asString()=="EXE") {
                    string command = entry["command"].asString();
                    batchCommands += command + "&&";
                }
                // Requirement funtional Task 13
                // In this part of the code, entries of type "PATH" are stored with their file path ("path")
                if(entry["type"].asString()=="path") {
                    string path = entry["path"].asString();
                }
                batchFile << entry["path"].asString();
            }
            batchFile << batchCommands;
            batchFile << "\"\n";
            batchFile <<"@ECHO ON";
        }
        return EXIT_SUCCESS;
    }
}