#ifndef RESERVATION_H
#define RESERVATION_H

#include <iosfwd>
#include <string>

class Reservation {
 private:
  std::string reservationID;
  std::string studentID;
  std::string studentName;
  std::string resourceID;
  std::string reservationDate;

 public:
  Reservation();
  Reservation(const std::string& reservationID,
              const std::string& studentID,
              const std::string& studentName,
              const std::string& resourceID,
              const std::string& reservationDate);

  const std::string& getReservationID() const;
  const std::string& getStudentID() const;
  const std::string& getStudentName() const;
  const std::string& getResourceID() const;
  const std::string& getReservationDate() const;

  void display(std::ostream& output) const;
};

#endif
