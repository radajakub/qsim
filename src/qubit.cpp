#include "qubit.hpp"

qs::Qubit::Qubit(qs::BasicQubits basis) {
    this->dim = 2;
    this->coefficient = qs::Complex(1);
    this->items = std::vector<Complex>(2);
    std::string label(1, static_cast<char>(basis));
    this->label = std::string(label);
    switch (basis) {
    case qs::BasicQubits::ZERO:
        this->items[0] = qs::Complex(1);
        break;
    case qs::BasicQubits::ONE:
        this->items[1] = qs::Complex(1);
        break;
    case qs::BasicQubits::PLUS:
        this->coefficient = Complex(1 / sqrt(2));
        this->items[0] = Complex(1);
        this->items[1] = Complex(1);
        break;
    case qs::BasicQubits::MINUS:
        this->coefficient = Complex(1 / sqrt(2));
        this->items[0] = Complex(1);
        this->items[1] = Complex(-1);
        break;
    default:
        std::cout << "Unsupported basis" << std::endl;
        exit(1);
    }
}

std::vector<qs::Complex> qs::Qubit::_tensor_vector(std::vector<qs::Complex> v1, std::vector<qs::Complex> v2) {
    int n = v1.size();
    int m = v2.size();
    std::vector<qs::Complex> result(n * m);
    int idx = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            result[idx++] = v1[i] * v2[j];
        }
    }
    return result;
}

void qs::Ket::symbol() {
    std::cout << "|" << this->label << ">";
}

void qs::Ket::vector() {
    std::cout << this->coefficient << " * [";
    for (int i = 0; i < this->dim; ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << this->items[i];
    }
    std::cout << "]^T";
}

qs::Ket qs::Ket::tensor(Ket &other) {
    int new_dim = this->dim * other.dim;
    std::vector<qs::Complex> new_items = qs::Qubit::_tensor_vector(this->items, other.items);
    std::string new_label = this->label + other.label;
    return qs::Ket(new_dim, this->coefficient * other.coefficient, new_items, new_label);
}

void qs::Bra::symbol() {
    std::cout << "<" << this->label << "|";
}

void qs::Bra::vector() {
    std::cout << this->coefficient << " * [";
    for (int i = 0; i < this->dim; ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << this->items[i];
    }
    std::cout << "]";
}

qs::Bra qs::Bra::tensor(Bra &other) {
    int new_dim = this->dim * other.dim;
    std::vector<qs::Complex> new_items = qs::Qubit::_tensor_vector(this->items, other.items);
    std::string new_label = this->label + other.label;
    return qs::Bra(new_dim, this->coefficient * other.coefficient, new_items, new_label);
}

qs::Complex qs::dot(Bra &bra, Ket &ket) {
    if (bra.dim != ket.dim) {
        std::cout << "Bra and Ket dimensions do not match" << std::endl;
        exit(1);
    }

    qs::Complex result;
    for (int i = 0; i < bra.dim; ++i) {
        qs::Complex item = bra.items[i] * ket.items[i];
        result += item;
    }
    return bra.coefficient * ket.coefficient * result;
}
