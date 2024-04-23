#include <iostream>

#include "circuit.hpp"
#include "const.hpp"
#include "qubit.hpp"

int main(int argc, char* argv[]) {
    qs::Circuit circuit(3, qs::BasicQubits::ZERO);

    // circuit.gate(qs::Hadamard(), 0);
    // circuit.gate(qs::PauliX(), 1);
    circuit.cgate(qs::PauliX(), 2, 0);

    circuit.compile();

    circuit.run();

    circuit.display();

    return 0;
}
