#include "ResourceManager.h"

#include <fstream>
#include <ostream>
#include <sstream>

namespace {

std::string trim(const std::string& value) {
  const std::string whitespace = " \t\r\n";
  const std::size_t first = value.find_first_not_of(whitespace);
  if (first == std::string::npos) {
    return "";
  }

  const std::size_t last = value.find_last_not_of(whitespace);
  return value.substr(first, last - first + 1);
}

void mergeResources(std::vector<Resource>& items, std::vector<Resource>& scratch, std::size_t first, std::size_t middle, std::size_t last) {
  std::size_t left = first;
  std::size_t right = middle;
  std::size_t output = first;

  while (left < middle && right < last) {
    if (items[left].getResourceName() <= items[right].getResourceName()) {
      scratch[output++] = items[left++];
    }
    else {
      scratch[output++] = items[right++];
    }
  }

  while (left < middle) {
    scratch[output++] = items[left++];
  }
  while (right < last) {
    scratch[output++] = items[right++];
  }

  for (std::size_t index = first; index < last; ++index) {
    items[index] = scratch[index];
  }
}

void mergeSortResources(std::vector<Resource>& items, std::vector<Resource>& scratch, std::size_t first, std::size_t last) {
  if (last - first < 2) {
    return;
  }
  const std::size_t middle = first + (last - first) / 2;
  mergeSortResources(items, scratch, first, middle);
  mergeSortResources(items, scratch, middle, last);
  mergeResources(items, scratch, first, middle, last);
}
    
}  // namespace

int ResourceManager::findIndex(const std::string& resourceID) const {
  for (std::size_t index = 0; index < resources.size(); ++index) {
    if (resources[index].getResourceID() == resourceID) {
      return static_cast<int>(index);
    }
  }

  return -1;
}

AvailabilityStatus ResourceManager::statusFromText(
    const std::string& statusText) {
  const std::string status = trim(statusText);

  if (status == "Available" || status == "AVAILABLE") {
    return AvailabilityStatus::AVAILABLE;
  }

  if (status == "Reserved" || status == "RESERVED") {
    return AvailabilityStatus::RESERVED;
  }

  return AvailabilityStatus::UNAVAILABLE;
}

bool ResourceManager::loadFromFile(const std::string& fileName,
                                   std::string& errorMessage) {
  std::ifstream input(fileName);
  if (!input.is_open()) {
    errorMessage = "Could not open resource file: " + fileName;
    return false;
  }

  std::vector<Resource> loadedResources;
  std::string line;
  std::size_t lineNumber = 0;

  while (std::getline(input, line)) {
    ++lineNumber;
    if (trim(line).empty()) {
      continue;
    }

    std::stringstream lineStream(line);
    std::string id;
    std::string name;
    std::string type;
    std::string status;

    if (!std::getline(lineStream, id, '|') ||
        !std::getline(lineStream, name, '|') ||
        !std::getline(lineStream, type, '|') ||
        !std::getline(lineStream, status)) {
      errorMessage = "Invalid resource record on line " +
                     std::to_string(lineNumber);
      return false;
    }

    id = trim(id);
    name = trim(name);
    type = trim(type);
    status = trim(status);

    if (id.empty() || name.empty() || type.empty() || status.empty()) {
      errorMessage = "Missing resource field on line " +
                     std::to_string(lineNumber);
      return false;
    }

    loadedResources.emplace_back(id, name, type, statusFromText(status));
  }

  resources = loadedResources;
  errorMessage.clear();
  return true;
}

const Resource* ResourceManager::findResource(
    const std::string& resourceID) const {
  const int index = findIndex(resourceID);
  if (index < 0) {
    return nullptr;
  }

  return &resources[static_cast<std::size_t>(index)];
}

bool ResourceManager::contains(const std::string& resourceID) const {
  return findIndex(resourceID) >= 0;
}

bool ResourceManager::isAvailable(const std::string& resourceID) const {
  const Resource* resource = findResource(resourceID);
  return resource != nullptr && resource->isAvailable();
}

bool ResourceManager::setStatus(const std::string& resourceID,
                                AvailabilityStatus status) {
  const int index = findIndex(resourceID);
  if (index < 0) {
    return false;
  }

  resources[static_cast<std::size_t>(index)].setAvailabilityStatus(status);
  return true;
}

void ResourceManager::displayAll(std::ostream& output) const {
  if (resources.empty()) {
    output << "No resources found.\n";
    return;
  }

  for (const Resource& resource : resources) {
    resource.display(output);
  }
}

void ResourceManager::displayAvailability(std::ostream& output) const {
  if (resources.empty()) {
    output << "No resources found.\n";
    return;
  }

  for (const Resource& resource : resources) {
    output << resource.getResourceID() << " - "
           << resource.getResourceName() << ": "
           << resource.getStatusText() << '\n';
  }
}

std::size_t ResourceManager::size() const { return resources.size(); }

const Resource* ResourceManager::searchByID (const std::string& resourceID) const {
  for (const Resource& resource : resources) {
    if (resource.getResourceID() == resourceID) {
      return &resource;
    }
  }
  return nullptr;
}

std::vector<Resource> ResourceManager::sortedByName() const {
  std::vector<Resource> sorted = resources;
  std::vector<Resource> scratch(sorted.size());
  mergeSortResources(sorted, scratch, 0, sorted.size());
  return sorted;
}

void ResourceManager::displayUtilization (std::ostream& output, const std::vector<Reservation>& activeReservations) const {
  if (resources.empty()) {
    output << "No resources found.\n";
    return;
  }
  output << "Resource Utilization Report\n";
  for (const Resource& resource : resources) {
    std::size_t count = 0;
    for (const Reservation& reservation : activeReservations) {
      if (reservation.getResourceID() == resource.getResourceID()) {
        ++count;
      }
    }
    output << resource.getResourceID() << " - " << resource.getResourceName() << ": " << count << " active Reservation(s)\n";
  }
}
