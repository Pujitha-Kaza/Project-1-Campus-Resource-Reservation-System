#include "CancellationStack.h"
#include "ReservationManager.h"
#include "ResourceManager.h"
#include "WaitingList.h"

#include <iostream>
#include <sstream>
#include <string>

namespace {

std::string readLine(const std::string& prompt) {
  std::cout << prompt;
  std::string value;
  std::getline(std::cin, value);
  return value;
}

int readMenuChoice() {
  const std::string input = readLine("Choose an option: ");
  std::stringstream parser(input);
  int choice = -1;

  if (!(parser >> choice)) {
    return -1;
  }

  return choice;
}

const char* statusMessage(ReservationStatus status) {
  switch (status) {
    case ReservationStatus::SUCCESS:
      return "Success";
    case ReservationStatus::MISSING_REQUIRED_FIELD:
      return "A required field is missing.";
    case ReservationStatus::INVALID_DATE:
      return "The date is invalid. Use YYYY-MM-DD.";
    case ReservationStatus::DUPLICATE_RESERVATION_ID:
      return "That reservation ID is already in use.";
    case ReservationStatus::RESOURCE_NOT_FOUND:
      return "The resource was not found.";
    case ReservationStatus::RESOURCE_UNAVAILABLE:
      return "The resource is not available for that date.";
    case ReservationStatus::RESERVATION_NOT_FOUND:
      return "The reservation was not found.";
  }

  return "Unknown reservation status.";
}

void printMenu() {
  std::cout << "\n===== Campus Resource Reservation System =====\n"
            << "1. Display all resources\n"
            << "2. Display resource availability\n"
            << "3. Create reservation\n"
            << "4. Cancel reservation\n"
            << "5. Display active reservations\n"
            << "6. Add student to waiting list\n"
            << "7. Display waiting lists\n"
            << "8. Process next waiting request\n"
            << "9. Display cancellation history\n"
            << "10. Undo last cancellation\n"
            << "0. Exit\n";
}

void addWaitingRequest(ResourceManager& resources, WaitingList& waitingList) {
  const std::string studentID = readLine("Student ID: ");
  const std::string studentName = readLine("Student name: ");
  const std::string resourceID = readLine("Resource ID: ");
  const std::string requestDate = readLine("Requested date (YYYY-MM-DD): ");

  if (!resources.contains(resourceID)) {
    std::cout << "The resource was not found.\n";
    return;
  }

  if (waitingList.isStudentAlreadyWaiting(studentID, resourceID)) {
    std::cout << "This student is already waiting for that resource.\n";
    return;
  }

  waitingList.addToWaitingList(
      WaitingRequest(studentID, studentName, resourceID, requestDate));
  std::cout << "Student added to the waiting list.\n";
}

void createReservation(ReservationManager& reservations,
                       ResourceManager& resources,
                       WaitingList& waitingList) {
  const std::string reservationID = readLine("Reservation ID: ");
  const std::string studentID = readLine("Student ID: ");
  const std::string studentName = readLine("Student name: ");
  const std::string resourceID = readLine("Resource ID: ");
  const std::string reservationDate =
      readLine("Reservation date (YYYY-MM-DD): ");

  const bool resourceExists = resources.contains(resourceID);
  const bool resourceAvailable = resources.isAvailable(resourceID);
  const Reservation reservation(reservationID, studentID, studentName,
                                resourceID, reservationDate);
  const ReservationStatus result = reservations.createReservation(
      reservation, resourceExists, resourceAvailable);

  std::cout << statusMessage(result) << '\n';

  if (result == ReservationStatus::SUCCESS) {
    resources.setStatus(resourceID, AvailabilityStatus::RESERVED);
    return;
  }

  if (result == ReservationStatus::RESOURCE_UNAVAILABLE) {
    if (!waitingList.isStudentAlreadyWaiting(studentID, resourceID)) {
      waitingList.addToWaitingList(
          WaitingRequest(studentID, studentName, resourceID, reservationDate));
      std::cout << "The student was added to the waiting list.\n";
    } else {
      std::cout << "The student is already on the waiting list.\n";
    }
  }
}

void promoteWaitingStudent(ReservationManager& reservations,
                           ResourceManager& resources,
                           WaitingList& waitingList,
                           const std::string& resourceID) {
  if (!waitingList.hasWaitingStudents(resourceID) ||
      !resources.isAvailable(resourceID)) {
    return;
  }

  const WaitingRequest request = waitingList.getNextWaitingRequest(resourceID);
  const std::string generatedID =
      "AUTO-" + request.getStudentId() + "-" + request.getRequestDate();
  const Reservation reservation(generatedID, request.getStudentId(),
                                request.getStudentName(), resourceID,
                                request.getRequestDate());

  const ReservationStatus result =
      reservations.createReservation(reservation, true, true);

  if (result == ReservationStatus::SUCCESS) {
    resources.setStatus(resourceID, AvailabilityStatus::RESERVED);
    waitingList.removeNextWaitingRequest(resourceID);
    std::cout << "The next waiting-list student was given a reservation.\n";
  }
}

void cancelReservation(ReservationManager& reservations,
                       ResourceManager& resources,
                       WaitingList& waitingList,
                       CancellationStack& cancellationHistory) {
  const std::string reservationID = readLine("Reservation ID to cancel: ");
  Reservation cancelledReservation;
  const ReservationStatus result =
      reservations.cancelReservation(reservationID, cancelledReservation);

  std::cout << statusMessage(result) << '\n';

  if (result != ReservationStatus::SUCCESS) {
    return;
  }

  resources.setStatus(cancelledReservation.getResourceID(),
                      AvailabilityStatus::AVAILABLE);
  cancellationHistory.push(cancelledReservation);
  if (waitingList.hasWaitingStudents(cancelledReservation.getResourceID())) {
    std::cout << "Students are waiting for this resource. Use the waiting "
                 "list processing option when ready.\n";
  }
}

void undoCancellation(ReservationManager& reservations,
                      ResourceManager& resources,
                      CancellationStack& cancellationHistory) {
  const Reservation* latest = cancellationHistory.peek();
  if (latest == nullptr) {
    std::cout << "No cancelled reservations to restore.\n";
    return;
  }

  if (!resources.contains(latest->getResourceID()) ||
      !resources.isAvailable(latest->getResourceID())) {
    std::cout << "The resource is not available, so the reservation cannot "
                 "be restored yet.\n";
    return;
  }

  const Reservation restored = *latest;
  const ReservationStatus result = reservations.createReservation(
      restored, true, true);

  if (result != ReservationStatus::SUCCESS) {
    std::cout << "The reservation could not be restored: "
              << statusMessage(result) << '\n';
    return;
  }

  resources.setStatus(restored.getResourceID(),
                      AvailabilityStatus::RESERVED);
  Reservation removedFromHistory;
  cancellationHistory.pop(removedFromHistory);
  std::cout << "The most recently cancelled reservation was restored.\n";
}

}  // namespace

