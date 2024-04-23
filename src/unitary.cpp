#include "unitary.hpp"

qs::Unitary::Unitary(int dim, qs::Complex coefficient, qs::c_mat items, std::string label) {
    this->dim = dim;
    this->items = items;
    this->label = label;
    // multiply only if the coefficient is not 1
    if (coefficient.Re != 1 || coefficient.Im != 0) {
        for (qs::c_vec &row : this->items) {
            for (qs::Complex &item : row) {
                item *= coefficient;
            }
        }
    }
}

qs::Unitary qs::Unitary::operator~() {
    qs::c_mat new_items = qs::_dagger(this->items);
    std::string new_label = "(" + label + ")^+";
    return qs::Unitary(this->dim, new_items, new_label);
}

qs::Unitary qs::Unitary::operator*(qs::Unitary &other) {
    int new_dim = this->dim * other.dim;
    qs::c_mat new_items = qs::_tensor(this->items, other.items);
    std::string new_label = this->label + " ⊗ " + other.label;
    return qs::Unitary(new_dim, new_items, new_label);
}

qs::Unitary qs::Unitary::operator%(qs::Unitary &other) {
    qs::c_mat new_items = qs::_matmul(this->items, other.items);
    std::string new_label = this->label + other.label;
    return qs::Unitary(this->dim, new_items, new_label);
}

qs::Unitary qs::Unitary::operator+(qs::Unitary &other) {
    qs::c_mat new_items = qs::_add(this->items, other.items);
    std::string new_label = "(" + this->label + " + " + other.label + ")";
    return qs::Unitary(this->dim, new_items, new_label);
}

qs::Unitary qs::Unitary::operator-(qs::Unitary &other) {
    qs::c_mat new_items = qs::_sub(this->items, other.items);
    std::string new_label = "(" + this->label + " - " + other.label + ")";
    return qs::Unitary(this->dim, new_items, new_label);
}

qs::Unitary qs::Unitary::operator*(qs::Complex &c) {
    qs::c_mat new_items = qs::_mul(c, this->items);
    std::string new_label = c.str() + this->label;
    return qs::Unitary(this->dim, new_items, new_label);
}

qs::Ket qs::Unitary::operator*(qs::Ket &other) {
    qs::c_vec new_items = qs::_matvecmul(this->items, other.items);
    std::string new_label = this->label + other.label;
    return qs::Ket(this->dim, new_items, new_label);
}

void qs::Unitary::symbol() {
    std::cout << this->label;
}

void qs::Unitary::matrix() {
    qs::print_mat(this->items);
}

qs::Unitary qs::tensor_reduce(std::vector<qs::Unitary> &gates) {
    qs::Unitary res = gates[0];
    for (int i = 1; i < gates.size(); ++i) {
        res = res * gates[i];
    }
    return res;
}
