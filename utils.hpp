#include <vector>
#include <iostream>
#include <string>

class Utils {
    public:
        static void print_vector(std::vector<float> vector) {
            std::cout << "[";
            for (int i = 0; i < vector.size(); i++) {
                std::cout << vector[i];
                if (i != vector.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "]" << std::endl;
        }
};