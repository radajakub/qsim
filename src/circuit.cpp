#include "circuit.hpp"

qs::Circuit::Circuit(std::vector<Ket> &qubits, int n_bits) {
    qs::check_err(qubits.size() != n_bits, "circuit", "vector dimension mismatch");

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
    qs::check_range("gate", qubit, this->n_qubits);
    qs::check_err(this->measurement_mapping[qubit] != -1, "gate", "qubit is already measured");

    std::vector<qs::Unitary> sub_gates = std::vector<qs::Unitary>(this->n_qubits, qs::Identity());
    sub_gates[qubit] = gate;
    this->gates.push_back(qs::tensor_reduce(sub_gates));
}

void qs::Circuit::gate(qs::Unitary gate, std::vector<int> qubits) {
    for (int qubit : qubits) {
        qs::check_range("gate", qubit, this->n_qubits);
        qs::check_err(this->measurement_mapping[qubit] != -1, "gate", "qubit is already measured");
    }

    std::vector<qs::Unitary> sub_gates = std::vector<qs::Unitary>(this->n_qubits, qs::Identity());
    for (int qubit : qubits) {
        sub_gates[qubit] = gate;
    }
    this->gates.push_back(qs::tensor_reduce(sub_gates));
}

void qs::Circuit::gate(qs::Unitary gate) {
    for (int qubit = 0; qubit < this->n_qubits; ++qubit) {
        qs::check_err(this->measurement_mapping[qubit] != -1, "gate", "qubit is already measured");
    }

    std::vector<qs::Unitary> sub_gates = std::vector<qs::Unitary>(this->n_qubits, gate);
    this->gates.push_back(qs::tensor_reduce(sub_gates));
}

void qs::Circuit::cgate(qs::Unitary gate, int control, int target) {
    qs::check_range("cgate", control, this->n_qubits);
    qs::check_range("cgate", target, this->n_qubits);
    qs::check_err(this->measurement_mapping[control] != -1, "cgate", "control qubit is already measured");
    qs::check_err(this->measurement_mapping[target] != -1, "cgate", "target qubit is already measured");
    qs::check_err(control == target, "cgate", "control and target qubits are the samed");

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
    qs::check_range("measure [qubit]", qubit, this->n_qubits);
    qs::check_range("measure [bit]", bit, this->n_bits);

    // check if the classical bit was already assigned
    for (int i = 0; i < this->n_qubits; ++i) {
        qs::check_err(this->measurement_mapping[i] == bit, "measure", "classical bit is already assigned to another qubit");
    }

    this->measurement_mapping[qubit] = bit;
}

void qs::Circuit::compile() {
    if (this->compiled) {
        return;
    }

    int n = this->gates.size();

    qs::check_err(n == 0, "compile", "no gates to compile");

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

    qs::check_err(!all_classical_used, "compile", "not all classical bits are used, either remove the extra ones or use them for some qubit");

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
    qs::check_err(!this->compiled, "run", "circuit was not compiled");

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
        // add the outcome or increase its probability
        results.add_outcome(bits, p);
    }

    return results;
}

void qs::Circuit::show() {
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

void qs::Outcome::show() {
    std::cout << this->bits << " [p=" << this->p << "]" << std::endl;
}

void qs::Results::add_outcome(std::string &bits, double p) {
    if (this->outcomes.find(bits) == this->outcomes.end()) {
        this->outcomes[bits] = Outcome(bits);
    }
    this->outcomes[bits].add_p(p);
}

void qs::Results::show() {
    std::cout << "Outcomes with probability:" << std::endl;
    for (const std::pair<const std::string, qs::Outcome> &key_val : this->outcomes) {
        qs::Outcome outcome = key_val.second;
        outcome.show();
    }
}
