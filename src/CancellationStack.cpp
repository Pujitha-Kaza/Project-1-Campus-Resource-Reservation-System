#include "CancellationStack.h"

#include <ostream>

void CancellationStack::push(const Reservation& reservation) {
  cancelledReservations.push(reservation);
}

bool CancellationStack::pop(Reservation& reservation) {
  if (cancelledReservations.empty()) {
    return false;
  }

  reservation = cancelledReservations.top();
  cancelledReservations.pop();
  return true;
}

const Reservation* CancellationStack::peek() const {
  if (cancelledReservations.empty()) {
    return nullptr;
  }

  return &cancelledReservations.top();
}

void CancellationStack::display(std::ostream& output) const {
  if (cancelledReservations.empty()) {
    output << "No cancelled reservations found.\n";
    return;
  }

  std::stack<Reservation> copy = cancelledReservations;
  std::size_t position = 1;

  while (!copy.empty()) {
    output << "--- Cancelled Reservation " << position << " ---\n";
    copy.top().display(output);
    copy.pop();
    ++position;
  }
}

bool CancellationStack::isEmpty() const {
  return cancelledReservations.empty();
}

std::size_t CancellationStack::size() const {
  return cancelledReservations.size();
}
