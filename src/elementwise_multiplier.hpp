#include <vector>

using std::vector;

class ElementwiseMultiplier {
    int total_size;

    public:
        ElementwiseMultiplier(int size) {
            this->total_size = size * size;
            for (int i = 0; i < (this->total_size) /* multiplication optimized away by compiler */; i++) {
                weights.push_back(rand() % 100);
            }
        }
        ~ElementwiseMultiplier() {};
        vector<float> multiply(vector<float> matrix) {
            vector<float> output;
            output.resize(this->total_size);
            for (int i = 0; i < this->total_size; i++) {
                output[i] = matrix[i] * weights[i];
            }
            return output;
        }
    private:
        vector<float> weights;
};