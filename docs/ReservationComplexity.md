# Milestone 1 Complexity Analysis

Let `n` be the number of active reservations and `w` be the number of
students waiting for one resource.

| Operation | Time complexity | Explanation |
| --- | --- | --- |
| Insert reservation | O(n) | The new node is attached to the tail in O(1), but validation scans the active list for duplicate IDs and conflicting resource/date reservations. |
| Remove reservation | O(n) | The linked list may be traversed from the head to find the requested reservation. Link updates and deletion are O(1). |
| Traverse/display reservations | O(n) | Every active reservation node is visited once. |
| Add to waiting list | O(log r) | The waiting-list map locates a resource queue in O(log r), then queue insertion is O(1). `r` is the number of resource queues. |
| Process waiting list | O(log r) | Locating the resource queue costs O(log r); removing the first queue item is O(1). |
| Store cancellation | O(1) | A cancelled reservation is pushed onto the stack. |
| Undo cancellation | O(n) | The stack pop is O(1), but restoring the reservation calls reservation validation, which may scan the active linked list. |
| Load resources | O(m) | The input file is read once. `m` is the number of resource records. |

The active reservations use a singly linked list. The waiting list uses a
queue for each resource, so requests are processed in FIFO order. The
cancellation history uses a stack, so undo always restores the most recently
cancelled reservation first.
