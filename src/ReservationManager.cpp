#include "ReservationManager.h"

#include <cctype>
#include <ostream>

namespace {

bool isLeapYear(int year) {
  return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

}  // namespace

ReservationManager::ReservationManager()
    : head(nullptr), tail(nullptr), reservationCount(0) {}

ReservationManager::~ReservationManager() { clear(); }

bool ReservationManager::containsReservationID(
    const std::string& reservationID) const {
  Node* current = head;

  while (current != nullptr) {
    if (current->reservation.getReservationID() == reservationID) {
      return true;
    }
    current = current->next;
  }

  return false;
}

bool ReservationManager::hasResourceReservation(
    const std::string& resourceID,
    const std::string& reservationDate) const {
  Node* current = head;

  while (current != nullptr) {
    const Reservation& reservation = current->reservation;
    if (reservation.getResourceID() == resourceID &&
        reservation.getReservationDate() == reservationDate) {
      return true;
    }
    current = current->next;
  }

  return false;
}

bool ReservationManager::isBlank(const std::string& value) {
  if (value.empty()) {
    return true;
  }

  for (char character : value) {
    if (!std::isspace(static_cast<unsigned char>(character))) {
      return false;
    }
  }

  return true;
}

bool ReservationManager::isValidDate(const std::string& date) {
  if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
    return false;
  }

  for (std::size_t index = 0; index < date.size(); ++index) {
    if (index == 4 || index == 7) {
      continue;
    }
    if (!std::isdigit(static_cast<unsigned char>(date[index]))) {
      return false;
    }
  }

  const int year = std::stoi(date.substr(0, 4));
  const int month = std::stoi(date.substr(5, 2));
  const int day = std::stoi(date.substr(8, 2));

  if (year < 1 || month < 1 || month > 12) {
    return false;
  }

  const int daysInMonth[] = {31, 28, 31, 30, 31, 30,
                             31, 31, 30, 31, 30, 31};
  int maximumDay = daysInMonth[month - 1];

  if (month == 2 && isLeapYear(year)) {
    maximumDay = 29;
  }

  return day >= 1 && day <= maximumDay;
}

ReservationStatus ReservationManager::validateReservation(
    const Reservation& reservation,
    bool resourceExists,
    bool resourceAvailable) const {
  if (isBlank(reservation.getReservationID()) ||
      isBlank(reservation.getStudentID()) ||
      isBlank(reservation.getStudentName()) ||
      isBlank(reservation.getResourceID()) ||
      isBlank(reservation.getReservationDate())) {
    return ReservationStatus::MISSING_REQUIRED_FIELD;
  }

  if (!isValidDate(reservation.getReservationDate())) {
    return ReservationStatus::INVALID_DATE;
  }

  if (containsReservationID(reservation.getReservationID())) {
    return ReservationStatus::DUPLICATE_RESERVATION_ID;
  }

  if (!resourceExists) {
    return ReservationStatus::RESOURCE_NOT_FOUND;
  }

  if (!resourceAvailable ||
      hasResourceReservation(reservation.getResourceID(),
                             reservation.getReservationDate())) {
    return ReservationStatus::RESOURCE_UNAVAILABLE;
  }

  return ReservationStatus::SUCCESS;
}

ReservationStatus ReservationManager::createReservation(
    const Reservation& reservation,
    bool resourceExists,
    bool resourceAvailable) {
  const ReservationStatus validationResult =
      validateReservation(reservation, resourceExists, resourceAvailable);

  if (validationResult != ReservationStatus::SUCCESS) {
    return validationResult;
  }

  // Keeping a tail pointer makes the physical list insertion O(1).
  Node* newNode = new Node(reservation);

  if (head == nullptr) {
    head = newNode;
    tail = newNode;
  } else {
    tail->next = newNode;
    tail = newNode;
  }

  ++reservationCount;
  return ReservationStatus::SUCCESS;
}

ReservationStatus ReservationManager::cancelReservation(
    const std::string& reservationID,
    Reservation& cancelledReservation) {
  Node* current = head;
  Node* previous = nullptr;

  while (current != nullptr &&
         current->reservation.getReservationID() != reservationID) {
    previous = current;
    current = current->next;
  }

  if (current == nullptr) {
    return ReservationStatus::RESERVATION_NOT_FOUND;
  }

  // Save the data before deleting the node so the caller can push it to the
  // cancellation-history stack.
  cancelledReservation = current->reservation;

  if (previous == nullptr) {
    head = current->next;
  } else {
    previous->next = current->next;
  }

  if (current == tail) {
    tail = previous;
  }

  delete current;
  --reservationCount;

  return ReservationStatus::SUCCESS;
}

void ReservationManager::displayActiveReservations(std::ostream& output) const {
  if (head == nullptr) {
    output << "No active reservations found.\n";
    return;
  }

  Node* current = head;
  std::size_t position = 1;

  while (current != nullptr) {
    output << "--- Active Reservation " << position << " ---\n";
    current->reservation.display(output);
    current = current->next;
    ++position;
  }
}

bool ReservationManager::isEmpty() const { return head == nullptr; }

std::size_t ReservationManager::size() const { return reservationCount; }

const char* ReservationManager::getStatusMessage(ReservationStatus status) {
  switch (status) {
    case ReservationStatus::SUCCESS:
      return "Operation completed successfully.";
    case ReservationStatus::MISSING_REQUIRED_FIELD:
      return "All reservation fields are required.";
    case ReservationStatus::INVALID_DATE:
      return "Reservation date must be a valid date in YYYY-MM-DD format.";
    case ReservationStatus::DUPLICATE_RESERVATION_ID:
      return "Reservation ID already exists.";
    case ReservationStatus::RESOURCE_NOT_FOUND:
      return "Resource ID does not exist.";
    case ReservationStatus::RESOURCE_UNAVAILABLE:
      return "Resource is unavailable for the requested date.";
    case ReservationStatus::RESERVATION_NOT_FOUND:
      return "Reservation ID was not found.";
  }

  return "Unknown reservation status.";
}

void ReservationManager::clear() {
  while (head != nullptr) {
    Node* nodeToDelete = head;
    head = head->next;
    delete nodeToDelete;
  }

  tail = nullptr;
  reservationCount = 0;
}
