#ifndef WAITING_REQUEST_H
#define WAITING_REQUEST_H

#include <string>

class WaitingRequest {
private:
    std::string studentId;
    std::string studentName;
    std::string resourceId;
    std::string requestDate;

public:
    WaitingRequest();

    WaitingRequest(const std::string& studentId,
                   const std::string& studentName,
                   const std::string& resourceId,
                   const std::string& requestDate);

    std::string getStudentId() const;
    std::string getStudentName() const;
    std::string getResourceId() const;
    std::string getRequestDate() const;

    void display() const;
};

#endif
