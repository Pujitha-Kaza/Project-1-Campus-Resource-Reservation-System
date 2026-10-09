#ifndef RESERVATION_MANAGER_H
#define RESERVATION_MANAGER_H

#include "Reservation.h"

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

enum class ReservationStatus {
  SUCCESS,
  MISSING_REQUIRED_FIELD,
  INVALID_DATE,
  DUPLICATE_RESERVATION_ID,
  RESOURCE_NOT_FOUND,
  RESOURCE_UNAVAILABLE,
  RESERVATION_NOT_FOUND
};

class ReservationManager {
 private:
  struct Node {
    Reservation reservation;
    Node* next;

    explicit Node(const Reservation& reservation)
        : reservation(reservation), next(nullptr) {}
  };

  Node* head;
  Node* tail;
  std::size_t reservationCount;

  static bool isBlank(const std::string& value);
  static bool isValidDate(const std::string& date);
  void clear();

 public:
  ReservationManager();
  ~ReservationManager();

  ReservationManager(const ReservationManager&) = delete;
  ReservationManager& operator=(const ReservationManager&) = delete;

  ReservationStatus validateReservation(const Reservation& reservation,
                                         bool resourceExists,
                                         bool resourceAvailable) const;

  ReservationStatus createReservation(const Reservation& reservation,
                                       bool resourceExists,
                                       bool resourceAvailable);

  ReservationStatus cancelReservation(const std::string& reservationID,
                                       Reservation& cancelledReservation);

  bool findReservationByID(const std::string& reservationID,
                            Reservation& foundReservation) const;
  std::vector<Reservation> findReservationsByStudentID(
      const std::string& studentID) const;
  std::vector<Reservation> getActiveReservations() const;

  void displayActiveReservations(std::ostream& output) const;
  void displayReservationsSortedByDate(std::ostream& output) const;
  void displayActiveReservationReport(std::ostream& output) const;
  bool isEmpty() const;
  std::size_t size() const;
};

#endif
