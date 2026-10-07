# Campus Resource Reservation System

This repository contains the Milestone 1 campus resource reservation system.

## Features

- Loads resources from `Resources.txt`.
- Displays resources and their availability.
- Creates and validates reservations.
- Stores active reservations in a singly linked list.
- Cancels reservations and records them in a stack.
- Restores the most recently cancelled reservation.
- Adds students to FIFO waiting lists when a resource is unavailable.
- Displays active reservations, waiting lists, and cancellation history.

## Build and run

From the repository root, compile the application with:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -I. -Iinclude \
  Resource.cpp ResourceManager.cpp WaitingRequest.cpp WaitingList.cpp \
  src/Reservation.cpp src/ReservationManager.cpp src/CancellationStack.cpp \
  main.cpp -o campus_reservation
```

Run the program with:

```bash
./campus_reservation
```

The program expects `Resources.txt` in the current directory. Each resource
record uses this format:

```text
resource ID|resource name|resource type|availability status
```

## Tests

Run the reservation tests:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude \
  src/Reservation.cpp src/ReservationManager.cpp \
  tests/reservation_manager_tests.cpp -o reservation_manager_tests
./reservation_manager_tests
```

Run the integrated Milestone 1 tests:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -I. -Iinclude \
  Resource.cpp ResourceManager.cpp WaitingRequest.cpp WaitingList.cpp \
  src/Reservation.cpp src/ReservationManager.cpp src/CancellationStack.cpp \
  tests/milestone1_tests.cpp -o milestone1_tests
./milestone1_tests
```

## Project structure

- `Resource.*` and `ResourceManager.*` manage resource data and file loading.
- `include/Reservation*.h` and `src/Reservation*.cpp` implement reservations.
- `include/CancellationStack.h` and `src/CancellationStack.cpp` implement cancellation history.
- `Waiting*.h/.cpp` implement FIFO waiting lists.
- `main.cpp` integrates the components through the user menu.
- `docs/ReservationComplexity.md` contains the complexity analysis.
- `docs/UserGuide.md` explains the menu and normal user flow.
