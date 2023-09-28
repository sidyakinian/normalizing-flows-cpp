### Design decisions

This doc will describe design decisions as they are made through the implementation of this project.

1. Probably best to use 1D `std::vector` over 2D `std::vector` for better cache locality and performance.
