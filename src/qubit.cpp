#include "qubit.hpp"

qs::Qubit::Qubit(int dim, qs::Complex coefficient, std::vector<qs::Complex> items, std::string label) {
    this->dim = dim;
    this->label = label;
    this->items = items;
    for (Complex &item : this->items) {
        item *= coefficient;
    }
}

qs::Qubit::Qubit(qs::BasicQubits basis) {
    dim = 2;
    this->items = std::vector<qs::Complex>(2);
    this->label = std::string(1, static_cast<char>(basis));
    qs::Complex coefficient(1);
    switch (basis) {
    case qs::BasicQubits::ZERO:
        items[0] = qs::Complex(1);
        break;
    case qs::BasicQubits::ONE:
        items[1] = qs::Complex(1);
        break;
    case qs::BasicQubits::PLUS:
        coefficient = qs::Complex(1 / sqrt(2));
        items[0] = qs::Complex(1);
        items[1] = qs::Complex(1);
        break;
    case qs::BasicQubits::MINUS:
        coefficient = qs::Complex(1 / sqrt(2));
        items[0] = qs::Complex(1);
        items[1] = qs::Complex(-1);
        break;
    default:
        std::cout << "Unsupported basis" << std::endl;
        exit(1);
    }
    this->items[0] *= coefficient;
    this->items[1] *= coefficient;
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
    std::cout << "[";
    for (int i = 0; i < this->dim; ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << this->items[i].str();
    }
    std::cout << "]^T";
}

qs::Ket qs::Ket::tensor(Ket &other) {
    int new_dim = this->dim * other.dim;
    std::vector<qs::Complex> new_items = qs::Qubit::_tensor_vector(this->items, other.items);
    std::string new_label = this->label + other.label;
    return qs::Ket(new_dim, new_items, new_label);
}

void qs::Bra::symbol() {
    std::cout << "<" << this->label << "|";
}

void qs::Bra::vector() {
    std::cout << "[";
    for (int i = 0; i < this->dim; ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << this->items[i].str();
    }
    std::cout << "]";
}

qs::Bra qs::Bra::tensor(Bra &other) {
    int new_dim = this->dim * other.dim;
    std::vector<qs::Complex> new_items = qs::Qubit::_tensor_vector(this->items, other.items);
    std::string new_label = this->label + other.label;
    return qs::Bra(new_dim, new_items, new_label);
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
    return result;
}
