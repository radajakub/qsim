#include <iostream>

#include "circuit.hpp"
#include "const.hpp"
#include "qubit.hpp"

int main(int argc, char* argv[]) {
    qs::Circuit circuit(2, 2);

    circuit.gate(qs::Hadamard(), 0);

    circuit.measure(0, 0);
    circuit.measure(1, 1);

    circuit.show();

    circuit.compile();

    circuit.show();

    qs::Results results = circuit.run(1000);

    results.show();

    return 0;
}
