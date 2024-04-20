#include "EnvironmentManager.h"
#include <jsoncpp/json/json.h>

using namespace std;

EnvironmentEntry::EnvironmentEntry(const string& n, const string& v, const string& t)
    : name(n), value(v), type(t) {}

void EnvironmentManager::populateEntries() {
    char** env = environ;
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
            if (fs::is_regular_file(entry.path()) && (fs::status(entry.path()).permissions() & fs::perms::owner_exec) != fs::perms::none) {
                string appName = entry.path().filename().string();
                string appPath = entry.path().string();
                entries.push_back(EnvironmentEntry("application", appName + " " + appPath, "EXE"));
            }
        }
    }

    entries.push_back(EnvironmentEntry("PATH", pathEnv, "PATH"));
}

void EnvironmentManager::printEntries() const {
    for (const auto& entry : entries) {
        cout << "Type: " << entry.type << ", Name: " << entry.name << ", Value: " << entry.value << "\r\n";
    }
}

void EnvironmentManager::readEntriesFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    Json::Value root;
    Json::CharReaderBuilder builder;
    string errs;
    bool parsingSuccessful = Json::parseFromStream(builder, file, &root, &errs);
    file.close();

    if (!parsingSuccessful) {
        cerr << "Error parsing JSON: " << errs << endl;
        return;
    }

    if (!root.isObject()) {
        cerr << "Root is not an object in JSON file." << endl;
        return;
    }

    string outputfile;
    bool hideshell = false;

    // Extracting data from JSON
    if (root.isMember("outputfile")) {
        outputfile = root["outputfile"].asString();
    } else {
        cerr << "Error: 'outputfile' not found in the file." << endl;
        return;
    }

    if (root.isMember("hideshell")) {
        hideshell = root["hideshell"].asBool();
    }

    if (root.isMember("entries")) {
    const Json::Value entriesArray = root["entries"];
    int line_number = 4; // Variable to store the line number

    for (const auto& entry : entriesArray) {
        line_number++; // Increment line number for each entry

        if (!entry.isObject()) {
            cerr << "Error at line " << line_number << ": Entry is not an object in JSON file." << endl;
            return;
        }

        if (!entry.isMember("type")) {
            cerr << "Error at line " << line_number << ": Entry does not have a 'type' field in JSON file." << endl;
            return;
        }

        string type = entry["type"].asString();
        if (type == "ENV" || type == "EXE" || type == "PATH") {
            if (type == "ENV") {
                if (!entry.isMember("key") || !entry.isMember("value")) {
                    cerr << "Error at line " << line_number << ": Entry of type 'ENV' requires 'key' and 'value' fields. In " << filename << endl;
                    return;
                }
                entries.push_back(EnvironmentEntry(entry["key"].asString(), entry["value"].asString(), "ENV"));
            } else if (type == "EXE") {
                if (!entry.isMember("command")) {
                    cerr << "Error at line " << line_number << ": Entry of type 'EXE' requires 'command' field. In " << filename << endl;
                    return;
                }
                entries.push_back(EnvironmentEntry("application", entry["command"].asString(), "EXE"));
            } else if (type == "PATH") {
                if (!entry.isMember("path")) {
                    cerr << "Error at line " << line_number << ": Entry of type 'PATH' requires 'path' field. In " << filename << endl;
                    return;
                }
                entries.push_back(EnvironmentEntry("PATH", entry["path"].asString(), "PATH"));
            }
        } else {
            cerr << "Error at line " << line_number << ": Unknown entry type '" << type << "' in JSON file" << filename << "." << endl;
            return;
        }
    }
}


    createBatchFile(outputfile, hideshell, root);
}


void EnvironmentManager::createBatchFile(const string& filename, bool hideShell, const Json::Value& root) const {
    ofstream batchFile(filename);
    if (!batchFile.is_open()) {
        cerr << "Error: Unable to create batch file " << filename << endl;
        return;
    }

    batchFile << "@ECHO OFF\r\n";

    // Überprüfen, ob "hideshell" im JSON vorhanden ist und den entsprechenden Befehl schreiben
    bool hideshell = false;
    if (root.isMember("hideshell")) {
        hideshell = root["hideshell"].asBool();
    }

    // Schreibe den entsprechenden Befehl basierend auf dem Wert von hideShell
    if (hideshell) {
        batchFile << "C:\\Windows\\System32\\cmd.exe /c "; // Pfad System32 hinzugefügt
    } else {
        batchFile << "C:\\Windows\\System32\\cmd.exe /k "; // Pfad System32 hinzugefügt
    }



    for (const auto& entry : entries) {
        if (entry.type == "EXE") {
            batchFile << "\"" << entry.value;
        }
    }

    for (const auto& entry : entries) {
        if (entry.type == "ENV") {
            batchFile << " && set " << entry.name << "=" << entry.value;
        }
    }

    // Variable zum Speichern der PATH-Befehle in der Batch-Datei
    string pathCommands;

    for (const auto& entry : entries) {
        if (entry.type == "PATH") {
            string path = entry.value;
            if (!path.empty()) {
                if (!pathCommands.empty()) {
                    pathCommands += ";";
                }
                pathCommands += path;
            }
        }
    }

    

    // Schreiben der PATH-Befehle in die Batch-Datei
    if (!pathCommands.empty()) {
        batchFile << " && set PATH=" << pathCommands << ";%PATH%";
    }

   string applicationPath = root["application"].asString();
    if (!applicationPath.empty()) {
        batchFile << " && start \"" << filename << "\" " << applicationPath;
    }

    batchFile << "\"\r\n@ECHO ON\r\n";

    batchFile.close();

    cout << "Batch file " << filename << " created successfully." << endl;
}
