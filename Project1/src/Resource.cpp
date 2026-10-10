#include "Resource.h"
#include <iostream>

Resource::Resource() {
  resourceID = "";
  resourceName = "";
  resourceType = "";
  availabilityStatus = AvailabilityStatus::AVAILABLE;
}

Resource::Resource (const std::string& id, const std::string& name, const std::string& type, AvailabilityStatus status) {
  resourceID = id;
  resourceName = name;
  resourceType = type;
  availabilityStatus = status;
}

//getters
std::string Resource::getResourceID() const {
  return resourceID;
}

std::string Resource::getResourceName() const {
  return resourceName;
}

std::string Resource::getResourceType() const {
  return resourceType;
}

AvailabilityStatus Resource::getAvailabilityStatus() const {
  return availabilityStatus;
}

//setters
void Resource::setAvailabilityStatus(AvailabilityStatus status) {
  availabilityStatus = status;
}

//helpers
bool Resource::isAvailable() const {
  return availabilityStatus == AvailabilityStatus::AVAILABLE;
}

std::string Resource::getStatusText() const {
  if (availabilityStatus == AvailabilityStatus::AVAILABLE) {
    return "Available";
  }

  if (availabilityStatus == AvailabilityStatus::RESERVED) {
    return "Reserved";
  }

  if (availabilityStatus == AvailabilityStatus::UNAVAILABLE) {
    return "Unavailable";
  }

  return "Unknown";
}

//display
void Resource::display() const {
  std::cout << "ID: " << resourceID << " | Name: " << resourceName << " | Type: " << resourceType << " | Status: " << getStatusText() << '\n';
}

//file loading operator
std::istream& operator>>(std::istream& in, Resource& r) {
  std::string line;
  if (!std::getline(in, line)) {
    return in;
  }
  if (line.empty()) {
    return in;
  }

  std::stringstream ss(line);
  std::string, id, name, type, statusStr;

  std::getline(ss, id, '|');
  std::getline(ss, name, '|');
  std::getline(ss, type, '|');
  std::getline(ss, statusStr, '|');

  r.resourceID = id;
  r.resourceName = name;
  r.resourceType = type;

  if (statusStr == "Available") {
    r.availabilityStatus = AvailabilityStatus::AVAILABLE:
  }
  else if (statusStr == "Reserved") {
    r.availabilityStatus = AvailabilityStatus::RESERVED;
  }
  else if (statusStr == "Unavailable") {
    r.availabilityStatus = AvailabilityStatus::UNAVAILABLE;
  }
  else {
    r.availabilityStatus = AvailabilityStatus::AVAILABLE;
  }
  return in;
}
