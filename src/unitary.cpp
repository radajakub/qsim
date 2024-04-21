#include "unitary.hpp"

std::vector<std::vector<qs::Complex>> qs::Unitary::_dagger_matrix(std::vector<std::vector<qs::Complex>> &items) {
    int dim = items.size();
    std::vector<std::vector<Complex>> daggered(dim, std::vector<Complex>(dim, Complex(0)));
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            daggered[j][i] = items[i][j].conjugate();
        }
    }
    return daggered;
}

std::string qs::Unitary::_dagger_label(std::string &label) {
    return "(" + label + ")^+";
}

qs::Unitary qs::Unitary::dagger() {
    return qs::Unitary(this->dim, this->coefficient.conjugate(), qs::Unitary::_dagger_matrix(this->items), qs::Unitary::_dagger_label(this->label));
}

qs::Hadamard qs::Hadamard::dagger() {
    return qs::Hadamard(this->dim, this->coefficient.conjugate(), qs::Unitary::_dagger_matrix(this->items), qs::Unitary::_dagger_label(this->label));
}

void qs::Hadamard::symbol() {
    std::cout << this->label;
}

void qs::Hadamard::matrix() {
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
