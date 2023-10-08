#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using std::vector;
using std::string;

class Tensor {
public:
    Tensor() {}
    Tensor(const vector<int>& shape) : shape_(shape) {
        int total_size = 1;
        for (int dim_size : shape) {
            total_size *= dim_size;
        }
        data_.resize(total_size, 0.0);
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

    vector<float> getData() const {
        return data_;
    }

    vector<int> getShape() const {
        return shape_;
    }

private:
    vector<float> data_;
    vector<int> shape_;
};