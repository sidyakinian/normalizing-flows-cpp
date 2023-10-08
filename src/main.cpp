#include <iostream>
#include <string>
#include <vector>

#include "elementwise_multiplier.hpp"
#include "utils.hpp"
#include "tensor.hpp"
#include "ops.hpp"

using std::vector;

int main(int argc, char** argv) {
    Tensor tensor({5, 3, 4});
    tensor.load("test_tensor.txt");
    Utils::print_tensor(tensor);

    Tensor sum_tensor = Ops::sum(tensor, 0);
    Utils::print_tensor(sum_tensor);

    return 0;
}