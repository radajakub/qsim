from qiskit import QuantumCircuit
from qiskit_aer.aerprovider import AerSimulator

if __name__ == "__main__":
    sim = AerSimulator()

    qc = QuantumCircuit(3, 3)

    qc.barrier()

    qc.h(0)
    qc.cx(0, 1)
    qc.cx(1, 2)

    qc.barrier()

    qc.measure([0, 1, 2], [0, 1, 2])

    print(qc)

    job = sim.run(qc)

    result = job.result()
    print(result)
    counts = result.get_counts(qc)
    print(counts)
