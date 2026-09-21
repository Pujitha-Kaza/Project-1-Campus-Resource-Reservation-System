#include "Reservation.h"

#include <ostream>

Reservation::Reservation() = default;

Reservation::Reservation(const std::string& reservationID,
                         const std::string& studentID,
                         const std::string& studentName,
                         const std::string& resourceID,
                         const std::string& reservationDate)
    : reservationID(reservationID),
      studentID(studentID),
      studentName(studentName),
      resourceID(resourceID),
      reservationDate(reservationDate) {}

const std::string& Reservation::getReservationID() const {
  return reservationID;
}

const std::string& Reservation::getStudentID() const { return studentID; }

const std::string& Reservation::getStudentName() const { return studentName; }

const std::string& Reservation::getResourceID() const { return resourceID; }

const std::string& Reservation::getReservationDate() const {
  return reservationDate;
}

void Reservation::display(std::ostream& output) const {
  output << "Reservation ID: " << reservationID << '\n'
         << "Student ID: " << studentID << '\n'
         << "Student Name: " << studentName << '\n'
         << "Resource ID: " << resourceID << '\n'
         << "Reservation Date: " << reservationDate << '\n';
}
