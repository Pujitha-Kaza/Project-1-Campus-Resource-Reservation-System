# User Guide

## Starting the program

Run the program from the repository root so that it can find `Resources.txt`.
The program loads the resource records before showing the menu.

## Menu options

1. **Display all resources** shows each resource ID, name, type, and status.
2. **Display resource availability** shows a shorter availability list.
3. **Create reservation** asks for reservation, student, resource, and date information.
4. **Cancel reservation** removes an active reservation and stores it in cancellation history.
5. **Display active reservations** traverses the linked list and prints active records.
6. **Add student to waiting list** adds a FIFO request for a valid resource.
7. **Display waiting lists** prints the current queue for every resource.
8. **Process next waiting request** creates a reservation for the first waiting student when the resource is available.
9. **Display cancellation history** prints the cancellation stack from newest to oldest.
10. **Undo last cancellation** restores the most recently cancelled reservation when its resource is available.
11. **Search reservations** searches by reservation ID or student ID using linear search.
12. **Display reservations sorted by date** uses the project's merge-sort implementation.
13. **Display active reservation report** shows the active reservation count and records.
0. **Exit** closes the program.

Dates must use `YYYY-MM-DD`. A resource that is not found or is not available
cannot be reserved. When a reservation cannot be created because a resource is
unavailable, the student is added to that resource's waiting list.
