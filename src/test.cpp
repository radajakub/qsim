#include <iostream>

#include "complex.hpp"
#include "qubit.hpp"
#include "unitary.hpp"
#include "vec_op.hpp"

void print_complex_operation(qs::Complex &a, qs::Complex &b, qs::Complex &res, std::string str) {
    std::cout << a.str() << " " << str << " " << b.str() << " = " << res.str() << std::endl;
}

void print_complex_operation(qs::Complex &a, qs::Complex &res, std::string str) {
    std::cout << a.str() << " " << str << " = " << res.str() << std::endl;
}

void print_qubit(qs::Qubit &q) {
    q.symbol();
    std::cout << " = ";
    q.vector();
    std::cout << std::endl;
}

void print_unitary(qs::Unitary &u) {
    u.symbol();
    std::cout << " = " << std::endl;
    u.matrix();
}

void test_complex() {
    qs::Complex a(1, 2);
    qs::Complex b(3, 4);
    qs::Complex res;

    res = a + b;
    print_complex_operation(a, b, res, "+");

    res = a - b;
    print_complex_operation(a, b, res, "-");

    res = a * b;
    print_complex_operation(a, b, res, "*");

    res = a.conjugate();
    print_complex_operation(a, res, "conjugate");
}

void test_vec() {
    qs::Complex res;
    qs::c_vec vec_res;
    qs::c_mat mat_res;

    qs::c_vec a = {qs::Complex(1), qs::Complex(0)};
    qs::c_vec b = {qs::Complex(0), qs::Complex(1)};
    qs::c_vec x = {qs::Complex(0, 1), qs::Complex(1, -1)};
    qs::Complex c = {qs::Complex(2)};

    std::cout << "[1, 0] + [0, 1] = ";
    vec_res = qs::_add(a, b);
    qs::print_vec(vec_res);
    std::cout << std::endl;

    std::cout << "[1, 0] - [0, 1] = ";
    vec_res = qs::_sub(a, b);
    qs::print_vec(vec_res);
    std::cout << std::endl;

    std::cout << "[0, 1] - [1, 0] = ";
    vec_res = qs::_sub(b, a);
    qs::print_vec(vec_res);
    std::cout << std::endl;

    std::cout << "2 * [1, 0] = ";
    vec_res = qs::_mul(c, a);
    qs::print_vec(vec_res);
    std::cout << std::endl;

    std::cout << "conjugate([1, 0]) = ";
    vec_res = qs::_conjugate(a);
    qs::print_vec(vec_res);
    std::cout << std::endl;

    std::cout << "conjugate([0+1i, 1-1i]) = ";
    vec_res = qs::_conjugate(x);
    qs::print_vec(vec_res);
    std::cout << std::endl;

    std::cout << "inner([1, 0], [0, 1]) = " << qs::_inner(a, b).str() << std::endl;
    std::cout << "inner([0+1i, 1-1i], [0, 1]) = " << qs::_inner(x, b).str() << std::endl;

    std::cout << "outer([1, 0], [0, 1]) = " << std::endl;
    mat_res = qs::_outer(a, b);
    qs::print_mat(mat_res);
    std::cout << std::endl;

    std::cout << "outer([1, 0], [1, 1]) = " << std::endl;
    mat_res = qs::_outer(a, a);
    qs::print_mat(mat_res);
    std::cout << std::endl;

    std::cout << "tensor([1, 0], [0, 1]) = ";
    vec_res = qs::_tensor(a, b);
    qs::print_vec(vec_res);
    std::cout << std::endl;
}

void test_qubits() {
    qs::Ket q0(qs::BasicQubits::ZERO);
    print_qubit(q0);

    qs::Ket q1(qs::BasicQubits::ONE);
    print_qubit(q1);

    qs::Ket q_minus(qs::BasicQubits::MINUS);
    print_qubit(q_minus);

    qs::Ket q_plus(qs::BasicQubits::PLUS);
    print_qubit(q_plus);

    qs::Ket q00 = q0 * q0;
    print_qubit(q00);

    qs::Ket q10 = q1 * q0;
    print_qubit(q10);

    qs::Ket q01 = q0 * q1;
    print_qubit(q01);

    qs::Ket q11 = q1 * q1;
    print_qubit(q11);

    qs::Ket q110 = q1 * q1 * q0;
    print_qubit(q110);

    qs::Bra b0(qs::BasicQubits::PLUS);
    print_qubit(b0);
    qs::Unitary H = qs::Hadamard();
    print_unitary(H);

    qs::Ket q = H * q0;
    print_qubit(q);

    qs::Bra b = b0 * H;
    print_qubit(b);
}

void test_unitary() {
    qs::Unitary H = qs::Hadamard();
    print_unitary(H);

    qs::Unitary I = qs::Identity();
    print_unitary(I);

    qs::Unitary X = qs::PauliX();
    print_unitary(X);

    qs::Unitary Y = qs::PauliY();
    print_unitary(Y);

    qs::Unitary Z = qs::PauliZ();
    print_unitary(Z);

    qs::Unitary IX = I * X;
    print_unitary(IX);

    qs::Unitary XI = X * I;
    print_unitary(XI);

    qs::Unitary CNOT = qs::CNOT();
    print_unitary(CNOT);
}

int main(int argc, char *argv[]) {
    std::cout << "Test complex library" << std::endl;
    test_complex();
    std::cout << std::endl;

    std::cout << "Test vec_op library" << std::endl;
    test_vec();
    std::cout << std::endl;

    std::cout << "Test Qubits library" << std::endl;
    test_qubits();
    std::cout << std::endl;

    std::cout << "Test Unitary library" << std::endl;
    test_unitary();
    std::cout << std::endl;
    return 0;
}
