#ifndef ENVIRONMENTMANAGER_H
#define ENVIRONMENTMANAGER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <jsoncpp/json/json.h>

using namespace std;
namespace fs = filesystem;

class EnvironmentEntry {
public:
    string name;
    string value;
    string type;

    EnvironmentEntry(const string& n, const string& v, const string& t);
};

class EnvironmentManager {
private:
    vector<EnvironmentEntry> entries;

public:
    void populateEntries();
    void printEntries() const;
    void readEntriesFromFile(const string& filename);
    void createBatchFile(const string& filename, bool hideShell, const Json::Value& root) const;

};

#endif // ENVIRONMENTMANAGER_H
