#include "sparse_matrix.h"

SparseMatrix::SparseMatrix(fs::path fname) {
    std::ifstream stream(fname);
    int32_t n_rows;
    stream >> n_rows;
    data.resize(n_rows);

    for (int32_t i = 0; i < n_rows; i++) {
        int32_t n_cols;
        stream >> n_cols;
        data[i].reserve(n_cols);
        for (int32_t j = 0; j < n_cols; j++) {
            int32_t index;
            double value;
            stream >> index >> value;
            data[i].push_back({index, value});
        }
    }
}

void SparseMatrix::Print() {
    int32_t n = data.size();
    printf("%d rows:\n", n);
    for (int32_t i = 0; i < n; i++) {
        int32_t element_index = 0;
        for (int32_t j = 0; j < n; j++) {
            if (element_index < data[i].size()) {
                if (data[i][element_index].index == j) {
                    printf("%.2f ", data[i][element_index].value);
                    element_index++;
                } else {
                    printf("%.2f ", 0.);
                }
            } else {
                printf("%.2f ", 0.);
            }
        }
        printf("\n");
    }
}

SparseMatrix SparseMatrix::operator*(const SparseMatrix &m) {
    SparseMatrix result;
    auto n = data.size();
    result.data.resize(n);

    for (int32_t i = 0; i < n; i++) {
        // Scalar multiplication using O(n) memory. Can be further optimized
        std::vector<double> tmp(n, 0);
        for (const auto &element1 : data[i]) {
            for (const auto &element2 : m.data[element1.index]) {
                tmp[element2.index] += element1.value * element2.value;
            }
        }
        // Sparsify the i-th row
        for (int32_t j = 0; j < n; j++) {
            if (tmp[j] != 0) {
                result.data[i].push_back({j, tmp[j]});
            }
        }
    }

    return result;
};

SparseMatrix SparseMatrix::operator^(uint32_t p) {
    // Todo: if power == 0, return identity matrix
    if (p == 1) {
        return *this;
    }

    // Recursion
    SparseMatrix tmp = (*this) ^ (p / 2);
    if (p % 2 == 0) {
        return tmp * tmp;
    } else {
        return tmp * tmp * (*this);
    }
}
