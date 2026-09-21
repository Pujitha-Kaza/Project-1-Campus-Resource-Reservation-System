#include "../include/WaitingList.h"
#include <iostream>

void WaitingList::addToWaitingList(const WaitingRequest& request) {
    waitingQueues[request.getResourceId()].push(request);
}

bool WaitingList::hasWaitingStudents(
    const std::string& resourceId) const {

    std::map<std::string,
             std::queue<WaitingRequest>>::const_iterator it;

    it = waitingQueues.find(resourceId);

    if (it == waitingQueues.end()) {
        return false;
    }

    return !it->second.empty();
}

WaitingRequest WaitingList::getNextWaitingRequest(
    const std::string& resourceId) const {

    return waitingQueues.at(resourceId).front();
}

void WaitingList::removeNextWaitingRequest(
    const std::string& resourceId) {

    std::map<std::string,
             std::queue<WaitingRequest>>::iterator it;

    it = waitingQueues.find(resourceId);

    if (it == waitingQueues.end() || it->second.empty()) {
        std::cout << "No students are waiting for resource "
                  << resourceId << "." << std::endl;
        return;
    }

    it->second.pop();

    if (it->second.empty()) {
        waitingQueues.erase(it);
    }
}

int WaitingList::getWaitingCount(
    const std::string& resourceId) const {

    std::map<std::string,
             std::queue<WaitingRequest>>::const_iterator it;

    it = waitingQueues.find(resourceId);

    if (it == waitingQueues.end()) {
        return 0;
    }

    return static_cast<int>(it->second.size());
}

bool WaitingList::isStudentAlreadyWaiting(
    const std::string& studentId,
    const std::string& resourceId) const {

    std::map<std::string,
             std::queue<WaitingRequest>>::const_iterator it;

    it = waitingQueues.find(resourceId);

    if (it == waitingQueues.end()) {
        return false;
    }

    std::queue<WaitingRequest> temporaryQueue = it->second;

    while (!temporaryQueue.empty()) {
        if (temporaryQueue.front().getStudentId() == studentId) {
            return true;
        }

        temporaryQueue.pop();
    }

    return false;
}

void WaitingList::displayWaitingListForResource(
    const std::string& resourceId) const {

    std::map<std::string,
             std::queue<WaitingRequest>>::const_iterator it;

    it = waitingQueues.find(resourceId);

    if (it == waitingQueues.end() || it->second.empty()) {
        std::cout << "No students are currently waiting for resource "
                  << resourceId << "." << std::endl;
        return;
    }

    std::queue<WaitingRequest> temporaryQueue = it->second;
    int position = 1;

    std::cout << "\nWaiting List for Resource: "
              << resourceId << std::endl;

    std::cout << "----------------------------------------"
              << std::endl;

    while (!temporaryQueue.empty()) {
        std::cout << position << ". ";
        temporaryQueue.front().display();

        temporaryQueue.pop();
        position++;
    }
}

void WaitingList::displayAllWaitingLists() const {
    bool foundWaitingStudents = false;

    std::cout << "\n===== Current Waiting Lists ====="
              << std::endl;

    for (std::map<std::string,
                  std::queue<WaitingRequest>>::const_iterator it =
             waitingQueues.begin();
         it != waitingQueues.end();
         ++it) {

        const std::string& resourceId = it->first;
        std::queue<WaitingRequest> temporaryQueue = it->second;

        if (temporaryQueue.empty()) {
            continue;
        }

        foundWaitingStudents = true;

        std::cout << "\nResource ID: "
                  << resourceId << std::endl;

        int position = 1;

        while (!temporaryQueue.empty()) {
            std::cout << position << ". ";
            temporaryQueue.front().display();

            temporaryQueue.pop();
            position++;
        }
    }

    if (!foundWaitingStudents) {
        std::cout << "There are currently no waiting-list requests."
                  << std::endl;
    }
}

bool WaitingList::isEmpty() const {
    for (std::map<std::string,
                  std::queue<WaitingRequest>>::const_iterator it =
             waitingQueues.begin();
         it != waitingQueues.end();
         ++it) {

        if (!it->second.empty()) {
            return false;
        }
    }

    return true;
}
