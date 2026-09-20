#include <fstream>
#include <iostream>
#include <sstream>
#include "Resource_Management.h"

using namespace std;

AvailabilityStatus convertStatus (const std::string& statusText) {
  if (statusText == "Available") {
    return AvailabilityStatus::AVAILABLE;
  }

  if (statusText == "Reserved") {
    return AvailabilityStatus::RESERVED;
  }
  if (statusText == "Unavailable") {
    return AvailabilityStatus::UNAVAILABLE;
  }
}

bool loadResourcesFromFile (const std::string& fileName, ResourceList& resources) {
  ifstream inputFile(fileName);

  if (!inputFile.is_open() ) {
    cout << "Could not open file: " << fileName << '\n';
    return false;
  }

  string line;
  while (getline(inputFile, line) ) {
    stringstream lineStream(line);

    string id;
    string name;
    string type;
    string status;

    getline(lineStream, id, '|');
    getline(lineStream, name, '|');
    getline(lineStream, type, '|');
    getline(lineStream, status, '|');

    if (id != "" && name != "" && type != "" && status != "") {
      Resource newResource (id, name, type, convertStatus(status));
      resources.addResource(newResource);
    }
  }
  inputFile.close();
  cout << "Resources loaded successfully.\n";
  return true;
}
