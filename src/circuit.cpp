#include "circuit.hpp"

qs::Circuit::Circuit(std::vector<Ket> &qubits) {
    if (qubits.size() == 0) {
        std::cerr << "_add: vector dimension mismatch" << std::endl;
        exit(1);
    }
    this->n_qubits = qubits.size();
    this->qubit = qs::tensor_reduce(qubits);
}

qs::Circuit::Circuit(int n_qubits) {
    std::vector<qs::Ket> qubits(n_qubits, qs::Ket(qs::BasicQubits::ZERO));
    this->n_qubits = n_qubits;
    this->qubit = qs::tensor_reduce(qubits);
}

void qs::Circuit::gate(int qubit, qs::Unitary gate) {
    std::vector<qs::Unitary> sub_gates;
    for (int i = 0; i < this->n_qubits; ++i) {
        if (i == qubit) {
            sub_gates.push_back(gate);
        } else {
            sub_gates.push_back(qs::Identity());
        }
    }
    this->gates.push_back(qs::tensor_reduce(sub_gates));
}

void qs::Circuit::compile() {
    if (this->compiled) {
        return;
    }

    if (this->gates.size() == 0) {
        std::cerr << "compile: no gates to compile" << std::endl;
        exit(1);
    }

    this->full_gate = this->gates[0];
    for (int i = 1; i < this->gates.size(); ++i) {
        this->full_gate = this->full_gate % this->gates[i];
    }

    this->compiled = true;
}

void qs::Circuit::run() {
    if (!this->compiled) {
        std::cerr << "run: circuit was not compiled" << std::endl;
        exit(1);
    }

    qs::Ket res = this->full_gate * this->qubit;
    res.vector();
    std::cout << std::endl;
}

void qs::Circuit::display() {
    std::cout << "Circuit is compiled" << std::endl;
    std::cout << "Qubit: ";
    this->qubit.symbol();
    std::cout << std::endl;
    std::cout << "Gates:" << std::endl;
    for (qs::Unitary &gate : this->gates) {
        gate.symbol();
        std::cout << std::endl;
    }
}
