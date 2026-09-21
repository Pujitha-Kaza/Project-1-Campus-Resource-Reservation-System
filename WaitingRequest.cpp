#include "../include/WaitingRequest.h"
#include <iostream>

WaitingRequest::WaitingRequest()
    : studentId(""),
      studentName(""),
      resourceId(""),
      requestDate("") {
}

WaitingRequest::WaitingRequest(const std::string& studentId,
                               const std::string& studentName,
                               const std::string& resourceId,
                               const std::string& requestDate)
    : studentId(studentId),
      studentName(studentName),
      resourceId(resourceId),
      requestDate(requestDate) {
}

std::string WaitingRequest::getStudentId() const {
    return studentId;
}

std::string WaitingRequest::getStudentName() const {
    return studentName;
}

std::string WaitingRequest::getResourceId() const {
    return resourceId;
}

std::string WaitingRequest::getRequestDate() const {
    return requestDate;
}

void WaitingRequest::display() const {
    std::cout << "Student ID: " << studentId
              << " | Name: " << studentName
              << " | Resource ID: " << resourceId
              << " | Request Date: " << requestDate
              << std::endl;
}
