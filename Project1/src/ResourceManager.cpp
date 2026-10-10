#include "ResourceManager.h"
#include "Reservation.h"
#include <fstream>
#include <iostream>

//loading resources
void ResourceManager::loadResources(const std::string& filename) {
  std::ifstream inFile(filename);

  if (!inFile) {
    std::cerr << "Error: Could not open resource file: " << filename << std::endl;
    return;
  }

  Resource r;
  while (inFile >> r) {
    resources.push_back(r);
  }
}

//accessor
const std::vector<Resource>& ResourceManager::getResources() const {
  return resources;
}

//displaying resources
void ResourceManager::displayResources() const {
  if (resources.empty()) {
    std::cout << "No resources loaded.\n";
    return;
  }
  for (const auto& r : resources) {
    r.display();
  }
}

//manual linear search
const Resource* ResourceManager::findResourceByID(const std::string& ID) const {
  for (const auto& r : resources) {
    if (r.getResourceID() == ID) {
      return &r;
    }
  }
  return nullptr;
}

//merge sort
void ResourceManager::mergeSortByName(int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    mergeSortByName(left, mid);
    mergeSortByName(mid + 1, right);
    mergeByName(left, mid, right);
  }
}

void ResourceManager::mergeByName(int left, int mid, int right) {
  std::vector<Resource> temp;
  int i = left;
  int j = mid + 1;

  while (i <= mid && j <= right) {
    if (resources[i].getResourceName() <= resources[j].getResourceName()) {
      temp.push_back(resources[i++]);
    }
    else {
      temp.push_back(resources[j++]);
    }
  }
  while (i <= mid) {
    temp.push_back(resources[i++]);
  }
  while (j <= right) {
    temp.push_back(resources[j++]);
  }
  for (int k = 0; k < temp.size(); ++k) {
    resources[left + k] = temp[k];
  }
}

//report
std::vector<ResourceManager::ResourceUtilization>
ResourceManager::buildUtilizationReport(const std::vector<Reservation>& activeReservations) const {
  std::vector<ResourceUtilization> report;
  report.reserve(resources.size());

  for (const auto& r : resources) {
    int count = 0;

    for (const auto& resv : activeReservations) {
      if (resv.getResourceID() == r.getResourceID()) {
        count++;
      }
    }
    report.push_back({r.getResourceID(), r.getResourceName(), count});
  }
  return report;
}

void ResourceManager::displayUtilizationReport(const std::vector<Reservation>& activeReservations) const {
  auto report = buildUtilizationReport(activeReservations);

  std::cout << "\nResource Utilization Report\n";
  std::cout << "ID\tName\tActive Reservations\n";

  for (const auto& entry : report) {
    std::cout << entry.resourceID << "\t" << entry.resourceName << "\t" << entry.count << "\n";
  }
}
