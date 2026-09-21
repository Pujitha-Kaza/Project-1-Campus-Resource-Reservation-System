# Project 1: Campus Resource Reservation System

This branch contains the Milestone 1 reservation code.

## Reservation Management Features

- Create and cancel reservations
- Validate reservation information
- Store active reservations in a linked list
- Display active reservations
- Return cancelled reservations for the cancellation stack
- Report unavailable resources for the waiting queue

## Files

- `include/` - header files
- `src/` - implementation files
- `tests/` - reservation tests
- `docs/` - complexity analysis

## Integration

`createReservation` receives the resource existence and availability results
from Resource Management. `cancelReservation` returns the removed reservation
so it can be added to the cancellation stack.

## Compile and Test

From the repository root:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude \
  src/Reservation.cpp src/ReservationManager.cpp \
  tests/reservation_manager_tests.cpp -o reservation_tests
./reservation_tests
```
