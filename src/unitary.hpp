#ifndef __UNITARY_HPP__
#define __UNITARY_HPP__

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "complex.hpp"

namespace qs {

    class Unitary {
    public:
        int dim;
        Complex coefficient;
        std::vector<std::vector<Complex>> items;
        std::string label;

        Unitary(int dim, Complex coefficient, std::vector<std::vector<Complex>> items, std::string label) : dim(dim), coefficient(coefficient), items(items), label(label){};

        Unitary dagger();

        void symbol();
        void matrix();
    };

    class Hadamard : public Unitary {
    protected:
        Hadamard(int dim, Complex coefficient, std::vector<std::vector<Complex>> items, std::string label) : Unitary(dim, coefficient, items, label){};

    public:
        Hadamard() : Unitary(2, Complex(1 / sqrt(2)), {{Complex(1), Complex(1)}, {Complex(1), Complex(-1)}}, std::string("H")){};
    };

};

#endif
