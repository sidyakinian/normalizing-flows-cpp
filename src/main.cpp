#include <iostream>
#include <string>
#include <vector>

#include "elementwise_multiplier.hpp"
#include "utils.hpp"
#include "tensor.hpp"
#include "ops.hpp"

using std::vector;
using std::tuple;

tuple<Tensor, Tensor> rqt(
    Tensor inputs,
    Tensor unnormalized_widths,
    Tensor unnormalized_heights,
    Tensor unnormalized_derivatives,
    float left=0.0,
    float right=1.0,
    float bottom=0.0,
    float top=1.0,
    float min_bin_width=1e-3,
    float min_bin_height=1e-3,
    float min_derivative=1e-3
) {
    // if (inputs.min() < left || inputs.max() > right) {
    //     std::cerr << "Error in rqt: inputs out of bounds" << std::endl;
    // }

    int num_bins = unnormalized_widths.getShape().back();

    if (min_bin_width * num_bins > 1.0) {
        std::cerr << "Error in rqt: min_bin_width too large" << std::endl;
    }
    if (min_bin_height * num_bins > 1.0) {
        std::cerr << "Error in rqt: min_bin_height too large" << std::endl;
    }

    Tensor widths = unnormalized_widths.softmax_last_dim();
    widths = min_bin_width + (1.0 - min_bin_width * num_bins) * widths;
    Tensor cumwidths = widths.cumsum();
    cumwidths = cumwidths.pad(true, false);
    cumwidths = cumwidths + left;

    Tensor heights = unnormalized_heights.softmax_last_dim();
    heights = min_bin_height + (1.0 - min_bin_height * num_bins) * heights;
    Tensor cumheights = heights.cumsum();
    cumheights = cumheights.pad(true, false);
    cumheights = cumheights + bottom;

    Utils::printTensor(cumwidths, "cumwidths");
    Utils::printTensor(cumheights, "cumheights");

    Tensor tensor_1 = Tensor({4, 5}, true);
    Tensor tensor_2 = Tensor({4, 5}, true);

    return std::make_tuple(tensor_1, tensor_2);
}

int main(int argc, char** argv) {
    // init inptus
    Tensor inputs({1, 1, 55});
    inputs.load("tensor_files/x1_in.txt");
    Tensor unnormalized_widths({1, 1, 55, 10});
    unnormalized_widths.load("tensor_files/unnormalized_widths.txt");
    Tensor unnormalized_heights({1, 1, 55, 10});
    unnormalized_heights.load("tensor_files/unnormalized_heights.txt");
    Tensor unnormalized_derivatives({1, 1, 55, 9});
    unnormalized_derivatives.load("tensor_files/unnormalized_derivatives.txt");

    tuple<Tensor, Tensor> output_tuple = rqt(
        inputs,
        unnormalized_widths,
        unnormalized_heights,
        unnormalized_derivatives
    );

    return 0;
}