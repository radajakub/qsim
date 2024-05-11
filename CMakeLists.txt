cmake_minimum_required(VERSION 3.10)
project(qsim)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native")

add_executable(test src/ghz.cpp src/utils/err.cpp src/lib/complex.cpp src/lib/vec_op.cpp src/quantum/circuit.cpp src/quantum/qubit.cpp src/quantum/unitary.cpp)

add_executable(ghz src/ghz.cpp src/utils/err.cpp src/lib/complex.cpp src/lib/vec_op.cpp src/quantum/circuit.cpp src/quantum/qubit.cpp src/quantum/unitary.cpp)
add_executable(deutsch src/deutsch.cpp src/utils/err.cpp src/lib/complex.cpp src/lib/vec_op.cpp src/quantum/circuit.cpp src/quantum/qubit.cpp src/quantum/unitary.cpp)
add_executable(simon src/simon.cpp src/utils/err.cpp src/lib/complex.cpp src/lib/vec_op.cpp src/quantum/circuit.cpp src/quantum/qubit.cpp src/quantum/unitary.cpp src/lib/gem.cpp)

