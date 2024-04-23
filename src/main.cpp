#include <iostream>

#include "circuit.hpp"
#include "qubit.hpp"

int main(int argc, char* argv[]) {
    qs::Circuit circuit(3);

    circuit.gate(0, qs::Hadamard());

    circuit.compile();

    circuit.run();

    circuit.display();

    return 0;
}
