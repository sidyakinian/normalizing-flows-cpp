#include <iostream>
#include <string>
#include <vector>
#include "tensor.hpp"

// TODO: remove global vars
int tests_passed;
int tests_failed;

void test_tensor_comparison_same_shape() {
    Tensor t1({3, 3}, true);
    Tensor t2({3, 3}, true);
    Tensor result = t1 >= t2;
    
    vector<int> expected_shape = {3, 3};
    if (result.getShape() != expected_shape) {
        std::cerr << "Error in test_tensor_comparison_same_shape: Unexpected shape" << std::endl;
        tests_failed++;
    } else {
        tests_passed++;
    }
}

void test_tensor_comparison_broadcast_last_dim() {
    Tensor t1({3, 3}, true);
    Tensor t2({3, 1}, true);
    Tensor result = t1 >= t2;
    
    vector<int> expected_shape = {3, 3};
    if (result.getShape() != expected_shape) {
        std::cerr << "Error in test_tensor_comparison_broadcast_last_dim: Unexpected shape" << std::endl;
        tests_failed++;
    } else {
        tests_passed++;
    }
}

void test_tensor_scalar_comparison() {
    Tensor t1({3, 3}, true);
    float scalar = 0.5f;
    Tensor result = t1 >= scalar;
    
    vector<int> expected_shape = {3, 3};
    if (result.getShape() != expected_shape) {
        std::cerr << "Error in test_tensor_scalar_comparison: Unexpected shape" << std::endl;
        tests_failed++;
    } else {
        tests_passed++;
    }
}

int main() {
    test_tensor_comparison_same_shape();
    test_tensor_comparison_broadcast_last_dim();
    test_tensor_scalar_comparison();

    std::cout << "Tests completed" << std::endl;
    std::cout << "Passed: " << tests_passed << ", Failed: " << tests_failed << std::endl;
    return 0;
}
