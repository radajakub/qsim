# quantum_sim
Implementation of Quantum Circuits Quantum Simulator in C++ as a University assignment

## Instalation
The project uses CMake as a build tool, thus it might be convenient to install it.
The project is built from scratch with no dependencies, so no further packages are necessary to install.
This is the suggested workflow to build the project:

1. Clone the repository
2. Create a `build` directory on the root level and `cd` into it
3. Run `cmake ..` to generate the build files
4. Run `make` to build all targets or `make <target>` to build a specific target only
5. Run `./<target>` to execute the target

## Usage
One can use the simulator in similar way as is showcased in `src/ghz.cpp` which creates the GHZ state and performs measurements on it.
The API for the circuit is described below.
Note that it is necessary to add a new target to the `CMakeLists.txt` file in order to compile a new created program using the simulator.

## API
First step of simulating a quantum circuit is creating the circuit itself.
This is all handled by the `QuantumCircuit` class, from initializing the qubits, to adding gates to making measurements.

### Qubits
As we all know, there are two types of qubits in quantum mechanics, kets and bras.
In this simulator, we distinguish between them as well and we do so by two classes `Ket` and `Bra`, which both inherit from a abstract parent class `Qubit`.

We can initialize both of them by 4 different methods.
Every one of them is the same for bras and kets, the only difference is the class name:

1. Most flexible initialization is the `Ket(int dim, Complex coefficient, c_vec items, std::string label)`. Here we supply all parameters by hand. The dimension of the qubit, the multiplicative complex coefficient, the vector of complex numbers representing the qubit and the label of the qubit, i.e. `|0>` or `|1>`.
2. We can also omit the coefficient and use just `Ket(int dim, c_vec items, std::string label)`.
3. There is also a shorthand notation `Ket(BasicQubits basis)`, where we just specify one of the well known basis options. For now, the implemented basis options are `BasicQubits::ZERO`, `BasicQubits::ONE`, `BasicQubits::PLUS`, `BasicQubits::MINUS`.
4. The shortest way is to create the `|0>` qubit by just calling `Ket()`.

Note that the type of the qubit really matters, especially in multiplication.
Consider two qubits `Ket x = |0>` and `Bra y = <0|`.
For both of them, when multiplied together, the result is a tensor product, i.e. `x * x = |00>` and `y * y = <00|`.
However, when we multiply one ket and one bra, the results are different.
The multiplication `x * y` is an outer product `|0><0|`, while `y * x` is an inner product `<0|0>`.
Moreover, when multiplied with a unitary operator, the bra has to be on the left side of `*` (`<0|U`) while ket has to be on the right side of `*` (`U|0>`).
Note that we can transform `Bra` to `Ket` and `Ket` to `Bra` by using the `.conjugate()` method.

### Quantum Gates
Unitary gates are realized in a similar way to qubits.
All gates are of type `Unitary` and general gates can be initialized by two options:

1. The most general way is to supply the matrix of the gate directly: `Unitary(int dim, Complex coefficient, c_mat matrix, std::string label)`.
2. Or, we can omit the coefficient by shorter call `Unitary(int dim, c_mat matrix, std::string label)`.

In addition to creating general ways, there are multiple standard `2x2` gates predefined.
There is the `Hadamard()`, `Identity()`, `PauliX()`, `PauliY()`, `PauliZ()` and a special gate `Proj(BasicQubits basis)`.
This special gates serves to create the projection operator `|0><0|` or `|1><1|`.
It can be also called with a vector of basis vectors for projections in multi-qubit systems.

The operations on unitary gates can be confusing, so I will list them here.
Apart from multiplying by `Complex` constant and addition/difference of two unitary operators, there is the *dagger* operation (i.e. complex conjugate transpose) realized by the operator `~U`.
To create a tensor product of two operators `U` and `V`, we use the `W = U * V` operator.
This will increase the dimension of the resulting `W` operator.
To multiply two unitary operators of the same dimension, we use the `W = U % V` operator.
In contrast to tensor product, the dimension of `W` will be the same as the dimension of `U` and `V`.

### Quantum Circuits 
Arguably, the most important part is the `QuantumCircuit` class.
The lifecycle of a quantum circuit consists of initialization, adding quantum gates, setting measurements and running experiments.

#### Initialization
To initialize the quantum circuit, we need to provide the initial qubits and classical bits, which will represent the measured values.
There are multiple ways how to do it:

1. We can supply prepared qubits (kets) directly to the circuit: `QuantumCircuit(std::vector<Ket> &qubits, int n_bits)`. If the `n_bits` parameter is omitted, the number of classical bits will be initialized to the number of qubits supplied by the vector.
2. We can also initialize the circuit with a number of qubits and classical bits: `QuantumCircuit(int n_qubits, int n_bits, BasisQubits basis)`. This will create `n_qubits` qubits in the state specified by the `basis` parameter and `n_bits` classical bits.
If we omit the `basis` parameter, the qubits will be initialized in the `|0>` state.
if we omit the `n_bits` parameter, the number of classical bits will be initialized to `n_qubits`.

#### Adding quantum gates
To add a new gate to the circuit, we use the `gate` or `cgate` methods.
Gate adds a unitary gate that acts on all or on a subset of qubits in the same way.
More specifically:

1. `circuit.gate(Unitary gate)` uses the same gate in parallel on every qubit.
2. `circuit.gate(Unitar gate, std::vector<int> qubits)` uses the same gate in parallel on qubits given by the parameter.
3. `circuit.gate(Unitar gate, int qubit)` uses the gate only on the single given qubit.

We can also add a controlled gate (with single control qubit for now) by the function `curcuit.cgate(Unitary gate, int control, int qubit)`, where `control` and `qubit` must be different.


#### Measurements
After adding all gates, we select which qubits to measure and to which classical bits output the measured values.
This is done simply by method `circuit.measure(int qubit, int bit)`.
Note, that after measuring a qubit, no gate can be added which acts on that qubit.

#### Experiments
After adding all elements to the circuit, we call method `circuit.compile()`.
After that we call a method `circuit.run(int n_shots)` to run the circuit `n_shots` times and collect the statistics and output a `Results` object.
To display the results of measurements to the console, use method `results.show_counts()`.

To see a complete and functioning example, see the file `src/ghz.cpp`, which implements preparation and measurement of the GHZ state.
