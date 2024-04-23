#ifndef __CIRCUIT_HPP__
#define __CIRCUIT_HPP__

#include <string>
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
        Circuit(int n_qubits, BasicQubits basis = BasicQubits::ZERO);

        // insert gate for a single qubit
        void gate(Unitary gate, int qubit);
        // insert the same gate for qubits in parallel
        void gate(Unitary gate, std::vector<int> qubits);
        // insert gate for every qubit
        void gate(Unitary gate);

        void cgate(Unitary gate, int control, int target);

        // compile the gates into one
        void compile();

        void run();

        // display the circuit based on if it was compiled or not
        void display();
    };
};

#endif
