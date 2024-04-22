#include "unitary.hpp"

qs::Unitary::Unitary(int dim, Complex coefficient, std::vector<std::vector<Complex>> items, std::string label) {
    this->dim = dim;
    this->items = items;
    this->label = label;
    // multiply only if the coefficient is not 1
    if (coefficient.Re != 1 || coefficient.Im != 0) {
        for (std::vector<Complex> &row : this->items) {
            for (Complex &item : row) {
                item *= coefficient;
            }
        }
    }
}

qs::Unitary qs::Unitary::dagger() {
    std::vector<std::vector<Complex>> daggered(this->dim, std::vector<Complex>(this->dim, Complex(0)));
    for (int i = 0; i < this->dim; i++) {
        for (int j = 0; j < this->dim; j++) {
            daggered[i][j] = this->items[j][i].conjugate();
        }
    }

    std::string new_label = "(" + label + ")^+";

    return qs::Unitary(this->dim, daggered, new_label);
}

qs::Unitary qs::Unitary::operator+(qs::Unitary &other) {
    if (this->dim != other.dim) {
        throw std::invalid_argument("+: Dimension Mismatch");
    }

    std::vector<std::vector<Complex>> result(this->items);
    for (int i = 0; i < this->dim; i++) {
        for (int j = 0; j < this->dim; j++) {
            result[i][j] += other.items[i][j];
        }
    }

    std::string new_label = "(" + this->label + " + " + other.label + ")";

    return qs::Unitary(this->dim, result, new_label);
}

qs::Unitary qs::Unitary::operator-(qs::Unitary &other) {
    if (this->dim != other.dim) {
        throw std::invalid_argument("-: Dimension Mismatch");
    }

    std::vector<std::vector<Complex>> result(this->items);
    for (int i = 0; i < this->dim; i++) {
        for (int j = 0; j < this->dim; j++) {
            result[i][j] -= other.items[i][j];
        }
    }

    std::string new_label = "(" + this->label + " - " + other.label + ")";

    return qs::Unitary(this->dim, result, new_label);
}

qs::Unitary qs::Unitary::operator*(qs::Unitary &other) {
    if (this->dim != other.dim) {
        throw std::invalid_argument("*: Dimension Mismatch");
    }

    std::vector<std::vector<qs::Complex>> result(this->dim, std::vector<Complex>(this->dim));
    for (int r = 0; r < this->dim; ++r) {
        for (int c = 0; c < this->dim; ++c) {
            for (int i = 0; i < this->dim; ++i) {
                qs::Complex subres = this->items[r][i] * other.items[i][c];
                result[r][c] += subres;
            }
        }
    }

    std::string new_label = this->label + other.label;

    return qs::Unitary(this->dim, result, new_label);
}

qs::Ket qs::Unitary::operator*(qs::Ket &other) {
    if (this->dim != other.dim) {
        throw std::invalid_argument("*: Dimension Mismatch");
    }

    std::vector<qs::Complex> result(this->dim);
    for (int i = 0; i < this->dim; i++) {
        for (int j = 0; j < this->dim; j++) {
            qs::Complex subres = this->items[i][j] * other.items[j];
            result[i] += subres;
        }
    }

    std::string new_label = this->label + other.label;

    return qs::Ket(this->dim, result, label);
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

    for (int r = 0; r < this->dim; r++) {
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
