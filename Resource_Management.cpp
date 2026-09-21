#include "ResourceList.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>

std::string toLowerCase(std::string text) {
  for (int i = 0; i < text.length(); i++) {
    text[i] = toLower(text[i]);
  }
  return text;
}

ResourceList::ResourceList() {
  head = nullptr;
  count = 0;
}

ResourceList::~ResourceLIst() {
  clear();
}

bool ResourceList::isEmpty() const {
  return head == nullptr;
}

int ResourceList::getCount() const {
  return count;
}

void ResourceList::addResource(const Resource& resource) {
  ResourceNode* newNode = new ResourceNode(resource);

  if (head == nullptr) {
    head = newNode;
  }

  else {
    ResourceNode* current = head;

    while (current->next != nullptr) {
      current = current->next;
    }

    current->next = newNode;
  }

  count++;
}

bool ResourceList::removeResourceByID(const std::string& resourceID) {
  if (head == nullptr) {
    return false;
  }

  if (head->data.getResourceID() == resourceID) {
    ResourceNode* nodeToDelete = head;
    head = head->next;
    delete nodeToDelete;
    count--;
    return true;
  }

  ResourceNode* current = head;

  while (current->next != nullptr && current->next->data.getResourceID() != resourceID) {
    current = current->next;
  }

  if (current->next == nullptr) {
    return false;
  }

  ResourceNode* nodeToDelete = current->next;
  current->next = nodeToDelete->next;

  delete nodeToDelete;
  count--;

  return true;
}

Resource* ResourceList::findByID(const std::string& resourceID) {
  ResourceNode* current = head;
  
  while (current != nullptr) {
    if (current->data.getResourceID() == resourceID) {
      return &(current->data);
    }
    current = current->next;
  }
  return nullptr;
}

void ResourceList::searchByName(const std::string& keyword) const {
  std::string searchText = toLowerCase(keyword);
  ResourceNode* current = head;
  bool found = false;

  while (current != nullptr) {
    std::string resourceName = toLowerCase(current->data.getResourceName());

    if (resourceName.find(searchText) != std::string::npos) {
      current->data.display();
      found = true;
    }

    current = current->next;
  }

  if (!found) {
    std::cout << "No resources found with that name.\n";
  }
}

void ResourceList::searchByType(const std::string& type) const {
  std::string targetType = toLowerCase(type);
  ResourceNode* current = head;
  bool found = false;

  while (current != nullptr) {
    std::string resourceType = toLowerCase(current->getResourceType());

    if (resourceType == targetType) {
      current->data.display();
      found = true;
    }

    current = current->next;
  }

  if (!found) {
    std::cout << "No resources found for that type.\n";
  }
}

void ResourceList::displayAll() const {
  if (isEmpty()) {
    std::cout << "No resources are loaded.\n";
    return;
  }

  ResourceNode* current = head;

  while(current != nullptr) {
    current->data.display();
    current = current->next;
  }
}

void ResourceList::displayAvailable() const {
  ResourceNode* current = head;
  bool found = false;

  while (current != nullptr) {
    if (current->data.isAvailable()) {
      current->data.display();
      found = true;
    }
    current = current->next;
  }

  if (!found) {
    std::cout << "No resources are currently available.\n";
  }
}

void ResourceList::swapResources(Resource& first, Resource& second) {
  Resource temporary = first;
  first = second;
  second = temporary;
}

void ResourceList::sortByID() {
  if (head == nullptr || head->next == nullptr) {
    return;
  }

  bool swapped;

  do {
    swapped = false;
    ResourceNode* current = head;

    while (current->next != nullptr) {
      std::string currentID = current->data.getResourceID();
      std::string nextID = current->next->data.getResourceID();

      if (currentID > nextID) {
        swapResources(current->data, current->next->data);
        swapped = true;
      }
      current = current->next;
    }
  } while (swapped);
}

void ResourceList::sortByName() {
  if (head == nullptr || head->next == nullptr) {
    return;
  }

  bool swapped;

  do {
    swapped = false;
    ResourceNode* current = head;

    while (current->next != nullptr) {
      std::string currentID = current->data.getResourceName();
      std::string nextID = current->next->data.getResourceName();

      if (currentID > nextID) {
        swapResources(current->data, current->next->data);
        swapped = true;
      }
      current = current->next;
    }
  } while (swapped);
}

void ResourceList::sortbyType() {
  if (head == nullptr || head->next == nullptr) {
    return;
  }

  bool swapped;

  do {
    swapped = false;
    ResourceNode* current = head;

    while (current->next != nullptr) {
      std::string currentID = current->data.getResourceType();
      std::string nextID = current->next->data.getResourceType();

      if (currentID > nextID) {
        swapResources(current->data, current->next->data);
        swapped = true;
      }
      current = current->next;
    }
  } while (swapped);
}
