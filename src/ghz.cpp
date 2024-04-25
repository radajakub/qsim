#include <iostream>

#include "./quantum/circuit.hpp"
#include "./quantum/unitary.hpp"

int main(int argc, char* argv[]) {
    // initialize qubits to |0>
    qs::Circuit circuit(3, 3, qs::BasicQubits::ZERO);

    // apply Hadamard gate to qubit 0
    circuit.gate(qs::Hadamard(), 0);
    // apply CNOT gate to qubit 1 with control qubit 0
    circuit.cgate(qs::PauliX(), 0, 1);
    // apply CNOT gate to qubit 2 with control qubit 1
    circuit.cgate(qs::PauliX(), 1, 2);

    // measure qubits into corresponding classical bits
    circuit.measure(0, 0);
    circuit.measure(1, 1);
    circuit.measure(2, 2);

    // compile the circuit
    circuit.compile();

    // run the circuit
    qs::Results results = circuit.run(1024);
    results.show_counts();

    return 0;
}
