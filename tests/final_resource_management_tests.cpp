#include "ResourceManager.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

int testsRun = 0;
int testsFailed = 0;

void check(bool condition, const std::string& testName) {
  ++testsRun;
  if (!condition) {
    ++testsFailed;
    std::cerr << "FAILED: " << testName << '\n';
  }
}

} 

int main() {
  {
    ResourceManager resources;
    std::string error;

    check(resources.loadFromFile("Resources.txt", error),
          "resource file loads successfully");
    check(resources.size() == 20,
          "all resource records are loaded");

    const Resource* found = resources.searchByID("R101");
    check(found != nullptr,
          "search finds an existing resource ID");
    check(found != nullptr &&
              found->getResourceName() == "Study Room 101" &&
              found->getResourceType() == "Study Room",
          "loaded resource fields are parsed correctly");
    check(resources.searchByID("R999") == nullptr,
          "search handles a missing resource ID");

    check(resources.isAvailable("R101"),
          "available resource is reported as available");
    check(!resources.isAvailable("R103"),
          "unavailable resource is reported as unavailable");
    check(resources.setStatus("R103", AvailabilityStatus::AVAILABLE) &&
              resources.isAvailable("R103"),
          "resource availability can be updated");

    const std::vector<Resource> sorted = resources.sortedByName();
    bool namesAreSorted = true;
    for (std::size_t index = 1; index < sorted.size(); ++index) {
      if (sorted[index - 1].getResourceName() >
          sorted[index].getResourceName()) {
        namesAreSorted = false;
      }
    }

    check(sorted.size() == resources.size(),
          "sorting keeps every resource");
    check(namesAreSorted,
          "merge sort orders resources alphabetically by name");
    check(!sorted.empty() &&
              sorted.front().getResourceID() == "R111" &&
              sorted.front().getResourceType() == "Lab Equipment" &&
              sorted.front().isAvailable(),
          "sorting preserves resource ID, type, and availability");

    const std::vector<Reservation> activeReservations = {
        Reservation("A1", "S1", "Student One", "R101", "2026-10-10"),
        Reservation("A2", "S2", "Student Two", "R101", "2026-10-11"),
        Reservation("A3", "S3", "Student Three", "R105", "2026-10-12")};

    std::ostringstream report;
    resources.displayUtilization(report, activeReservations);
    const std::string reportText = report.str();

    check(reportText.find(
              "R101 - Study Room 101: 2 active reservation(s)") !=
              std::string::npos,
          "utilization report counts active reservations");
    check(reportText.find(
              "R102 - Study Room 102: 0 active reservation(s)") !=
              std::string::npos,
          "utilization report includes a resource with zero reservations");
    check(reportText.find(
              "R120 - Physics Tutoring: 0 active reservation(s)") !=
              std::string::npos,
          "utilization report includes the final zero-reservation resource");
  }

  {
    ResourceManager resources;
    std::string error;

    check(!resources.loadFromFile(
              "missing_resource_management_test_file.txt", error),
          "missing resource file is rejected");
    check(!error.empty(),
          "missing resource file returns an error message");
  }

  {
    const std::string fileName =
        "resource_management_invalid_test_file.txt";
    {
      std::ofstream output(fileName);
      output << "R999|Incomplete Resource||Available\n";
    }

    ResourceManager resources;
    std::string error;
    check(!resources.loadFromFile(fileName, error),
          "resource record with a missing field is rejected");
    check(error.find("line 1") != std::string::npos,
          "invalid resource record reports its line number");

    std::remove(fileName.c_str());
  }

  if (testsFailed == 0) {
    std::cout << "All " << testsRun
              << " resource management tests passed.\n";
    return 0;
  }

  std::cerr << testsFailed << " of " << testsRun
            << " resource management tests failed.\n";
  return 1;
}
