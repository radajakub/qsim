#ifndef __UNITARY_HPP__
#define __UNITARY_HPP__

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "complex.hpp"
#include "qubit.hpp"
#include "vec_op.hpp"

namespace qs {

    class Bra;
    class Ket;

    class Unitary {
    public:
        int dim;
        c_mat items;
        std::string label;

        Unitary(int dim, Complex coefficient, c_mat items, std::string label);
        Unitary(int dim, c_mat items, std::string label) : Unitary(dim, Complex(1), items, label){};

        // complex conjugate and transposition
        Unitary operator~();

        // perform tensor product for two unitary matrices
        Unitary operator*(Unitary& other);

        // matrix multiplication
        Unitary operator%(Unitary& other);

        // addition of two matrices
        Unitary operator+(Unitary& other);
        // difference of two matrices
        Unitary operator-(Unitary& other);
        // scaling by a constant
        Unitary operator*(Complex& c);

        // apply this operator to a ket to produce a new ket
        Ket operator*(Ket& other);

        void symbol();
        void matrix();
    };

    class Hadamard : public Unitary {
    public:
        Hadamard() : Unitary(2, Complex(1 / sqrt(2)), {{Complex(1), Complex(1)}, {Complex(1), Complex(-1)}}, std::string("H")){};
    };

    class Identity : public Unitary {
    public:
        Identity() : Unitary(2, {{Complex(1), Complex(0)}, {Complex(0), Complex(1)}}, std::string("I")) {}
    };

    class PauliX : public Unitary {
    public:
        PauliX() : Unitary(2, {{Complex(0), Complex(1)}, {Complex(1), Complex(0)}}, std::string("X")) {}
    };

    class PauliY : public Unitary {
    public:
        PauliY() : Unitary(2, {{Complex(0), Complex(0, -1)}, {Complex(0, 1), Complex(0)}}, std::string("Y")) {}
    };

    class PauliZ : public Unitary {
    public:
        PauliZ() : Unitary(2, {{Complex(1), Complex(0)}, {Complex(0), Complex(-1)}}, std::string("Z")) {}
    };

    class CNOT : public Unitary {
    public:
        CNOT() : Unitary(4, {{Complex(1), Complex(0), Complex(0), Complex(0)}, {Complex(0), Complex(1), Complex(0), Complex(0)}, {Complex(0), Complex(0), Complex(0), Complex(1)}, {Complex(0), Complex(0), Complex(1), Complex(0)}}, std::string("CX")) {}
    };

};

#endif
