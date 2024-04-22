#include <iostream>

#include "complex.hpp"
#include "qubit.hpp"
#include "unitary.hpp"

void test_complex() {
    qs::Complex a(1, 2);
    qs::Complex b(3, 4);
    qs::Complex c = a + b;
    std::cout << c.str() << std::endl;
    c = a - b;
    std::cout << c.str() << std::endl;
    c = a * b;
    std::cout << c.str() << std::endl;
    c += a;
    std::cout << c.str() << std::endl;
    c = c.conjugate();
    std::cout << c.str() << std::endl;
}

void test_qubits() {
    qs::Ket q0(qs::BasicQubits::ZERO);
    q0.symbol();
    std::cout << " = ";
    q0.vector();
    std::cout << std::endl;

    qs::Ket q1(qs::BasicQubits::ONE);
    q1.symbol();
    std::cout << " = ";
    q1.vector();
    std::cout << std::endl;

    qs::Ket q_minus(qs::BasicQubits::MINUS);
    q_minus.symbol();
    std::cout << " = ";
    q_minus.vector();
    std::cout << std::endl;

    qs::Ket q_plus(qs::BasicQubits::PLUS);
    q_plus.symbol();
    std::cout << " = ";
    q_plus.vector();
    std::cout << std::endl;

    qs::Ket q01 = q0.tensor(q1);
    q01.symbol();
    std::cout << " = ";
    q01.vector();
    std::cout << std::endl;

    qs::Ket q10 = q1.tensor(q0);
    q10.symbol();
    std::cout << " = ";
    q10.vector();
    std::cout << std::endl;
}

void test_unitary() {
    qs::Unitary h = qs::Hadamard();
    h.symbol();
    std::cout << " = " << std::endl;
    h.matrix();

    qs::Unitary h_dagger = h.dagger();
    h_dagger.symbol();
    std::cout << " = " << std::endl;
    h_dagger.matrix();

    qs::Unitary x = h + h_dagger;
    x.symbol();
    std::cout << " = " << std::endl;
    x.matrix();

    x = h - h_dagger;
    x.symbol();
    std::cout << " = " << std::endl;
    x.matrix();

    x = h * h_dagger;
    x.symbol();
    std::cout << " = " << std::endl;
    x.matrix();

    qs::Ket q0(qs::BasicQubits::ZERO);
    q0.symbol();
    std::cout << " = ";
    q0.vector();
    std::cout << std::endl;
    qs::Ket q1 = h * q0;
    q1.symbol();
    std::cout << " = ";
    q1.vector();
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
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
