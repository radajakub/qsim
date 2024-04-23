#include "circuit.hpp"

qs::Circuit::Circuit(std::vector<Ket> &qubits) {
    if (qubits.size() == 0) {
        std::cerr << "_add: vector dimension mismatch" << std::endl;
        exit(1);
    }
    this->n_qubits = qubits.size();
    this->qubit = qs::tensor_reduce(qubits);
}

qs::Circuit::Circuit(int n_qubits, BasicQubits basis) {
    std::vector<qs::Ket> qubits(n_qubits, qs::Ket(basis));
    this->n_qubits = n_qubits;
    this->qubit = qs::tensor_reduce(qubits);
}

void qs::Circuit::gate(qs::Unitary gate, int qubit) {
    if (qubit >= this->n_qubits || qubit < 0) {
        std::cerr << "gate: qubit index out of range [0," << this->n_qubits - 1 << "]" << std::endl;
        exit(1);
    }

    std::vector<qs::Unitary> sub_gates = std::vector<qs::Unitary>(this->n_qubits, qs::Identity());
    sub_gates[qubit] = gate;
    this->gates.push_back(qs::tensor_reduce(sub_gates));
}

void qs::Circuit::gate(qs::Unitary gate, std::vector<int> qubits) {
    for (int qubit : qubits) {
        if (qubit >= this->n_qubits || qubit < 0) {
            std::cerr << "gate: qubit index out of range [0," << this->n_qubits - 1 << "]" << std::endl;
            exit(1);
        }
    }

    std::vector<qs::Unitary> sub_gates = std::vector<qs::Unitary>(this->n_qubits, qs::Identity());
    for (int qubit : qubits) {
        sub_gates[qubit] = gate;
    }
    this->gates.push_back(qs::tensor_reduce(sub_gates));
}

void qs::Circuit::gate(qs::Unitary gate) {
    if (gate.dim != this->qubit.dim) {
        std::cerr << "gate: gate dimension mismatch" << std::endl;
        exit(1);
    }

    this->gates.push_back(gate);
}

void qs::Circuit::cgate(qs::Unitary gate, int control, int target) {
    if (control >= this->n_qubits || control < 0) {
        std::cerr << "cgate: control qubit index out of range [0," << this->n_qubits - 1 << "]" << std::endl;
        exit(1);
    }

    if (target >= this->n_qubits || target < 0) {
        std::cerr << "cgate: target qubit index out of range [0," << this->n_qubits - 1 << "]" << std::endl;
        exit(1);
    }

    if (control == target) {
        std::cerr << "cgate: control and target qubits are the same" << std::endl;
        exit(1);
    }

    qs::Unitary proj0 = qs::Proj(qs::BasicQubits::ZERO);
    qs::Unitary proj1 = qs::Proj(qs::BasicQubits::ONE);
    std::vector<qs::Unitary> inactive_parts;
    std::vector<qs::Unitary> active_parts;
    for (int i = 0; i < this->n_qubits; ++i) {
        if (i == target) {
            active_parts.push_back(gate);
            inactive_parts.push_back(qs::Identity());
        } else if (i == control) {
            active_parts.push_back(proj1);
            inactive_parts.push_back(proj0);
        } else {
            active_parts.push_back(qs::Identity());
            inactive_parts.push_back(qs::Identity());
        }
    }
    qs::Unitary inactive = qs::tensor_reduce(inactive_parts);
    qs::Unitary active = qs::tensor_reduce(active_parts);
    this->gates.push_back(inactive + active);
}

// void qs::Circuit::cgate(qs::Unitary gate, std::vector<int> controls, int target) {
//     qs::Unitary proj0 = qs::Proj(qs::BasicQubits::ZERO);
//     qs::Unitary proj1 = qs::Proj(qs::BasicQubits::ONE);
//     std::vector<qs::Unitary> inactive_parts;
//     std::vector<qs::Unitary> active_parts;

//     std::vector<bool> is_control(this->n_qubits, false);
//     for (int control : controls) {
//         if (control >= this->n_qubits || control < 0) {
//             std::cerr << "cgate: control qubit index out of range [0," << this->n_qubits - 1 << "]" << std::endl;
//             exit(1);
//         }
//         is_control[control] = true;
//     }

//     for (int i = 0; i < this->n_qubits; ++i) {
//         if (i == target) {
//             active_parts.push_back(gate);
//             inactive_parts.push_back(qs::Identity());
//         } else if (is_control[i]) {
//             active_parts.push_back(proj1);
//             inactive_parts.push_back(proj0);
//         } else {
//             active_parts.push_back(qs::Identity());
//             inactive_parts.push_back(qs::Identity());
//         }
//     }
//     qs::Unitary inactive = qs::tensor_reduce(inactive_parts);
//     qs::Unitary active = qs::tensor_reduce(active_parts);
//     this->gates.push_back(inactive + active);
// }

void qs::Circuit::compile() {
    if (this->compiled) {
        return;
    }

    int n = this->gates.size();

    if (n == 0) {
        std::cerr << "compile: no gates to compile" << std::endl;
        exit(1);
    }

    // we need to reverse the order
    // |x>-X-Y-Z => ZYX|x>
    this->full_gate = this->gates[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        this->full_gate = this->full_gate % this->gates[i];
    }

    this->full_gate.symbol();
    std::cout << std::endl;
    this->full_gate.matrix();

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
