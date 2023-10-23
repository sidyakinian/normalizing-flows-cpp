<img width="1000" alt="flows" src="https://github.com/sidyakinian/normalizing-flows-cpp/assets/34050187/a7b48ac8-4760-4e02-916c-de6fbd1b2dcf">

# Normalizing flows in C++

This project implements normalizing flows in C++, specifically with rational quadratic transforms. It's part of my effort to build a fully local, high quality, real-time text-to-speech iOS app.

### Progress

Implementing a normalizing flow from scratch in C++ requires building abstractions for tensor operations – essentially creating a very small linear algebra library. To this end, I implement a tensor class with the following interface:

```c++
class Tensor {
public:
    Tensor() {}
    Tensor(const std::vector<int>& shape, bool random_init = false) : shape_(shape);
    ~Tensor();

    void load(const string& file);

    // operators...

    friend Tensor operator>=(float scalar, const Tensor &tensor);
    friend Tensor operator+(float scalar, const Tensor &tensor);
    friend Tensor operator*(float scalar, const Tensor &tensor);

private:
    vector<float> data_;
    vector<int> shape_;
};
```

Below is the list of all tensor operations that are going to be implemented. Some operations are only implemented for particular dimensions or number of dimensions since that's all that's required by the normalizing flow function.

- [x] `min`/`max`
- [x] `gather`
- [x] `reshape'
- [x] `softmax` (last dimension only)
- [x] `slice` (last dimension only)
- [x] `pad` (special case only)
- [ ] `concat`
- [ ] `softplus`
- [x] `permute` (special case only)
- [ ] `search_sorted` (composite operation)
- [ ] Elementwise addition.
- [ ] Elementwise multiplication.
- [ ] Elemenwise subtraction.
