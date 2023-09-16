#include <filesystem>
#include <iostream>

#include "sparse_matrix.h"

namespace fs = std::filesystem;

auto test_dir = fs::path(__FILE__).parent_path().parent_path() / "data";

double tolerance = 1e-2;

double compare(const SparseMatrix &m1, const SparseMatrix &m2) {
    if (m1.data.size() != m2.data.size()) {
        fprintf(stderr, "ERROR: wrong size %zu (must be %zu)\n", m1.data.size(), m2.data.size());
        return std::numeric_limits<double>::infinity();
    }

    auto n = m1.data.size();
    double err = 0;
    for (int32_t i = 0; i < n; i++) {
        std::vector<double> dense1(n, 0);
        std::vector<double> dense2(n, 0);
        for (const auto &element : m1.data[i]) {
            dense1[element.index] = element.value;
        }
        for (const auto &element : m2.data[i]) {
            dense2[element.index] = element.value;
        }
        for (int32_t j = 0; j < n; j++) {
            err = std::max(err, std::abs(dense1[j] - dense2[j]));
        }
    }
    return err;
}

void check_power_2() {
    for (auto item : fs::directory_iterator(test_dir)) {
        auto fname = item.path();
        if (fname.extension() == fs::path(".in")) {
            std::cout << "Test " << fname.filename() << " ... ";
            SparseMatrix m(fname.c_str());
            auto m_power_2 = m * m;

            SparseMatrix answer(fname.replace_extension(".out-2"));

            auto err = compare(m_power_2, answer);
            if (err < tolerance) {
                std::cout << "success, error: " << err << "\n";
            } else {
                std::cout << "failed, error: " << err << "\n";
            }
        }
    }
}

void check_power_5() {
    for (auto item : fs::directory_iterator(test_dir)) {
        auto fname = item.path();
        if (fname.extension() == fs::path(".in")) {
            std::cout << "Test " << fname.filename() << " ... ";
            SparseMatrix m(fname.c_str());
            auto m_power_p = m ^ 5;

            SparseMatrix answer(fname.replace_extension(".out-5"));

            auto err = compare(m_power_p, answer);
            if (err < tolerance) {
                std::cout << "success, error: " << err << "\n";
            } else {
                std::cout << "failed, error: " << err << "\n";
            }
        }
    }
}

void check_power_999() {
    auto fname = test_dir / "3.in";
    if (fname.extension() == fs::path(".in")) {
        std::cout << "Test " << fname.filename() << " ... ";
        SparseMatrix m(fname.c_str());
        auto m_power_p = m ^ 999;

        SparseMatrix answer(fname.replace_extension(".out-999"));

        auto err = compare(m_power_p, answer);
        if (err < tolerance) {
            std::cout << "success, error: " << err << "\n";
        } else {
            std::cout << "failed, error: " << err << "\n";
        }
    }
}

int main(int argc, char **argv) {
    check_power_2();
    check_power_5();
    check_power_999();
}
