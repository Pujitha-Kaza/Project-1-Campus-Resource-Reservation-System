#ifndef RESOURCE_H
#define RESOURCE_H
#include <string>

enum class AvailabilityStatus {
  AVAILABLE,
  RESERVED,
  UNAVAILABLE
};

class Resource {
  private:
    std::string resourceID;
    std::string resourceName;
    std::string resourceType;
    AvailabilityStatus availabilityStatus;

  public: 
    Resource();
    Resource(const string& id, const string& name, const string& type, AvailabilityStatus status);
  
    std::string getResourceID() const;
    std::string getResourceName() const;
    std::string getResourceType() const;
    AvailabilityStatus getAvailabilityStatus() const;

    void setAvailabilityStatus(AvailabilityStatus status);
    bool isAvailable() const;
    string getStatusText() const;

    void display() const;
  };

#endif
