#include <iostream>
#include <string>
#include <vector>
#include "tensor.hpp"

void test_sliceLast_indexing() {
    Tensor tensor({3, 4, 5});
    tensor.load("../test_tensor.txt");
    Tensor result = tensor.sliceLast(1);
    
    vector<int> expected_shape = {3, 4};
    if (result.getShape() != expected_shape) {
        std::cerr << "Error in test_sliceLast_indexing: Unexpected shape" << std::endl;
    }
}

void test_sliceLast_slicing_full() {
    Tensor tensor({3, 4, 5});
    tensor.load("../test_tensor.txt");
    Tensor result = tensor.sliceLast(0, 2);

    vector<int> expected_shape = {3, 4, 2};
    if (result.getShape() != expected_shape) {
        std::cerr << "Error in test_sliceLast_slicing_full: Unexpected shape" << std::endl;
    }
}

void test_sliceLast_slicing_left_bound_only() {
    Tensor tensor({3, 4, 5});
    tensor.load("../test_tensor.txt");
    Tensor result = tensor.sliceLast(2, std::nullopt);

    vector<int> expected_shape = {3, 4, 3};
    if (result.getShape() != expected_shape) {
        std::cerr << "Error in test_sliceLast_slicing_left_bound_only: Unexpected shape" << std::endl;
    }
}

void test_sliceLast_slicing_right_bound_only() {
    Tensor tensor({3, 4, 5});
    tensor.load("../test_tensor.txt");
    Tensor result = tensor.sliceLast(std::nullopt, 3);

    vector<int> expected_shape = {3, 4, 3};
    if (result.getShape() != expected_shape) {
        std::cerr << "Error in test_sliceLast_slicing_right_bound_only: Unexpected shape" << std::endl;
    }
}

int main() {
    test_sliceLast_indexing();
    test_sliceLast_slicing_full();
    test_sliceLast_slicing_left_bound_only();
    test_sliceLast_slicing_right_bound_only();
    
    std::cout << "Tests completed" << std::endl;
    return 0;
}