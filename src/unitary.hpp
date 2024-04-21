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
    protected:
        static std::vector<std::vector<Complex>> _dagger_matrix(std::vector<std::vector<Complex>> &items);
        static std::string _dagger_label(std::string &label);

    public:
        int dim;
        Complex coefficient;
        std::vector<std::vector<Complex>> items;
        std::string label;

        Unitary(int dim, Complex coefficient, std::vector<std::vector<Complex>> items, std::string label) : dim(dim), coefficient(coefficient), items(items), label(label){};

        Unitary dagger();

        virtual void symbol(){};
        virtual void matrix(){};
    };

    class Hadamard : public Unitary {
    protected:
        Hadamard(int dim, Complex coefficient, std::vector<std::vector<Complex>> items, std::string label) : Unitary(dim, coefficient, items, label){};

    public:
        Hadamard() : Unitary(2, Complex(1 / sqrt(2)), {{Complex(1), Complex(1)}, {Complex(1), Complex(-1)}}, std::string("H")){};

        Hadamard dagger();

        void symbol() override;
        void matrix() override;
    };

};

#endif
