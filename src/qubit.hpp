#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <iostream>
#include <string>
#include <vector>

#include "complex.hpp"

namespace qs {

    enum BasicQubits : char {
        ZERO = '0',
        ONE = '1',
        PLUS = '+',
        MINUS = '-',
    };

    class Qubit {
    protected:
        // helper function to compute tensor product of two vectors
        static std::vector<Complex> _tensor_vector(std::vector<Complex> v1, std::vector<Complex> v2);

        Qubit(int dim, Complex coefficient, std::vector<Complex> items, std::string label) : dim(dim), coefficient(coefficient), items(items), label(label){};
        // construct one of predefined qubits from BasicQubits enum
        Qubit(BasicQubits basis);
        // constructs qubit from binary representation for a number
        Qubit(int x);

    public:
        // dimension of the qubit, should always be 2^n
        int dim;
        // vector representation of the qubit
        Complex coefficient;
        std::vector<Complex> items;
        // symbol representation of the qubit
        std::string label;

        // print qubit in symbol representation
        virtual void symbol(){};
        // print qubit in vector representation
        virtual void vector(){};
    };

    class Ket : public Qubit {
    public:
        Ket(int dim, Complex coefficient, std::vector<Complex> items, std::string label) : Qubit(dim, coefficient, items, label){};
        Ket(BasicQubits basis) : Qubit(basis){};
        Ket(int x) : Qubit(x){};

        void symbol() override;
        void vector() override;

        // preform tensor product with other ket
        Ket tensor(Ket &other);
    };

    class Bra : public Qubit {
    public:
        Bra(int dim, Complex coefficient, std::vector<Complex> items, std::string label) : Qubit(dim, coefficient, items, label){};
        Bra(BasicQubits basis) : Qubit(basis){};
        Bra(int x) : Qubit(x){};

        void symbol() override;
        void vector() override;

        // preform tensor product with other bra
        Bra tensor(Bra &other);
    };

    Complex dot(Bra &bra, Ket &ket);

};

#endif
