#include "EnvironmentManager.h"
#include <jsoncpp/json/json.h>

using namespace std;

/**
 * Constructor for EnvironmentEntry class.
 * @param n Name of the environment entry.
 * @param v Value of the environment entry.
 * @param t Type of the environment entry.
 * made by Philipp Assfalg
 */

EnvironmentEntry::EnvironmentEntry(const string& n, const string& v, const string& t)
    : name(n), value(v), type(t) {}

/**
 * Populates environment entries from system environment variables and directories.
 * made by Philipp Assfalg
 */

void EnvironmentManager::populateEntries() {
    // Retrieving environment variables
    char** env = environ;
    for (int i = 0; env[i] != nullptr; ++i) {
        // Parsing environment variable
        string envVar(env[i]);
        size_t equalsPos = envVar.find('=');
        if (equalsPos != string::npos) {
            string name = envVar.substr(0, equalsPos);
            string value = envVar.substr(equalsPos + 1);
            // Adding environment entry
            entries.push_back(EnvironmentEntry(name, value, "ENV"));
        }
    }

    // Parsing PATH environment variable
    string pathEnv = getenv("PATH");
    istringstream pathStream(pathEnv);
    string path;
    while (getline(pathStream, path, fs::path::preferred_separator)) {
        // Iterating through directories in PATH
        for (const auto& entry : fs::directory_iterator(path)) {
            // Checking for executable files
            if (fs::is_regular_file(entry.path()) && (fs::status(entry.path()).permissions() & fs::perms::owner_exec) != fs::perms::none) {
                // Adding executable entry
                string appName = entry.path().filename().string();
                string appPath = entry.path().string();
                entries.push_back(EnvironmentEntry("application", appName + " " + appPath, "EXE"));
            }
        }
    }
    // Adding PATH environment entry
    entries.push_back(EnvironmentEntry("PATH", pathEnv, "PATH"));
}

/**
 * Prints environment entries to standard output.
 * made by Philipp Assfalg
 */

void EnvironmentManager::printEntries() const {
    for (const auto& entry : entries) {
        cout << "Type: " << entry.type << ", Name: " << entry.name << ", Value: " << entry.value << "\r\n";
    }
}

/**
 * Reads environment entries from a JSON file and adds them to the entries list.
 * @param filename Name of the JSON file to read from.
 * Made by Tobias Stoppelkamp
 */

void EnvironmentManager::readEntriesFromFile(const string& filename) {
    // Open the JSON file
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    // Parsing JSON
    Json::Value root;
    Json::CharReaderBuilder builder;
    string errs;
    bool parsingSuccessful = Json::parseFromStream(builder, file, &root, &errs);
    file.close();

    // Check if JSON parsing was successful
    if (!parsingSuccessful) {
        cerr << "Error parsing JSON: " << errs << endl;
        return;
    }

    // Check if the root of JSON is an object
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

    // Check if 'hideshell' option is specified in the JSON
    if (root.isMember("hideshell")) {
        hideshell = root["hideshell"].asBool();
    }

    // Check if 'entries' array is present in the JSON
    if (root.isMember("entries")) {
    const Json::Value entriesArray = root["entries"];
    int line_number = 4; // Variable to store the line number

    // Loop through each entry in the 'entries' array
    for (const auto& entry : entriesArray) {
        line_number++; // Increment line number for each entry

        // Check if each entry is an object
        if (!entry.isObject()) {
            cerr << "Error at line " << line_number << ": Entry is not an object in JSON file." << endl;
            return;
        }

        // Check if each entry has a 'type' field
        if (!entry.isMember("type")) {
            cerr << "Error at line " << line_number << ": Entry does not have a 'type' field in JSON file." << endl;
            return;
        }

        // Process each entry based on its type
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
    // Create the batch file using extracted data from JSON
    createBatchFile(outputfile, hideshell, root);
}

/*
     * Creates a batch file with environment configurations and executable commands.
     * @param filename Name of the batch file to create.
     * @param hideShell Boolean indicating whether to hide the shell window.
     * @param root JSON root object containing additional configuration.
     * Made by David Prinz & Tobias Stoppelkamp
     */


void EnvironmentManager::createBatchFile(const string& filename, bool hideShell, const Json::Value& root) const {
    // Open the batch file for writing
    ofstream batchFile(filename);
    if (!batchFile.is_open()) {
        cerr << "Error: Unable to create batch file " << filename << endl;
        return;
    }

    // Write initial batch file commands
    batchFile << "@ECHO OFF\r\n";

    // Check if "hideshell" option is present in the JSON and write the corresponding command
    bool hideshell = false;
    if (root.isMember("hideshell")) {
        hideshell = root["hideshell"].asBool();
    }

    // Write the appropriate command based on the value of hideShell
    if (hideshell) {
        batchFile << "C:\\Windows\\System32\\cmd.exe /c "; 
    } else {
        batchFile << "C:\\Windows\\System32\\cmd.exe /k "; 
    }

    // Write executable commands to the batch file
    for (const auto& entry : entries) {
        if (entry.type == "EXE") {
            batchFile << "\"" << entry.value;
        }
    }

    // Write environment variable commands to the batch file
    for (const auto& entry : entries) {
        if (entry.type == "ENV") {
            batchFile << " && set " << entry.name << "=" << entry.value;
        }
    }

    // Variable to store PATH commands in the batch file
    string pathCommands;

    // Write PATH commands to the batch file
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

    

    // Write PATH commands to the batch file
    if (!pathCommands.empty()) {
        batchFile << " && set path=" << pathCommands << ";%path%";
    }

    // Additional application command to be written to the batch file
   string applicationPath = root["application"].asString();
    if (!applicationPath.empty()) {
        // Remove the ".bat" extension from the filename
        string filenameWithoutExtension = filename.substr(0, filename.size() - 4);
        batchFile << " && start \"" << filenameWithoutExtension << "\" " << applicationPath;
    }

    // Write closing commands to the batch file
    batchFile << "\"\r\n@ECHO ON\r\n";

    // Close the batch file
    batchFile.close();

    // Print success message
    cout << "Batch file " << filename << " created successfully. You can find the created file under the build folder." << endl;
}
