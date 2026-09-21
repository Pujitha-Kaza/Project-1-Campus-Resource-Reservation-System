#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <string>
#include "Resource_Management.h"

AvailabilityStatus convertStatus (const std::string& statusText);

bool loadResourcesFromFile (const std::string& fileName, ResourceList& resources);

#endif
