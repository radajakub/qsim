#include <iostream>

#include "circuit.hpp"
#include "const.hpp"
#include "qubit.hpp"

int main(int argc, char* argv[]) {
    qs::Circuit circuit(2, qs::BasicQubits::ZERO);

    // circuit.gate(qs::Hadamard(), 0);
    // circuit.gate(qs::PauliX(), 1);
    circuit.cgate(qs::PauliX(), 1, 0);

    circuit.display();

    circuit.compile();

    circuit.display();

    // circuit.run();

    return 0;
}
