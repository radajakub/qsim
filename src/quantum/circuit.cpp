#include "./circuit.hpp"

qs::QuantumCircuit::QuantumCircuit(std::vector<Ket> &qubits, int n_bits) {
    qs::check_err(qubits.size() != n_bits, "circuit", "vector dimension mismatch");

    this->compiled = false;

    this->n_qubits = qubits.size();
    this->qubits = qubits;

    this->n_bits = n_bits;
    this->measurement_mapping = std::vector<int>(this->n_qubits, -1);
}

qs::QuantumCircuit::QuantumCircuit(int n_qubits, int n_bits, BasicQubits basis) {
    this->compiled = false;

    this->n_qubits = n_qubits;
    this->qubits = std::vector<qs::Ket>(n_qubits, qs::Ket(basis));

    this->n_bits = n_bits;
    this->measurement_mapping = std::vector<int>(this->n_qubits, -1);
}

void qs::QuantumCircuit::barrier() {
    this->gates.push_back(qs::Barrier());
}

void qs::QuantumCircuit::gate(qs::Unitary gate, int qubit) {
    qs::check_range("gate", qubit, this->n_qubits);
    qs::check_err(this->measurement_mapping[qubit] != -1, "gate", "qubit is already measured");

    std::vector<qs::Unitary> sub_gates = std::vector<qs::Unitary>(this->n_qubits, qs::Identity());
    sub_gates[qubit] = gate;
    this->gates.push_back(qs::tensor_reduce(sub_gates));
}

