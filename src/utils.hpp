#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "tensor.hpp"

class Utils {
    public:
        static void print_tensor(const Tensor &tensor) {
            vector<int> shape = tensor.getShape();
            if (shape.empty()) {
                return;
            }

            if (shape.size() == 1) {
                for (int i = 0; i < shape[0]; ++i) {
                    std::cout << tensor.getData()[i] << " ";
                }
                std::cout << std::endl;
            } else if (shape.size() == 2) {
                for (int i = 0; i < shape[0]; ++i) {
                    for (int j = 0; j < shape[1]; ++j) {
                        std::cout << tensor.getData()[i * shape[1] + j] << " ";
                    }
                    std::cout << std::endl;
                }
            } else if (shape.size() == 3) {
                for (int i = 0; i < shape[0]; ++i) {
                    for (int j = 0; j < shape[1]; ++j) {
                        for (int k = 0; k < shape[2]; ++k) {
                            std::cout << tensor.getData()[i * shape[1] * shape[2] + j * shape[2] + k] << " ";
                        }
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;
                }
            } else if (shape.size() == 4) {
                for (int i = 0; i < shape[0]; ++i) {
                    for (int j = 0; j < shape[1]; ++j) {
                        for (int k = 0; k < shape[2]; ++k) {
                            for (int l = 0; l < shape[3]; ++l) {
                                std::cout << tensor.getData()[i * shape[1] * shape[2] * shape[3] + j * shape[2] * shape[3] + k * shape[3] + l] << " ";
                            }
                            std::cout << std::endl;
                        }
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;
                }
            } else {
                std::cerr << "Unsupported tensor shape size for printing." << std::endl;
            }
        }
};