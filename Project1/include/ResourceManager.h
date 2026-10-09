#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <vector>
#include <string>
#include "Resource.h"
#include "Reservation.h"
#include "ReservationManager.h"

class ResourceManager {
  private:
    std::vector<Resources> resources;

    void mergeSortByName(int left, int right);
    void mergeByName(int left, int mid, int right);

  public:
    //loading
    void loadResources (const std::string& filename);

    //accessors
    const std::vector<Resources>& getResources() cosnt;

    //display
    void displayResources() const;

    //searching
    const Resource* findResourceByID(const std::string& ID) const;

    //sorting
    void sortResourcesByName();

    //report
    struct ResourceUtilization {
      std::string resourceID;
      std::string resourceName;
      int count;
    };

    std::vector<ResourceUtilization> buildUtilizationReport(const std::vector<Reservation>& activeReservations) const;
    void displayUtilizationReport(const std::vector<Reservation>& activeReservations) const;
};
#endif
