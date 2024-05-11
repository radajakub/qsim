#include <cstring>
#include <iostream>

#include "./quantum/circuit.hpp"
#include "./quantum/unitary.hpp"
#include "./utils/err.hpp"

int main(int argc, char* argv[]) {
    qs::check_err(argc % 2 == 0, "main", "Invalid number of arguments");

    int n = 1;
    int shots = 1024;
    std::string type;
    int output = 0;
    bool verbose = false;

    // parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--type") == 0) {
            type = std::string(argv[++i]);
        } else if (std::strcmp(argv[i], "--n") == 0) {
            n = std::stoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--shots") == 0) {
            shots = std::stoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--output") == 0) {
            qs::check_err(type != "constant", "main", "Output is specified onlyt for constant oracle");
            output = std::stoi(argv[++i]);
            qs::check_err(output != 0 && output != 1, "main", "Invalid output specified. Must be either 0 or 1");
        } else if (std::strcmp(argv[i], "--verbose") == 0) {
            verbose = std::stoi(argv[++i]);
        } else {
            qs::check_err(true, "main", std::string("Invalid argument: ") + std::string(argv[i]));
        }
    }

    // check if type was specified correctly
    qs::check_err(type == "", "main", "Type not specified");
    qs::check_err(type != "balanced" && type != "constant", "main", "Invalid type specified. Must be either 'balanced' or 'constant'");

    // initialize qubits to |0>
    qs::QuantumCircuit circuit(n + 1, n, qs::BasicQubits::ZERO);

    circuit.barrier();

    // set ancilla qubit to |1>
    circuit.gate(qs::PauliX(), n);

    circuit.barrier();

    // apply Hadamard to every qubit
    std::vector<int> set_qubits(n + 1);
    std::iota(set_qubits.begin(), set_qubits.end(), 0);
    circuit.gate(qs::Hadamard(), set_qubits);

    circuit.barrier();

    // add oracle
    if (type == "balanced") {
        qs::BalancedOracle oracle(n + 1);
        circuit.oracle(oracle);
    } else if (type == "constant") {
        qs::ConstantOracle oracle(n + 1, output);
        circuit.oracle(oracle);
    }

    circuit.barrier();

    // apply Hadamard to every qubit except the ancilla because we don't need it anymore
    set_qubits.pop_back();
    circuit.gate(qs::Hadamard(), set_qubits);

    circuit.barrier();

    // measure all qubits except the ancilla
    for (int i = 0; i < n; ++i) {
        circuit.measure(i, i);
    }

    // compile the circuit
    circuit.compile();

    // run the circuit
    qs::Results results = circuit.run(shots, verbose);

    std::cout << std::endl;
    results.show_counts();

    // analyse measurements to see if function is balanced or constant
    std::string constant_outcome(n, '0');
    double constant_prob = results.get_measured_ratio(constant_outcome);
    std::cout << "Function is " << (constant_prob > 0.5 ? "CONSTANT" : "BALANCED") << std::endl;

    return 0;
}
