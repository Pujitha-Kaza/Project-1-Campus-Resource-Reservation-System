#include "ReservationManager.h"

#include <cctype>
#include <ostream>
#include <vector>

namespace {

bool isLeapYear(int year) {
  return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

bool comesBefore(const Reservation& first, const Reservation& second) {
  if (first.getReservationDate() != second.getReservationDate()) {
    return first.getReservationDate() < second.getReservationDate();
  }

  return first.getReservationID() < second.getReservationID();
}

void mergeReservations(std::vector<Reservation>& reservations,
                       std::vector<Reservation>& temporary,
                       std::size_t first,
                       std::size_t middle,
                       std::size_t last) {
  std::size_t left = first;
  std::size_t right = middle + 1;
  std::size_t position = first;

  while (left <= middle && right <= last) {
    if (comesBefore(reservations[left], reservations[right])) {
      temporary[position] = reservations[left];
      ++left;
    } else {
      temporary[position] = reservations[right];
      ++right;
    }
    ++position;
  }

  while (left <= middle) {
    temporary[position] = reservations[left];
    ++left;
    ++position;
  }

  while (right <= last) {
    temporary[position] = reservations[right];
    ++right;
    ++position;
  }

  for (std::size_t index = first; index <= last; ++index) {
    reservations[index] = temporary[index];
  }
}

void mergeSortReservations(std::vector<Reservation>& reservations,
                           std::vector<Reservation>& temporary,
                           std::size_t first,
                           std::size_t last) {
  if (first >= last) {
    return;
  }

  const std::size_t middle = first + (last - first) / 2;
  mergeSortReservations(reservations, temporary, first, middle);
  mergeSortReservations(reservations, temporary, middle + 1, last);
  mergeReservations(reservations, temporary, first, middle, last);
}

}  // namespace

ReservationManager::ReservationManager()
    : head(nullptr), tail(nullptr), reservationCount(0) {}

ReservationManager::~ReservationManager() { clear(); }

bool ReservationManager::isBlank(const std::string& value) {
  return value.find_first_not_of(" \t\n\r") == std::string::npos;
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

  if (!resourceExists) {
    return ReservationStatus::RESOURCE_NOT_FOUND;
  }

  if (!resourceAvailable) {
    return ReservationStatus::RESOURCE_UNAVAILABLE;
  }

  // Check the current reservations.
  Node* current = head;
  while (current != nullptr) {
    const Reservation& active = current->reservation;

    if (active.getReservationID() == reservation.getReservationID()) {
      return ReservationStatus::DUPLICATE_RESERVATION_ID;
    }

    if (active.getResourceID() == reservation.getResourceID() &&
        active.getReservationDate() == reservation.getReservationDate()) {
      return ReservationStatus::RESOURCE_UNAVAILABLE;
    }

    current = current->next;
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

  // Save the cancelled reservation for the stack.
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

bool ReservationManager::findReservationByID(
    const std::string& reservationID,
    Reservation& foundReservation) const {
  Node* current = head;

  while (current != nullptr) {
    if (current->reservation.getReservationID() == reservationID) {
      foundReservation = current->reservation;
      return true;
    }

    current = current->next;
  }

  return false;
}

std::vector<Reservation> ReservationManager::findReservationsByStudentID(
    const std::string& studentID) const {
  std::vector<Reservation> matches;
  Node* current = head;

  while (current != nullptr) {
    if (current->reservation.getStudentID() == studentID) {
      matches.push_back(current->reservation);
    }

    current = current->next;
  }

  return matches;
}

std::vector<Reservation> ReservationManager::getActiveReservations() const {
  std::vector<Reservation> activeReservations;
  Node* current = head;

  while (current != nullptr) {
    activeReservations.push_back(current->reservation);
    current = current->next;
  }

  return activeReservations;
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

void ReservationManager::displayReservationsSortedByDate(
    std::ostream& output) const {
  std::vector<Reservation> sortedReservations = getActiveReservations();

  if (sortedReservations.empty()) {
    output << "No active reservations found.\n";
    return;
  }

  std::vector<Reservation> temporary(sortedReservations.size());
  mergeSortReservations(sortedReservations, temporary, 0,
                        sortedReservations.size() - 1);

  output << "===== Reservations Sorted by Date =====\n";
  for (std::size_t index = 0; index < sortedReservations.size(); ++index) {
    output << "--- Sorted Reservation " << index + 1 << " ---\n";
    sortedReservations[index].display(output);
  }
}

void ReservationManager::displayActiveReservationReport(
    std::ostream& output) const {
  output << "===== Active Reservation Report =====\n";
  output << "Total active reservations: " << reservationCount << '\n';
  displayActiveReservations(output);
}

bool ReservationManager::isEmpty() const { return head == nullptr; }

std::size_t ReservationManager::size() const { return reservationCount; }

void ReservationManager::clear() {
  while (head != nullptr) {
    Node* nodeToDelete = head;
    head = head->next;
    delete nodeToDelete;
  }

  tail = nullptr;
  reservationCount = 0;
}
