#ifndef WAITING_LIST_H
#define WAITING_LIST_H

#include <map>
#include <queue>
#include <string>

#include "WaitingRequest.h"

class WaitingList {
private:
    std::map<std::string, std::queue<WaitingRequest>> waitingQueues;

public:
    void addToWaitingList(const WaitingRequest& request);

    bool hasWaitingStudents(const std::string& resourceId) const;

    WaitingRequest getNextWaitingRequest(
        const std::string& resourceId) const;

    void removeNextWaitingRequest(const std::string& resourceId);

    int getWaitingCount(const std::string& resourceId) const;

    bool isStudentAlreadyWaiting(
        const std::string& studentId,
        const std::string& resourceId) const;

    void displayWaitingListForResource(
        const std::string& resourceId) const;

    void displayAllWaitingLists() const;

    bool isEmpty() const;
};

#endif
