#include "circuit.hpp"

qs::Circuit::Circuit(std::vector<Ket> &qubits, int n_bits) {
    if (qubits.size() == 0) {
        std::cerr << "_add: vector dimension mismatch" << std::endl;
        exit(1);
    }

    this->compiled = false;

    this->n_qubits = qubits.size();
    this->qubits = qubits;

    this->n_bits = n_bits;
    this->measurement_mapping = std::vector<int>(this->n_qubits, -1);
}

qs::Circuit::Circuit(int n_qubits, int n_bits, BasicQubits basis) {
    this->compiled = false;

    this->n_qubits = n_qubits;
    this->qubits = std::vector<qs::Ket>(n_qubits, qs::Ket(basis));

    this->n_bits = n_bits;
    this->measurement_mapping = std::vector<int>(this->n_qubits, -1);
}

void qs::Circuit::gate(qs::Unitary gate, int qubit) {
    if (qubit >= this->n_qubits || qubit < 0) {
        std::cerr << "gate: qubit index out of range [0," << this->n_qubits - 1 << "]" << std::endl;
        exit(1);
    }

    if (this->measurement_mapping[qubit] != -1) {
        std::cerr << "gate: qubit is already measured" << std::endl;
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

        if (this->measurement_mapping[qubit] != -1) {
            std::cerr << "gate: qubit is already measured" << std::endl;
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
    for (int qubit = 0; qubit < this->n_qubits; ++qubit) {
        if (this->measurement_mapping[qubit] != -1) {
            std::cerr << "gate: qubit is already measured" << std::endl;
            exit(1);
        }
    }

    std::vector<qs::Unitary> sub_gates = std::vector<qs::Unitary>(this->n_qubits, gate);
    this->gates.push_back(qs::tensor_reduce(sub_gates));
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

    if (this->measurement_mapping[control] != -1) {
        std::cerr << "cgate: control qubit is already measured" << std::endl;
        exit(1);
    }

    if (this->measurement_mapping[target] != -1) {
        std::cerr << "cgate: target qubit is already measured" << std::endl;
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
    qs::Unitary result = inactive + active;
    result.label = "C" + gate.label + "[" + std::to_string(control) + "," + std::to_string(target) + "]";
    this->gates.push_back(result);
}

void qs::Circuit::measure(int qubit, int bit) {
    if (qubit >= this->n_qubits || qubit < 0) {
        std::cerr << "measure: qubit index out of range [0," << this->n_qubits - 1 << "]" << std::endl;
        exit(1);
    }

    if (bit >= this->n_bits || bit < 0) {
        std::cerr << "measure: bit index out of range [0," << this->n_bits - 1 << "]" << std::endl;
        exit(1);
    }

    // check if the classical bit was already assigned
    for (int i = 0; i < this->n_qubits; ++i) {
        if (this->measurement_mapping[i] == bit) {
            std::cerr << "measure: classical bit is already assigned to another qubit" << std::endl;
            exit(1);
        }
    }

    this->measurement_mapping[qubit] = bit;
}

void qs::Circuit::compile() {
    if (this->compiled) {
        return;
    }

    int n = this->gates.size();

    if (n == 0) {
        std::cerr << "compile: no gates to compile" << std::endl;
        exit(1);
    }

    // reduce qubits into one qubit
    this->full_qubit = qs::tensor_reduce(this->qubits);

    // we need to reverse the order
    // |x>-X-Y-Z => ZYX|x>
    this->full_gate = this->gates[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        this->full_gate = this->full_gate % this->gates[i];
    }

    bool all_classical_used = true;
    for (int bit = 0; bit < this->n_bits; ++bit) {
        bool found = false;
        for (int qubit = 0; qubit < this->n_qubits; ++qubit) {
            if (this->measurement_mapping[qubit] == bit) {
                found = true;
                break;
            }
        }
        if (!found) {
            all_classical_used = false;
            break;
        }
    }

    if (!all_classical_used) {
        std::cerr << "compile: not all classical bits are used, either remove the extra ones or use them for some qubit" << std::endl;
        exit(1);
    }

    this->measured_qubits.resize(0);
    this->nonmeasured_qubits.resize(0);
    for (int qubit = 0; qubit < this->n_qubits; ++qubit) {
        if (this->measurement_mapping[qubit] == -1) {
            this->nonmeasured_qubits.push_back(qubit);
        } else {
            this->measured_qubits.push_back(qubit);
        }
    }

    this->compiled = true;
}

void qs::Circuit::_generate_projections(std::vector<std::vector<qs::BasicQubits>> &projections, std::vector<qs::BasicQubits> &basic_qubits, std::vector<qs::BasicQubits> basis) {
    if (basis.size() == n_qubits) {
        projections.push_back(basis);
        return;
    }

    for (qs::BasicQubits &basic_qubit : basic_qubits) {
        basis.push_back(basic_qubit);
        qs::Circuit::_generate_projections(projections, basic_qubits, basis);
        basis.pop_back();
    }
}

qs::Results qs::Circuit::run(int shots) {
    if (!this->compiled) {
        std::cerr << "run: circuit was not compiled" << std::endl;
        exit(1);
    }

    qs::Ket ket_res = this->full_gate * this->full_qubit;
    qs::Bra bra_res = ket_res.conjugate();

    qs::Results results;

    // generate all projections
    std::vector<qs::BasicQubits> basic_qubits = {qs::BasicQubits::ZERO, qs::BasicQubits::ONE};
    std::vector<std::vector<qs::BasicQubits>> all_projection_bases;
    qs::Circuit::_generate_projections(all_projection_bases, basic_qubits);

    // for every projection basis
    for (std::vector<qs::BasicQubits> &projection_basis : all_projection_bases) {
        // compute projection matrix
        qs::Unitary proj = qs::Proj(projection_basis);
        // compute probability of measurement
        double p = (bra_res * proj * ket_res).magnitude();

        // determine bits of measured qubit
        std::string bits = qs::Outcome::get_bits(projection_basis, this->measured_qubits, this->measurement_mapping);

        std::cout << bits << " " << p << std::endl;

        results.add_outcome(bits, p);
    }

    return results;
}

void qs::Circuit::display() {
    if (!this->compiled) {
        std::cout << "Circuit is not compiled" << std::endl;
        std::cout << "Qubits:" << std::endl;
        for (qs::Ket &qubit : this->qubits) {
            qubit.symbol();
            std::cout << " = ";
            qubit.vector();
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Gates:" << std::endl;
        for (qs::Unitary &gate : this->gates) {
            gate.symbol();
            std::cout << std::endl;
        }
        std::cout << std::endl;
    } else {
        std::cout << "Circuit is compiled" << std::endl;
        std::cout << "Qubit: ";
        this->full_qubit.symbol();
        std::cout << " = ";
        this->full_qubit.vector();
        std::cout << std::endl;
        std::cout << "Full gate:" << std::endl;
        this->full_gate.matrix();
        std::cout << std::endl;
    }
}

std::string qs::Outcome::get_bits(std::vector<qs::BasicQubits> &basis, std::vector<int> &measured_qubits, std::vector<int> &measurement_mapping) {
    std::string bits(measured_qubits.size(), '-');
    for (int m : measured_qubits) {
        bits[measurement_mapping[m]] = basis[m];
    }
    return bits;
}

void qs::Outcome::add_p(double p) {
    this->p += p;
}

void qs::Outcome::display() {
    std::cout << "[p=" << this->p << "] : " << this->bits;
}

void qs::Results::add_outcome(std::string &bits, double p) {
    if (this->outcomes.find(bits) == this->outcomes.end()) {
        this->outcomes[bits] = Outcome(bits);
    }
    this->outcomes[bits].add_p(p);
}

void qs::Results::display() {
    std::cout << "Outcomes with probability:" << std::endl;
    for (const std::pair<const std::string, qs::Outcome> &key_val : this->outcomes) {
        qs::Outcome outcome = key_val.second;
        outcome.display();
        std::cout << std::endl;
    }
}
