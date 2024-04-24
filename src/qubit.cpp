#include "qubit.hpp"

qs::Qubit::Qubit(int dim, qs::Complex coefficient, std::vector<qs::Complex> items, std::string label) {
    this->dim = dim;
    this->label = label;
    this->items = items;
    for (Complex &item : this->items) {
        item *= coefficient;
    }
}

qs::Qubit::Qubit(qs::BasicQubits basis, bool ket) {
    dim = 2;
    this->items = std::vector<qs::Complex>(2);
    std::string symbol(1, static_cast<char>(basis));
    this->label = this->add_brackets(symbol);
    if (ket) {
        this->label = "|" + symbol + ">";
    } else {
        this->label = "<" + symbol + "|";
    }
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

std::string qs::Qubit::add_brackets(std::string label) {
    return "|" + label + "|";
}

std::string qs::Qubit::strip_brackets() {
    return this->label.substr(1, this->label.size() - 2);
}

std::string qs::Ket::add_brackets(std::string label) {
    return "|" + label + ">";
}

void qs::Ket::symbol() {
    std::cout << this->label;
}

void qs::Ket::vector() {
    qs::print_vec(this->items, true);
}

qs::Ket qs::Ket::operator*(Ket &other) {
    int new_dim = this->dim * other.dim;
    qs::c_vec new_items = qs::_tensor(this->items, other.items);
    std::string new_label = this->add_brackets(this->strip_brackets() + other.strip_brackets());
    return qs::Ket(new_dim, new_items, new_label);
}

qs::Unitary qs::Ket::operator*(qs::Bra &other) {
    int new_dim = this->dim;
    qs::c_mat new_items = qs::_outer(this->items, other.items);
    std::string new_label = this->label + other.label;
    return qs::Unitary(new_dim, new_items, new_label);
}

qs::Bra qs::Ket::conjugate() {
    qs::c_vec new_items = qs::_conjugate(this->items);
    std::string new_label = qs::Bra::add_brackets(this->strip_brackets());
    return qs::Bra(this->dim, new_items, new_label);
}

std::string qs::Bra::add_brackets(std::string label) {
    return "<" + label + "|";
}

void qs::Bra::symbol() {
    std::cout << this->label;
}

void qs::Bra::vector() {
    qs::print_vec(this->items);
}

qs::Bra qs::Bra::operator*(qs::Unitary &other) {
    qs::c_vec new_items = qs::_vecmatmul(this->items, other.items);
    std::string new_label = this->label + other.label;
    return qs::Bra(this->dim, new_items, new_label);
}

qs::Complex qs::Bra::operator*(qs::Ket &other) {
    return qs::_inner(this->items, other.items);
}

qs::Bra qs::Bra::operator*(qs::Bra &other) {
    int new_dim = this->dim * other.dim;
    qs::c_vec new_items = qs::_tensor(this->items, other.items);
    std::string new_label = qs::Bra::add_brackets(this->strip_brackets() + other.strip_brackets());
    return qs::Bra(new_dim, new_items, new_label);
}

qs::Ket qs::Bra::conjugate() {
    qs::c_vec new_items = qs::_conjugate(this->items);
    std::string new_label = qs::Ket::add_brackets(this->strip_brackets());
    return qs::Ket(this->dim, new_items, new_label);
}

qs::Ket qs::tensor_reduce(std::vector<qs::Ket> &qubits) {
    qs::Ket qubit = qubits[0];
    for (int i = 1; i < qubits.size(); ++i) {
        qubit = qubit * qubits[i];
    }
    return qubit;
}

qs::Bra qs::tensor_reduce(std::vector<qs::Bra> &qubits) {
    qs::Bra qubit = qubits[0];
    for (int i = 1; i < qubits.size(); ++i) {
        qubit = qubit * qubits[i];
    }
    return qubit;
}
