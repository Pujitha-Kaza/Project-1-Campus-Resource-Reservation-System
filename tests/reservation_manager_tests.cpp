#include "ReservationManager.h"

#include <iostream>
#include <sstream>
#include <string>

namespace {

int testsRun = 0;
int testsFailed = 0;

void check(bool condition, const std::string& testName) {
  ++testsRun;
  if (!condition) {
    ++testsFailed;
    std::cerr << "FAILED: " << testName << '\n';
  }
}

}  // namespace

int main() {
  ReservationManager manager;
  Reservation cancelledReservation;

  std::ostringstream emptyOutput;
  manager.displayActiveReservations(emptyOutput);
  check(emptyOutput.str() == "No active reservations found.\n",
        "empty reservation list message");

  Reservation missingStudentName("R-100", "1001", "", "ROOM-1",
                                 "2026-09-20");
  check(manager.createReservation(missingStudentName, true, true) ==
            ReservationStatus::MISSING_REQUIRED_FIELD,
        "missing field is rejected");

  Reservation invalidDate("R-100", "1001", "Alice Smith", "ROOM-1",
                          "2026-02-30");
  check(manager.createReservation(invalidDate, true, true) ==
            ReservationStatus::INVALID_DATE,
        "invalid calendar date is rejected");

  Reservation leapDate("R-LEAP", "1001", "Alice Smith", "ROOM-1",
                       "2028-02-29");
  check(manager.validateReservation(leapDate, true, true) ==
            ReservationStatus::SUCCESS,
        "valid leap-year date is accepted");

  Reservation unknownResource("R-100", "1001", "Alice Smith", "ROOM-X",
                              "2026-09-20");
  check(manager.createReservation(unknownResource, false, true) ==
            ReservationStatus::RESOURCE_NOT_FOUND,
        "unknown resource is rejected");

  Reservation unavailableResource("R-100", "1001", "Alice Smith",
                                  "ROOM-1", "2026-09-20");
  check(manager.createReservation(unavailableResource, true, false) ==
            ReservationStatus::RESOURCE_UNAVAILABLE,
        "unavailable resource is rejected");

  Reservation first("R-100", "1001", "Alice Smith", "ROOM-1",
                    "2026-09-20");
  check(manager.createReservation(first, true, true) ==
            ReservationStatus::SUCCESS,
        "valid reservation is created");
  check(manager.size() == 1, "list size increases after insertion");

  Reservation duplicateID("R-100", "1002", "Bob Jones", "LAPTOP-1",
                          "2026-09-21");
  check(manager.createReservation(duplicateID, true, true) ==
            ReservationStatus::DUPLICATE_RESERVATION_ID,
        "duplicate reservation ID is rejected");

  Reservation doubleBooking("R-101", "1002", "Bob Jones", "ROOM-1",
                            "2026-09-20");
  check(manager.createReservation(doubleBooking, true, true) ==
            ReservationStatus::RESOURCE_UNAVAILABLE,
        "same resource and date cannot be double booked");

  Reservation second("R-102", "1002", "Bob Jones", "LAPTOP-1",
                     "2026-09-21");
  check(manager.createReservation(second, true, true) ==
            ReservationStatus::SUCCESS,
        "second valid reservation is created");

  std::ostringstream activeOutput;
  manager.displayActiveReservations(activeOutput);
  check(activeOutput.str().find("Reservation ID: R-100") !=
            std::string::npos,
        "display includes first reservation");
  check(activeOutput.str().find("Reservation ID: R-102") !=
            std::string::npos,
        "display traverses to second reservation");

  check(manager.cancelReservation("R-100", cancelledReservation) ==
            ReservationStatus::SUCCESS,
        "existing reservation is cancelled");
  check(cancelledReservation.getReservationID() == "R-100",
        "cancelled reservation is returned for cancellation stack");
  check(manager.size() == 1, "list size decreases after cancellation");

  check(manager.cancelReservation("R-999", cancelledReservation) ==
            ReservationStatus::RESERVATION_NOT_FOUND,
        "unknown reservation cannot be cancelled");

  check(manager.cancelReservation("R-102", cancelledReservation) ==
            ReservationStatus::SUCCESS,
        "tail reservation is cancelled");
  check(manager.isEmpty(), "list is empty after all cancellations");

  Reservation afterEmpty("R-103", "1003", "Carol Lee", "CALC-1",
                         "2026-09-22");
  check(manager.createReservation(afterEmpty, true, true) ==
            ReservationStatus::SUCCESS,
        "insertion works after list becomes empty");

  if (testsFailed == 0) {
    std::cout << "All " << testsRun << " reservation tests passed.\n";
    return 0;
  }

  std::cerr << testsFailed << " of " << testsRun << " tests failed.\n";
  return 1;
}
