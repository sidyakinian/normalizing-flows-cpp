#include <iostream>
#include <string>
#include <vector>

#include "elementwise_multiplier.hpp"
#include "utils.hpp"
#include "tensor.hpp"

using std::vector;

int main(int argc, char** argv) {
    Tensor tensor({3, 3});
    tensor.load("test_tensor.txt");
    Utils::print_tensor(tensor);

    return 0;
}