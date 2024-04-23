#ifndef __CIRCUIT_HPP__
#define __CIRCUIT_HPP__

#include <vector>

#include "qubit.hpp"
#include "unitary.hpp"

namespace qs {
    class Circuit {
    private:
        int n_qubits;
        Ket qubit;
        std::vector<Unitary> gates;

        bool compiled;
        Unitary full_gate;

    public:
        Circuit(std::vector<Ket> &qubits);
        Circuit(int n_qubits);

        void gate(int qubit, Unitary gate);

        // compile the gates into one
        void compile();

        void run();

        // display the circuit based on if it was compiled or not
        void display();
    };
};

#endif
