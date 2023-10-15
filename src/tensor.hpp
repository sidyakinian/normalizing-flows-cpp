#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <optional>
#include <cstdlib>
#include <ctime>
#include <random>

using std::vector;
using std::string;

class Tensor {
public:
    Tensor() {}
    Tensor(const std::vector<int>& shape, bool random_init = false) : shape_(shape) {
        int total_size = 1;
        for (int dim_size : shape) {
            total_size *= dim_size;
        }
        data_.resize(total_size, 0.0);
        
        if (random_init) {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::mt19937 generator(seed);
            std::normal_distribution<float> distribution(0.0, 1.0);  // Mean = 0, Standard deviation = 1
            for (auto& val : data_) {
                val = distribution(generator);
            }
        }
    }
    ~Tensor() {}

    void load(const string& file) {
        std::ifstream infile(file);
        if (!infile.is_open()) {
            std::cerr << "Cannot open file: " << file << std::endl;
            return;
        }
        string line;
        if (std::getline(infile, line)) {
            std::istringstream iss(line);
            string value;
            int index = 0;
            while (std::getline(iss, value, ',')) {
                // remove leading and trailing whitespaces
                value.erase(0, value.find_first_not_of(" \t\n\r"));
                value.erase(value.find_last_not_of(" \t\n\r") + 1);
                data_.at(index) = std::stof(value);
                index++;
            }
        }
        infile.close();
    }

    void reshape(const vector<int>& new_shape) {
        shape_ = new_shape;
    }

    float min() const {
        float min = data_[0];
        for (float value : data_) {
            if (value < min) {
                min = value;
            }
        }
        return min;
    }

    float max() const {
        float max = data_[0];
        for (float value : data_) {
            if (value > max) {
                max = value;
            }
        }
        return max;
    }

    Tensor sliceLast(int index) {
        if (index == -1) {
            index = shape_.back() - 1;
        }

        if (index < 0 || index >= shape_.back()) {
            std::cerr << "Index out of bounds" << std::endl;
            return Tensor();
        }

        vector<int> new_shape(shape_.begin(), shape_.end() - 1);
        Tensor result(new_shape);

        int stride = 1;
        for (size_t i = 0; i < shape_.size() - 1; ++i) {
            stride *= shape_[i];
        }

        int start_index = index * stride;
        int end_index = start_index + stride;

        std::copy(data_.begin() + start_index, data_.begin() + end_index, result.data().begin());

        return result;
    }

    Tensor sliceLast(std::optional<int> start_opt = std::nullopt, std::optional<int> end_opt = std::nullopt) {
        int start = start_opt.value_or(0);
        int end = end_opt.value_or(shape_.back());

        if (start == -1) {
            start = shape_.back() - 1;
        }
        if (end == -1) {
            end = shape_.back();
        }

        if (start < 0) start = 0;
        if (end > shape_.back()) end = shape_.back();

        if (start > end) {
            std::cerr << "Invalid slice range" << std::endl;
            return Tensor();
        }

        vector<int> new_shape(shape_);
        new_shape.back() = end - start;
        Tensor result(new_shape);

        int stride = 1;
        for (size_t i = 0; i < shape_.size() - 1; ++i) {
            stride *= shape_[i];
        }

        int start_index = start * stride;
        int end_index = end * stride;

        std::copy(data_.begin() + start_index, data_.begin() + end_index, result.data().begin());

        return result;
    }

    Tensor cumsum() {
        int last_dim_size = shape_.back();
        int num_slices = data_.size() / last_dim_size;

        Tensor result = Tensor(shape_);

        for (int slice = 0; slice < num_slices; ++slice) {
            float running_total = 0.0;
            for (int idx = 0; idx < last_dim_size; ++idx) {
                int data_idx = slice * last_dim_size + idx;
                running_total += getData()[data_idx];
                result.data()[data_idx] = running_total;
            }
        }

        return result;
    }

    Tensor softmax_last_dim() const {
        Tensor result(shape_);

        int last_dim_size = shape_.back();
        int num_groups = data_.size() / last_dim_size;

        for (int i = 0; i < num_groups; i++) {
            float sum_exp = 0.0;
            for (int j = 0; j < last_dim_size; j++) {
                sum_exp += std::exp(data_[i * last_dim_size + j]);
            }

            for (int j = 0; j < last_dim_size; j++) {
                result.data_[i * last_dim_size + j] = std::exp(data_[i * last_dim_size + j]) / sum_exp;
            }
        }

        return result;
    }

    Tensor pad(bool pad_left, bool pad_right) const {
        int dims = shape_.size();
        if (dims < 2 || dims > 4) {
            std::cerr << "The function supports tensors from 2D to 4D." << std::endl;
            return Tensor();
        }

        std::vector<int> new_shape = shape_;
        new_shape.back() += (pad_left ? 1 : 0) + (pad_right ? 1 : 0);  // update the last dimension

        Tensor result(new_shape);
        
        int n = (dims > 3) ? shape_[0] : 1;
        int c = (dims > 2) ? shape_[dims - 3] : 1;
        int h = shape_[dims - 2];
        int w = shape_[dims - 1];

        for (int ni = 0; ni < n; ++ni) {
            for (int ci = 0; ci < c; ++ci) {
                for (int hi = 0; hi < h; ++hi) {
                    for (int wi = 0; wi < w; ++wi) {
                        int old_idx = (ni * c * h * w) + (ci * h * w) + (hi * w) + wi;
                        int new_idx = (ni * c * h * new_shape.back()) + (ci * h * new_shape.back()) + (hi * new_shape.back()) + wi + (pad_left ? 1 : 0);
                        result.data_[new_idx] = data_[old_idx];
                    }
                }
            }
        }
        
        return result;
    }
    
