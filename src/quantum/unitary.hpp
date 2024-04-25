#ifndef __UNITARY_HPP__
#define __UNITARY_HPP__

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "../lib/complex.hpp"
#include "../lib/vec_op.hpp"
#include "./basis.hpp"
#include "./qubit.hpp"

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
        Unitary() : Unitary(2, c_mat(2, c_vec(2)), std::string("0")){};

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

    Unitary tensor_reduce(std::vector<Unitary>& gates);

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

    class Proj : public Unitary {
    public:
        // creates a projection operator to a single qubit basis, i.e. |0><0| or |1><1|
        Proj(qs::BasicQubits basis) : Proj(std::vector<qs::BasicQubits>{basis}){};
        // creates a projection operator to multiple qubit basis, i.e. |00><00| or |11><11| or more
        Proj(std::vector<qs::BasicQubits> bases);
    };

};

#endif
