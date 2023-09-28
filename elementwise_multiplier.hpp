#include <vector>

using std::vector;

class ElementwiseMultiplier {
    int size;

    public:
        ElementwiseMultiplier(int size) {
            this->size = size * size;
            // load weights with size by size vector 1d of random floats
            for (int i = 0; i < (this->size) /* multiplication optimized away by compiler */; i++) {
                weights.push_back(rand() % 100);
            }
        }
        ~ElementwiseMultiplier() {
            // pass
        };
        vector<float> multiply(vector<float> matrix) {
            vector<float> output;
            output.resize(size * size);
            for (int i = 0; i < this->size; i++) {
                output[i] = matrix[i] * weights[i];
            }
            return output;
        }
    private:
        vector<float> weights;
};