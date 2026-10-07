#include "Resource.h"
#include <ostream>

Resource::Resource() {
  resourceID = "";
  resourceName = "";
  resourceType = "";
  availabilityStatus = AvailabilityStatus::AVAILABLE;
}

Resource::Resource(const std::string& id, const std::string& name,
                   const std::string& type, AvailabilityStatus status) {
  resourceID = id;
  resourceName = name;
  resourceType = type;
  availabilityStatus = status;
}

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

void Resource::setAvailabilityStatus(AvailabilityStatus status) {
  availabilityStatus = status;
}

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

void Resource::display(std::ostream& output) const {
  output << "ID: " << resourceID << " | Name: " << resourceName
         << " | Type: " << resourceType
         << " | Status: " << getStatusText() << '\n';
}
