#ifndef RESOURCE_LIST_H
#define RESOURCE_LIST_H
#include "Resource.h"
#include <string>

struct ResourceNode {
  Resource data;
  ResourceNode* next;

struct ResourceNode {
  Resource data;
  ResourceNode* next;

  ResourceNode (const Resource& resource) {
    data = resources;
    next = nullpr;
  }
};

class ResourceList {
  private:
    ResourceNode* head;
    int count;

    void swapResources(Resource& first, Resource& second);

  public:
    ResourceList();
    ~ResourceList();

    bool isEmpty() const;
    int getCount() const;
    void addResource(const Resource& resource);
    bool removeResourceByID(const std::string& resourceID);

    Resource* findByID(const std::string& resourceID);
    void searchByName(const std::string& keyword);
    void searchByType(const std:string type) const;

    void displayAll() const;
    void displayAvailable() const;

    void sortByID();
    void sortByName();
    void sortByType();
    void sortByAvailability();

    void clear();
};

#endif
    