void qs::QuantumCircuit::gate(qs::Unitary gate, std::vector<int> qubits) {
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

void qs::QuantumCircuit::gate(qs::Unitary gate) {
    for (int qubit = 0; qubit < this->n_qubits; ++qubit) {
        qs::check_err(this->measurement_mapping[qubit] != -1, "gate", "qubit is already measured");
    }

    std::vector<qs::Unitary> sub_gates = std::vector<qs::Unitary>(this->n_qubits, gate);
    this->gates.push_back(qs::tensor_reduce(sub_gates));
}

void qs::QuantumCircuit::cgate(qs::Unitary gate, int control, int target) {
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

void qs::QuantumCircuit::oracle(qs::QuantumCircuit oracle) {
    qs::check_err(oracle.n_qubits != this->n_qubits, "oracle", "oracle dimension mismatch");

    this->gates.push_back(oracle.to_gate());
}

void qs::QuantumCircuit::measure(int qubit, int bit) {
    qs::check_range("measure [qubit]", qubit, this->n_qubits);
    qs::check_range("measure [bit]", bit, this->n_bits);

    // check if the classical bit was already assigned
    for (int i = 0; i < this->n_qubits; ++i) {
        qs::check_err(this->measurement_mapping[i] == bit, "measure", "classical bit is already assigned to another qubit");
    }

    this->measurement_mapping[qubit] = bit;
}

void qs::QuantumCircuit::compile() {
    if (this->compiled) {
        return;
    }

    int n = this->gates.size();

    qs::check_err(n == 0, "compile", "no gates to compile");

    // reduce qubits into one qubit
    this->full_qubit = qs::tensor_reduce(this->qubits);

    // configure measurements
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
    for (int qubit = 0; qubit < this->n_qubits; ++qubit) {
        if (this->measurement_mapping[qubit] != -1) {
            this->measured_qubits.push_back(qubit);
        }
    }

    this->compiled = true;
}

qs::Unitary qs::QuantumCircuit::to_gate() {
    qs::check_err(this->gates.empty(), "to_gate", "circuit has no gates");

    int n = this->gates.size();

    std::vector<qs::Unitary> sub_gates = std::vector<qs::Unitary>(this->n_qubits, qs::Identity());
    qs::Unitary gate = qs::tensor_reduce(sub_gates);

    for (int i = n - 1; i >= 0; --i) {
        if (this->gates[i].dim != 0) {
            gate = gate % this->gates[i];
        }
    }

    return gate;
}

void qs::QuantumCircuit::_generate_projections(std::vector<std::vector<qs::BasicQubits>> &projections, std::vector<qs::BasicQubits> &basic_qubits, std::vector<qs::BasicQubits> basis) {
    if (basis.size() == n_qubits) {
        projections.push_back(basis);
        return;
    }

    for (qs::BasicQubits &basic_qubit : basic_qubits) {
        basis.push_back(basic_qubit);
        qs::QuantumCircuit::_generate_projections(projections, basic_qubits, basis);
        basis.pop_back();
    }
}

qs::Results qs::QuantumCircuit::run(int shots) {
    qs::check_err(!this->compiled, "run", "circuit was not compiled");

    qs::Ket ket_res = this->full_qubit;
    int n = this->gates.size();

    int k = 0;
    for (qs::Unitary &gate : this->gates) {
        if (gate.dim == 0) {
            std::cout << "(" << k << ") ";
            ket_res.vector();
            std::cout << std::endl;
            continue;
        }
        ket_res = gate * ket_res;
    }

    qs::Bra bra_res = ket_res.conjugate();

    qs::Results results(shots);

    // generate all projections
    std::vector<qs::BasicQubits> basic_qubits = {qs::BasicQubits::ZERO, qs::BasicQubits::ONE};
    std::vector<std::vector<qs::BasicQubits>> all_projection_bases;
    qs::QuantumCircuit::_generate_projections(all_projection_bases, basic_qubits);

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

    // conduct experiment on the outcomes
    results.run();

    return results;
}

void qs::QuantumCircuit::show() {
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
            if (gate.dim == 0) {
                std::cout << "--- barrier ---" << std::endl;
                continue;
            }
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
        std::cout << "Gates:" << std::endl;
        for (qs::Unitary &gate : this->gates) {
            if (gate.dim == 0) {
                std::cout << "--- barrier ---" << std::endl;
                continue;
            }
            gate.symbol();
            std::cout << std::endl;
        }
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

qs::ConstantOracle::ConstantOracle(int n_qubits, int output) : QuantumCircuit(n_qubits) {
    qs::check_err(output != 0 && output != 1, "ConstantOracle", "Invalid output specified. Must be either 0 or 1");

    this->n_qubits = n_qubits;
    this->n_bits = 0;

    this->gate(qs::Identity());

    if (output == 1) {
        this->gate(qs::PauliX(), n_qubits - 1);
    }
}

qs::BalancedOracle::BalancedOracle(int n_qubits) : QuantumCircuit(n_qubits) {
    this->n_qubits = n_qubits;
    this->n_bits = 0;

    for (int i = 0; i < n_qubits - 1; ++i) {
        this->cgate(qs::PauliX(), i, n_qubits - 1);
    }
}

qs::Results::Results(int shots) {
    this->shots = shots;
    std::random_device rd;
    this->rng = std::mt19937(rd());
    this->dist = std::uniform_real_distribution<double>(0, 1);
}

void qs::Results::add_outcome(std::string &bits, double p) {
    // ignore outcomes that have zero probability
    if (p == 0) {
        return;
    }
    // add the outcome or increase its probability
    if (this->outcomes.find(bits) == this->outcomes.end()) {
        this->outcomes[bits] = Outcome(bits);
    }
    this->outcomes[bits].add_p(p);
}

double qs::Results::get_measured_ratio(std::string &bits) {
    // the outcome was not sampled at all
    if (this->counts.find(bits) == this->counts.end()) {
        return 0.0;
    }
    // return the ratio of this bit string
    return (double)this->counts[bits] / this->shots;
}

void qs::Results::run() {
    // reset the counts to zeros for each possible outcome
    this->counts.clear();
    for (const std::pair<const std::string, qs::Outcome> &key_val : this->outcomes) {
        this->counts[key_val.second.bits] = 0;
    }

    // sample shots times an outcome based on its probability
    for (int i = 0; i < this->shots; ++i) {
        double prob = this->dist(this->rng);
        double cum_prob = 0.0;
        for (const std::pair<const std::string, qs::Outcome> &key_val : this->outcomes) {
            qs::Outcome outcome = key_val.second;
            cum_prob += outcome.p;
            if (prob < cum_prob) {
                this->counts[outcome.bits] += 1;
                break;
            }
        }
    }
}

void qs::Results::show_outcomes() {
    std::cout << "Outcomes with probability:" << std::endl;
    for (const std::pair<const std::string, qs::Outcome> &key_val : this->outcomes) {
        qs::Outcome outcome = key_val.second;
        outcome.show();
    }
}

void qs::Results::show_counts() {
    std::cout << "Measurements: " << std::endl;
    int line_width = 100;
    double unit = (double)line_width / this->shots;
    int count;
    int filled;
    int nonfilled;
    double percent;
    std::string meter;
    std::string rest;
    for (const std::pair<const std::string, const int> &key_val : this->counts) {
        std::string bits = key_val.first;
        count = key_val.second;
        filled = floor(unit * count);
        nonfilled = line_width - filled;
        meter = std::string(filled, '#');
        rest = std::string(nonfilled, '.');
        percent = (double)count / this->shots * 100;
        std::cout << bits << " |" << meter << rest << "| " << percent << "% (" << count << "/" << this->shots << ")" << std::endl;
    }
    std::cout << std::endl;
}
