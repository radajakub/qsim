#include <iostream>

#include "circuit.hpp"
#include "const.hpp"
#include "qubit.hpp"

int main(int argc, char* argv[]) {
    qs::Circuit circuit(2);

    // circuit.gate(qs::Hadamard(), 1);
    circuit.gate(qs::PauliX(), 1);
    circuit.cgate(qs::PauliX(), 0, 1);

    circuit.display();

    circuit.compile();

    circuit.display();

    circuit.run(1000);

    return 0;
}
