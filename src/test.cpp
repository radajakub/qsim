#include <iostream>

#include "complex.hpp"
#include "qubit.hpp"
#include "unitary.hpp"

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

void test_qubits() {
    qs::Ket q0(qs::BasicQubits::ZERO);
    print_qubit(q0);

    qs::Ket q1(qs::BasicQubits::ONE);
    print_qubit(q1);

    qs::Ket q_minus(qs::BasicQubits::MINUS);
    print_qubit(q_minus);

    qs::Ket q_plus(qs::BasicQubits::PLUS);
    print_qubit(q_plus);

    qs::Ket q00 = q0.tensor(q0);
    print_qubit(q00);

    qs::Ket q10 = q1.tensor(q0);
    print_qubit(q10);

    qs::Ket q01 = q0.tensor(q1);
    print_qubit(q01);

    qs::Ket q11 = q1.tensor(q1);
    print_qubit(q11);

    qs::Ket q110 = q1.tensor(q1).tensor(q0);
    print_qubit(q110);
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

    qs::Unitary IX = I.tensor(X);
    print_unitary(IX);

    qs::Unitary XI = X.tensor(I);
    print_unitary(XI);

    qs::Unitary CNOT = qs::CNOT();
    print_unitary(CNOT);
}

int main(int argc, char *argv[]) {
    std::cout << "Test Complex library" << std::endl;
    test_complex();
    std::cout << std::endl;

    std::cout << "Test Qubits library" << std::endl;
    test_qubits();
    std::cout << std::endl;

    std::cout << "Test Unitary library" << std::endl;
    test_unitary();
    std::cout << std::endl;
    return 0;
}
