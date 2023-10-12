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
    Tensor(const vector<int>& shape, bool random_init = false) : shape_(shape) {
        int total_size = 1;
        for (int dim_size : shape) {
            total_size *= dim_size;
        }
        data_.resize(total_size, 0.0);
        
        if (random_init) {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::mt19937 generator(seed);
            std::uniform_real_distribution<float> distribution(0.0, 1.0);
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
                // Remove leading and trailing whitespaces
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
                running_total += result.getData()[data_idx];
                result.data()[data_idx] = running_total;
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

private:
    vector<float> data_;
    vector<int> shape_;
};

inline Tensor operator+(float scalar, const Tensor &tensor) {
    return tensor + scalar;
}

inline Tensor operator>=(float scalar, const Tensor &tensor) {
    Tensor result(tensor.getShape());
    const vector<float>& tensorData = tensor.getData();
    for (size_t i = 0; i < tensorData.size(); ++i) {
        result.data()[i] = (scalar >= tensorData[i]) ? 1.0f : 0.0f;
    }
    return result;
}