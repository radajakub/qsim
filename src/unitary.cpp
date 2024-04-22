#include "unitary.hpp"

qs::Unitary qs::Unitary::dagger() {
    std::vector<std::vector<Complex>> daggered(this->dim, std::vector<Complex>(this->dim, Complex(0)));
    for (int i = 0; i < this->dim; i++) {
        for (int j = 0; j < this->dim; j++) {
            daggered[j][i] = this->items[i][j].conjugate();
        }
    }

    std::string new_label = "(" + label + ")^+";

    return qs::Unitary(this->dim, this->coefficient.conjugate(), daggered, new_label);
}

void qs::Unitary::symbol() {
    std::cout << this->label;
}

void qs::Unitary::matrix() {
    size_t cell_size = 1;
    std::vector<std::vector<std::string>> numbers(this->dim, std::vector<std::string>(this->dim));
    for (int i = 0; i < this->dim; i++) {
        for (int j = 0; j < this->dim; j++) {
            numbers[i][j] = this->items[i][j].str();
            cell_size = std::max(cell_size, numbers[i][j].length());
        }
    }

    std::string coefficient_str = this->coefficient.str();
    std::cout << coefficient_str << " * ";
    size_t padding = coefficient_str.length() + 3;

    for (int r = 0; r < this->dim; r++) {
        if (r != 0) {
            std::cout << std::string(padding, ' ');
        }
        std::cout << "[";
        for (int c = 0; c < this->dim; c++) {
            if (c != 0) {
                std::cout << ", ";
            }
            std::cout << std::setw(cell_size) << numbers[r][c];
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
}