    Tensor gather(const Tensor& bin_idx) const {
        if (shape_.size() != bin_idx.shape_.size()) {
            std::cerr << "Shape mismatch!" << std::endl;
            return Tensor();
        }

        for (size_t i = 0; i < shape_.size() - 1; i++) {
            if (shape_[i] != bin_idx.shape_[i]) {
                std::cerr << "Shape mismatch!" << std::endl;
                return Tensor();
            }
        }

        if (shape_.back() < bin_idx.shape_.back()) {
            std::cerr << "Index tensor larger than the data tensor in the last dimension!" << std::endl;
            return Tensor();
        }

        Tensor result(bin_idx.shape_);

        int last_dim_size = shape_.back();
        int bin_idx_last_dim_size = bin_idx.shape_.back();
        int num_slices = data_.size() / last_dim_size;

        for (int slice = 0; slice < num_slices; ++slice) {
            for (int idx = 0; idx < bin_idx_last_dim_size; ++idx) {
                int gather_idx = static_cast<int>(bin_idx.data_[slice * bin_idx_last_dim_size + idx]);
                if (gather_idx < 0 || gather_idx >= last_dim_size) {
                    std::cerr << "Invalid index in bin_idx tensor!" << std::endl;
                    return Tensor();
                }
                result.data_[slice * bin_idx_last_dim_size + idx] = data_[slice * last_dim_size + gather_idx];
            }
        }

        return result;
    }
    
    vector<float> getData() const {
        return data_;
    }

    vector<float>& data() {
        return data_;
    }

    vector<int> getShape() const {
        return shape_;
    }

    Tensor operator+(const Tensor &other) const {
        if (shape_ != other.getShape()) {
            std::cerr << "Shape mismatch for tensor addition" << std::endl;
            return Tensor();
        }

        Tensor result(shape_);
        for (size_t i = 0; i < data_.size(); ++i) {
            result.data()[i] = data_[i] + other.getData()[i];
        }

        return result;
    }

    Tensor operator+(float scalar) const {
        Tensor result(shape_);
        for (size_t i = 0; i < data_.size(); ++i) {
            result.data()[i] = data_[i] + scalar;
        }

        return result;
    }

    Tensor operator*(const Tensor &other) const {
        if (shape_ != other.getShape()) {
            std::cerr << "Shape mismatch for tensor addition" << std::endl;
            return Tensor();
        }

        Tensor result(shape_);
        for (size_t i = 0; i < data_.size(); ++i) {
            result.data()[i] = data_[i] * other.getData()[i];
        }

        return result;
    }

    Tensor operator*(float scalar) const {
        Tensor result(shape_);
        for (size_t i = 0; i < data_.size(); ++i) {
            result.data()[i] = data_[i] * scalar;
        }

        return result;
    }

    Tensor operator>=(const Tensor &other) const {
        if (shape_ == other.getShape()) {
            Tensor result(shape_);
            for (size_t i = 0; i < data_.size(); ++i) {
                result.data()[i] = (data_[i] >= other.getData()[i]) ? 1.0f : 0.0f;
            }
            return result;
        } else if (other.getShape().back() == 1 && shape_.size() == other.getShape().size()) {
            Tensor result(shape_);
            int stride = 1;
            for (size_t i = 0; i < shape_.size() - 1; ++i) {
                stride *= shape_[i];
            }
            for (size_t i = 0; i < data_.size(); ++i) {
                result.data()[i] = (data_[i] >= other.getData()[i/stride]) ? 1.0f : 0.0f;
            }
            return result;
        } else {
            std::cerr << "Shape mismatch for comparison" << std::endl;
            return Tensor(); 
        }
    }

    Tensor operator>=(float scalar) const {
        Tensor result(shape_);
        for (size_t i = 0; i < data_.size(); ++i) {
            result.data()[i] = (data_[i] >= scalar) ? 1.0f : 0.0f;
        }
        return result;
    }

    friend Tensor operator>=(float scalar, const Tensor &tensor);
    friend Tensor operator+(float scalar, const Tensor &tensor);
    friend Tensor operator*(float scalar, const Tensor &tensor);

private:
    vector<float> data_;
    vector<int> shape_;
};

inline Tensor operator+(float scalar, const Tensor &tensor) {
    return tensor + scalar;
}

inline Tensor operator*(float scalar, const Tensor &tensor) {
    return tensor * scalar;
}

inline Tensor operator>=(float scalar, const Tensor &tensor) {
    Tensor result(tensor.getShape());
    const vector<float>& tensorData = tensor.getData();
    for (size_t i = 0; i < tensorData.size(); ++i) {
        result.data()[i] = (scalar >= tensorData[i]) ? 1.0f : 0.0f;
    }
    return result;
}