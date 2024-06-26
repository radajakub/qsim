#ifndef __CIRCUIT_HPP__
#define __CIRCUIT_HPP__

#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "../utils/err.hpp"
#include "./basis.hpp"
#include "./qubit.hpp"
#include "./unitary.hpp"

namespace qs {
    class QuantumCircuit;
    class Results;
    class Outcome;

    class QuantumCircuit {
    protected:
        int n_qubits;
        int n_bits;
        // list of intial qubits in the circuit
        std::vector<Ket> qubits;
        // list of gates to apply on initial qubits
        // are stored already in parallel format, i.e. tensor products
        std::vector<Unitary> gates;
        // map qubit index to measurement bit index
        std::vector<int> measurement_mapping;

        // variables that are filled during compilation
        bool compiled;
        // tensor product of initial qubits
        Ket full_qubit;
        // list of measured qubits
        std::vector<int> measured_qubits;

    public:
        QuantumCircuit(std::vector<Ket> &qubits) : QuantumCircuit(qubits, qubits.size()){};
        QuantumCircuit(std::vector<Ket> &qubits, int n_bits);
        QuantumCircuit(int n_qubits, BasicQubits basis = BasicQubits::ZERO) : QuantumCircuit(n_qubits, n_qubits, basis){};
        QuantumCircuit(int n_qubits, int n_bits, BasicQubits basis = BasicQubits::ZERO);

        // insert barrier which is used for printing intermediate results
        void barrier();

        // insert gate for a single qubit
        void gate(Unitary gate, int qubit);
        // insert the same gate for qubits in parallel
        void gate(Unitary gate, std::vector<int> qubits);
        // insert gate for every qubit
        void gate(Unitary gate);
        // insert controlled gate with control and target qubits
        void cgate(Unitary gate, int control, int target);
        // insert oracle converted into a gate
        void oracle(qs::QuantumCircuit oracle);

        // add measurement of qubit into classical bit
        void measure(int qubit, int bit);

        // recursive function to generate all variations of basis qubits of length n_qubits
        void _generate_projections(std::vector<std::vector<BasicQubits>> &projections, std::vector<qs::BasicQubits> &basic_qubits, std::vector<qs::BasicQubits> basis = {});

        // prepare the initial qubits and gates for the computation
        void compile();

        // convert the circuit into a gate
        Unitary to_gate();

        // run the experiment
        Results run(int shots, bool verbose = false);

        // show the circuit based on if it was compiled or not
        void show();
    };

    class ConstantOracle : public QuantumCircuit {
    public:
        ConstantOracle(int n_qubits, int output);
    };

    class BalancedOracle : public QuantumCircuit {
    public:
        BalancedOracle(int n_qubits);
    };

    // implementation of the oracle for Simon's algorithm (inspiration taken from qiskit article)
    class SimonOracle : public QuantumCircuit {
    public:
        SimonOracle(std::string s);
    };

    class Outcome {
    public:
        Outcome(){};
        Outcome(std::string &bits) : bits(bits), p(0.0){};

        // encode vector of basis qubits into classical bits
        static std::string get_bits(std::vector<BasicQubits> &basis, std::vector<int> &measured_qubits, std::vector<int> &measurement_mapping);

        // outcome is a list of classical bits measured with a probability distribution
        std::string bits;
        // probability of measuring 1
        double p;

        // add measured probability p
        void add_p(double p);

        void show();
    };

    class Results {
    private:
        std::mt19937 rng;
        std::uniform_real_distribution<double> dist;

        std::unordered_map<std::string, Outcome> outcomes;
        std::unordered_map<std::string, int> counts;
        int shots;

    public:
        Results(int shots);

        void add_outcome(std::string &bits, double p);

        std::vector<std::string> get_bits();

        double get_measured_ratio(std::string &bits);

        void run();

        void show_outcomes();
        void show_counts();
    };
};

#endif
