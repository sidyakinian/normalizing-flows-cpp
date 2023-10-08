#pragma once

class Ops {
public:
    static Tensor sum(const Tensor& tensor, int dim) {
        const vector<int>& shape = tensor.getShape();

        if (dim == -1) {
            dim = shape.size() - 1;
        }

        if (dim < 0 || dim >= shape.size()) {
            std::cerr << "Invalid dimension" << std::endl;
            return Tensor();
        }

        vector<int> new_shape = shape;
        new_shape[dim] = 1;

        Tensor result(new_shape);
        const vector<float>& data = tensor.getData();
        vector<float> result_data(result.getData().size(), 0.0);

        int stride = 1;
        for (int i = 0; i < dim; ++i) {
            stride *= shape[i];
        }

        int dim_size = shape[dim];
        int outer_size = result.getData().size() / stride;

        for (int i = 0; i < outer_size; ++i) {
            for (int j = 0; j < stride; ++j) {
                float sum_val = 0.0;
                for (int k = 0; k < dim_size; ++k) {
                    sum_val += data[i * stride * dim_size + j + k * stride];
                }
                result_data[i * stride + j] = sum_val;
            }
        }

        std::copy(result_data.begin(), result_data.end(), result.data().begin());

        return result;
    }
};
