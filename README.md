# Project 1: Campus Resource Reservation System

This branch contains the Milestone 1 Reservation Management component.

## Reservation Management Features

- Creates and validates reservation requests.
- Rejects missing fields, invalid dates, duplicate reservation IDs, unknown
  resources, unavailable resources, and resource/date conflicts.
- Stores active reservations in a custom singly linked list.
- Removes reservations by ID and returns the cancelled record for the
  cancellation-history stack.
- Traverses and displays all active reservations.
- Reports unavailable resources so the main program can add the student to the
  waiting-list queue.

## Files

- `include/Reservation.h` - reservation data model
- `include/ReservationManager.h` - reservation operations and result statuses
- `src/Reservation.cpp` - reservation implementation and display output
- `src/ReservationManager.cpp` - validation and linked-list implementation
- `tests/reservation_manager_tests.cpp` - focused component tests
- `docs/ReservationComplexity.md` - Big-O analysis for reservation operations

## Integration Contract

The Resource Management component checks whether the requested resource exists
and is currently available. It passes those results to
`createReservation(reservation, resourceExists, resourceAvailable)`.

When creation returns `RESOURCE_UNAVAILABLE`, the menu/controller can enqueue
the student in the waiting list. When `cancelReservation` succeeds, the
returned `Reservation` can be pushed onto the cancellation-history stack before
the resource and its waiting list are updated.

## Compile and Test

From the repository root:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude \
  src/Reservation.cpp src/ReservationManager.cpp \
  tests/reservation_manager_tests.cpp -o reservation_tests
./reservation_tests
```
