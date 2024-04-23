#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <iostream>
#include <string>
#include <vector>

#include "complex.hpp"
#include "unitary.hpp"
#include "vec_op.hpp"

namespace qs {

    enum BasicQubits : char {
        ZERO = '0',
        ONE = '1',
        PLUS = '+',
        MINUS = '-',
    };

    // forward declarations
    class Unitary;
    class Qubit;
    class Ket;
    class Bra;

    class Qubit {
    protected:
        // construct full Qubit
        Qubit(int dim, Complex coefficient, c_vec items, std::string label);
        // construct Qubit with implicit coefficient being 1
        Qubit(int dim, c_vec items, std::string label) : Qubit(dim, Complex(1), items, label){};
        // construct one of predefined qubits from BasicQubits enum
        Qubit(BasicQubits basis, bool ket);

    public:
        // dimension of the qubit, should always be 2^n
        int dim;
        // vector representation of the qubit
        c_vec items;
        // symbol representation of the qubit
        std::string label;

        // manipulate label string
        virtual std::string add_brackets(std::string label);
        std::string strip_brackets();

        // print qubit in symbol representation
        virtual void symbol(){};
        // print qubit in vector representation
        virtual void vector(){};
    };

    class Ket : public Qubit {
    public:
        Ket(int dim, Complex coefficient, c_vec items, std::string label) : Qubit(dim, coefficient, items, label){};
        Ket(int dim, c_vec items, std::string label) : Qubit(dim, items, label){};
        Ket(BasicQubits basis) : Qubit(basis, true){};

        std::string add_brackets(std::string label) override;
        void symbol() override;
        void vector() override;

        // tensor product between this ket and other ket
        Ket operator*(Ket &other);
        // outer product with a bra to construct unitary operator
        Unitary operator*(Bra &other);
    };

    class Bra : public Qubit {
    public:
        Bra(int dim, Complex coefficient, c_vec items, std::string label) : Qubit(dim, coefficient, items, label){};
        Bra(int dim, c_vec items, std::string label) : Qubit(dim, items, label){};
        Bra(BasicQubits basis) : Qubit(basis, false){};

        std::string add_brackets(std::string label) override;
        void symbol() override;
        void vector() override;

        // apply unitary operator to this bra
        Bra operator*(Unitary &other);
        // dot product with a ket
        Complex operator*(Ket &other);
        // tensor product with other bra
        Bra operator*(Bra &other);
    };
};

#endif
