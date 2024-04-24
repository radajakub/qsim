#include <iostream>

#include "circuit.hpp"
#include "const.hpp"
#include "qubit.hpp"

int main(int argc, char* argv[]) {
    qs::Circuit circuit(2);

    circuit.gate(qs::Hadamard(), 0);

    circuit.measure(0, 0);

    circuit.display();

    circuit.compile();

    circuit.display();

    circuit.run(1000);

    return 0;
}
