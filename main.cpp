#include <iostream>
#include <string>
#include <vector>

#include "elementwise_multiplier.hpp"

using std::vector;

int main(int argc, char** argv) {

    // Create random input matrix (flattened to 1d)
    int size = 10;
    vector<float> input_matrix;
    input_matrix.resize(size * size);
    for (int i = 0; i < (size * size); i++) {
        input_matrix[i] = rand() % 100;
    }

    ElementwiseMultiplier element_wise_multiplier = ElementwiseMultiplier(size);
    vector<float> output_matrix = element_wise_multiplier.multiply(input_matrix);

    return 0;
}