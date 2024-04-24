#ifndef __CIRCUIT_HPP__
#define __CIRCUIT_HPP__

#include <string>
#include <vector>

#include "qubit.hpp"
#include "unitary.hpp"

namespace qs {
    class Circuit;
    class Results;

    class Circuit {
    private:
        int n_qubits;
        int n_bits;
        std::vector<Ket> qubits;
        std::vector<Unitary> gates;
        // map qubit index to measurement bit index
        std::vector<int> measurement_mapping;

        bool compiled;
        Ket full_qubit;
        Unitary full_gate;

    public:
        Circuit(std::vector<Ket> &qubits) : Circuit(qubits, qubits.size()){};
        Circuit(std::vector<Ket> &qubits, int n_bits);
        Circuit(int n_qubits, BasicQubits basis = BasicQubits::ZERO) : Circuit(n_qubits, n_qubits, basis){};
        Circuit(int n_qubits, int n_bits, BasicQubits basis = BasicQubits::ZERO);

        // insert gate for a single qubit
        void gate(Unitary gate, int qubit);
        // insert the same gate for qubits in parallel
        void gate(Unitary gate, std::vector<int> qubits);
        // insert gate for every qubit
        void gate(Unitary gate);
        // insert controlled gate with control and target qubits
        void cgate(Unitary gate, int control, int target);

        void measure(int qubit, int bit);

        // compile the gates into one
        void compile();

        // run the experiment and report results
        Results run(int shots);

        // display the circuit based on if it was compiled or not
        void display();
    };

    class Results {
    public:
        void display();
    };
};

#endif
