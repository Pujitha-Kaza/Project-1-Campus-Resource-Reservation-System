# Reservation Management Complexity

The active reservations are stored in a singly linked list with both `head`
and `tail` pointers.

| Operation | Time Complexity | Explanation |
| --- | --- | --- |
| Insert reservation | O(1) after validation | The new node is attached directly to the tail. Request validation is O(n) because the list is checked for duplicate IDs and conflicting resource/date reservations. Therefore, the complete create operation is O(n). |
| Remove reservation | O(n) | The list may need to be traversed from the head to locate the requested reservation ID. Updating links and deleting the located node are O(1). |
| Traverse/display reservations | O(n) | Each active reservation node is visited exactly once. |

The cancelled reservation is returned by `cancelReservation`. The integrated
system can push that value onto the cancellation-history stack in O(1). If a
resource is unavailable, `createReservation` returns
`RESOURCE_UNAVAILABLE`, allowing the integrated system to enqueue the student
in its waiting-list queue.