int main() {
  ResourceManager resources;
  std::string loadError;

  if (!resources.loadFromFile("Resources.txt", loadError)) {
    std::cerr << loadError << '\n';
    return 1;
  }

  ReservationManager reservations;
  WaitingList waitingList;
  CancellationStack cancellationHistory;

  std::cout << resources.size() << " resources loaded.\n";

  while (true) {
    printMenu();
    const int choice = readMenuChoice();

    switch (choice) {
      case 1:
        resources.displayAll(std::cout);
        break;
      case 2:
        resources.displayAvailability(std::cout);
        break;
      case 3:
        createReservation(reservations, resources, waitingList);
        break;
      case 4:
        cancelReservation(reservations, resources, waitingList,
                          cancellationHistory);
        break;
      case 5:
        reservations.displayActiveReservations(std::cout);
        break;
      case 6:
        addWaitingRequest(resources, waitingList);
        break;
      case 7:
        waitingList.displayAllWaitingLists();
        break;
      case 8:
        {
          const std::string resourceID =
              readLine("Resource ID to process: ");
          if (!resources.contains(resourceID)) {
            std::cout << "The resource was not found.\n";
          } else if (!waitingList.hasWaitingStudents(resourceID)) {
            std::cout << "No students are waiting for that resource.\n";
          } else if (!resources.isAvailable(resourceID)) {
            std::cout << "The resource is not currently available.\n";
          } else {
            promoteWaitingStudent(reservations, resources, waitingList,
                                  resourceID);
          }
        }
        break;
      case 9:
        cancellationHistory.display(std::cout);
        break;
      case 10:
        undoCancellation(reservations, resources, cancellationHistory);
        break;
      case 0:
        std::cout << "Goodbye.\n";
        return 0;
      default:
        std::cout << "Invalid menu choice.\n";
        break;
    }
  }
}
