#include <cstring>
#include <iostream>

#include "./quantum/circuit.hpp"
#include "./quantum/unitary.hpp"
#include "./utils/err.hpp"

int main(int argc, char* argv[]) {
    qs::check_err(argc % 2 == 0, "main", "Invalid number of arguments");
    qs::check_err(argc == 1, "main", "Invalid number of arguments");

    std::string s;
    int shots = 1024;
    bool verbose = false;

    // parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--s") == 0) {
            s = argv[++i];
        } else if (std::strcmp(argv[i], "--verbose") == 0) {
            verbose = std::stoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--shots") == 0) {
            shots = std::stoi(argv[++i]);
        } else {
            qs::check_err(true, "main", std::string("Invalid argument: ") + std::string(argv[i]));
        }
    }

    // compute length of the string s
    int n = s.size();

    // initialize 2n qubits to |0>
    qs::QuantumCircuit circuit(2 * n, n, qs::BasicQubits::ZERO);

    circuit.barrier();

    // apply Hadamard gate to the first n qubits
    std::vector<int> first_n(n);
    std::iota(first_n.begin(), first_n.end(), 0);
    circuit.gate(qs::Hadamard(), first_n);

    circuit.barrier();

    // apply oracle
    qs::SimonOracle oracle(s);
    oracle.show();
    circuit.oracle(oracle);

    circuit.barrier();

    // apply Hadamard transform to the first n qubits
    circuit.gate(qs::Hadamard(), first_n);

    circuit.barrier();

    // measure first n qubits
    for (int i = 0; i < n; ++i) {
        circuit.measure(i, i);
    }

    // compile the circuit
    circuit.compile();

    // run the circuit
    qs::Results results = circuit.run(shots, verbose);
    results.show_outcomes();
    results.show_counts();

    return 0;
}
