#ifndef __UNITARY_HPP__
#define __UNITARY_HPP__

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "complex.hpp"
#include "qubit.hpp"

namespace qs {

    class Unitary {
    public:
        int dim;
        std::vector<std::vector<Complex>> items;
        std::string label;

        Unitary(int dim, Complex coefficient, std::vector<std::vector<Complex>> items, std::string label);
        Unitary(int dim, std::vector<std::vector<Complex>> items, std::string label) : Unitary(dim, Complex(1), items, label){};

        Unitary dagger();

        Unitary operator+(Unitary& other);
        Unitary operator-(Unitary& other);
        Unitary operator*(Unitary& other);
        Ket operator*(Ket& other);

        void symbol();
        void matrix();
    };

    class Hadamard : public Unitary {
    public:
        Hadamard() : Unitary(2, Complex(1 / sqrt(2)), {{Complex(1), Complex(1)}, {Complex(1), Complex(-1)}}, std::string("H")){};
    };

};

#endif
