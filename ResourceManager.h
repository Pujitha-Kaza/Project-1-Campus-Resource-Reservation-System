#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Resource.h"

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

class ResourceManager {
 private:
  std::vector<Resource> resources;

  int findIndex(const std::string& resourceID) const;
  static AvailabilityStatus statusFromText(const std::string& statusText);

 public:
  bool loadFromFile(const std::string& fileName, std::string& errorMessage);

  const Resource* findResource(const std::string& resourceID) const;
  bool contains(const std::string& resourceID) const;
  bool isAvailable(const std::string& resourceID) const;
  bool setStatus(const std::string& resourceID, AvailabilityStatus status);

  void displayAll(std::ostream& output) const;
  void displayAvailability(std::ostream& output) const;
  std::size_t size() const;
};

#endif
