## Resource Management algorithms

Let `n` be the number of resources and `m` the number of active reservations.

**- Loading Resources: ** O(n) time and O(1) extra space | each file record is parsed once and stored in the resource vector.

**- Resource ID search:** O(n) time and O(1) extra space. | A manual linear scan checks resource IDs until it finds a match or reaches the end.

**- Resource merge sort by name:** O(n log n) time and O(n) extra space. | Merge sort operates on a copy to preserve the original list and all resource fields.

**- Utilization report:** O(nm) time and O(1) extra counting space. | For each resource, the report scans active reservations and counts matching resource IDs. Thus, emitting zero for resources with no matches.
