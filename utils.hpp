#include <vector>
#include <iostream>
#include <string>

class Utils {
    public:
        template<typename T>
        static void print_vector(const std::vector<T> &vector) {
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