#ifndef CANCELLATION_STACK_H
#define CANCELLATION_STACK_H

#include "Reservation.h"

#include <cstddef>
#include <iosfwd>
#include <stack>

class CancellationStack {
 private:
  std::stack<Reservation> cancelledReservations;

 public:
  void push(const Reservation& reservation);
  bool pop(Reservation& reservation);
  const Reservation* peek() const;
  void display(std::ostream& output) const;
  bool isEmpty() const;
  std::size_t size() const;
};

#endif
