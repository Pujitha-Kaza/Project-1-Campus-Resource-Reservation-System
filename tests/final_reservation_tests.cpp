#include "ReservationManager.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

std::size_t positionOf(const std::string& text, const std::string& value) {
  const std::size_t position = text.find(value);
  return position == std::string::npos ? text.size() : position;
}

}  // namespace

int main() {
  ReservationManager manager;
  const Reservation december("R-DEC", "1001", "Alice", "R101",
                             "2026-12-20");
  const Reservation september("R-SEP", "1002", "Bob", "R102",
                              "2026-09-20");
  const Reservation october("R-OCT", "1001", "Alice", "R103",
                            "2026-10-20");

  check(manager.createReservation(december, true, true) ==
            ReservationStatus::SUCCESS,
        "December reservation is created");
  check(manager.createReservation(september, true, true) ==
            ReservationStatus::SUCCESS,
        "September reservation is created");
  check(manager.createReservation(october, true, true) ==
            ReservationStatus::SUCCESS,
        "October reservation is created");

  Reservation found;
  check(manager.findReservationByID("R-SEP", found),
        "linear search finds reservation by ID");
  check(found.getStudentName() == "Bob",
        "reservation search returns the correct record");
  check(!manager.findReservationByID("R-MISSING", found),
        "search reports a missing reservation");

  const std::vector<Reservation> studentReservations =
      manager.findReservationsByStudentID("1001");
  check(studentReservations.size() == 2,
        "student search returns all matching reservations");

  std::ostringstream sortedOutput;
  manager.displayReservationsSortedByDate(sortedOutput);
  const std::string sortedText = sortedOutput.str();
  check(positionOf(sortedText, "Reservation ID: R-SEP") <
            positionOf(sortedText, "Reservation ID: R-OCT"),
        "merge sort places September before October");
  check(positionOf(sortedText, "Reservation ID: R-OCT") <
            positionOf(sortedText, "Reservation ID: R-DEC"),
        "merge sort places October before December");

  std::ostringstream reportOutput;
  manager.displayActiveReservationReport(reportOutput);
  check(reportOutput.str().find("Total active reservations: 3") !=
            std::string::npos,
        "active reservation report includes the total");

  if (testsFailed == 0) {
    std::cout << "All " << testsRun << " final reservation tests passed.\n";
    return 0;
  }

  std::cerr << testsFailed << " of " << testsRun << " tests failed.\n";
  return 1;
}
