#ifndef __CIRCUIT_HPP__
#define __CIRCUIT_HPP__

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "err.hpp"
#include "qubit.hpp"
#include "unitary.hpp"

namespace qs {
    class Circuit;
    class Results;
    class Outcome;

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
        std::vector<int> measured_qubits;
        std::vector<int> nonmeasured_qubits;

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

        // add measurement of qubit into classical bit
        void measure(int qubit, int bit);

        void _generate_projections(std::vector<std::vector<BasicQubits>> &projections, std::vector<qs::BasicQubits> &basic_qubits, std::vector<qs::BasicQubits> basis = {});

        // compile the gates into one
        void compile();

        // run the experiment and report results
        Results run(int shots);

        // show the circuit based on if it was compiled or not
        void show();
    };

    class Outcome {
    public:
        Outcome(){};
        Outcome(std::string &bits) : bits(bits), p(0.0){};

        static std::string get_bits(std::vector<BasicQubits> &basis, std::vector<int> &measured_qubits, std::vector<int> &measurement_mapping);

        // outcome is a list of classical bits measured with a probability distribution
        std::string bits;
        // probability of measuring 1
        double p;

        void add_p(double p);

        void show();
    };

    class Results {
    private:
        std::unordered_map<std::string, Outcome> outcomes;

    public:
        Results(){};

        void add_outcome(std::string &bits, double p);

        void show();
    };
};

#endif
