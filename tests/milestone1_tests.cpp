#include "CancellationStack.h"
#include "ReservationManager.h"
#include "ResourceManager.h"
#include "WaitingList.h"

#include <iostream>
#include <sstream>
#include <string>

namespace {

int testsRun = 0;
int testsFailed = 0;

void check(bool condition, const std::string& name) {
  ++testsRun;
  if (!condition) {
    ++testsFailed;
    std::cerr << "FAILED: " << name << '\n';
  }
}

}  // namespace

int main() {
  ResourceManager resources;
  std::string loadError;
  check(resources.loadFromFile("Resources.txt", loadError),
        "resource file loads");
  check(resources.size() == 20, "all resource records are loaded");
  check(resources.contains("R101"), "resource ID can be found");
  check(resources.isAvailable("R101"), "available resource is recognized");
  check(!resources.isAvailable("R103"), "unavailable resource is recognized");

  std::ostringstream resourceOutput;
  resources.displayAvailability(resourceOutput);
  check(resourceOutput.str().find("R101") != std::string::npos,
        "resource availability is displayed");

  WaitingList waitingList;
  waitingList.addToWaitingList(
      WaitingRequest("1001", "Alice", "R103", "2026-09-20"));
  waitingList.addToWaitingList(
      WaitingRequest("1002", "Bob", "R103", "2026-09-21"));
  check(waitingList.getWaitingCount("R103") == 2,
        "waiting list stores requests");
  check(waitingList.getNextWaitingRequest("R103").getStudentId() == "1001",
        "waiting list follows FIFO order");
  waitingList.removeNextWaitingRequest("R103");
  check(waitingList.getWaitingCount("R103") == 1,
        "waiting list removes the first request");

  ReservationManager reservations;
  Reservation cancelled;
  const Reservation reservation("R-1", "1001", "Alice", "R101",
                                "2026-09-20");
  check(reservations.createReservation(reservation, true, true) ==
            ReservationStatus::SUCCESS,
        "reservation can be created");
  check(reservations.cancelReservation("R-1", cancelled) ==
            ReservationStatus::SUCCESS,
        "reservation can be cancelled");

  CancellationStack history;
  history.push(cancelled);
  check(history.size() == 1, "cancelled reservation enters stack");
  check(history.peek() != nullptr &&
            history.peek()->getReservationID() == "R-1",
        "most recent cancellation is at the top");

  Reservation restored;
  check(history.pop(restored), "most recent cancellation can be restored");
  check(restored.getReservationID() == "R-1" && history.isEmpty(),
        "stack removes the restored reservation");
  check(reservations.createReservation(restored, true, true) ==
            ReservationStatus::SUCCESS,
        "restored reservation can be added back");

  if (testsFailed == 0) {
    std::cout << "All " << testsRun << " milestone 1 tests passed.\n";
    return 0;
  }

  std::cerr << testsFailed << " of " << testsRun << " tests failed.\n";
  return 1;
}
