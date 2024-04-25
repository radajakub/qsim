#ifndef __VEC_OP_HPP__
#define __VEC_OP_HPP__

#include <iomanip>
#include <iostream>
#include <vector>

#include "../utils/err.hpp"
#include "./complex.hpp"

// NOTE: consider only square matrices as quantum linear operators are square in our case
namespace qs {

    // shorthand types for complex vector and matrix
    typedef std::vector<Complex> c_vec;
    typedef std::vector<std::vector<Complex>> c_mat;

    // basic operations on complex vectors
    c_vec _add(c_vec& a, c_vec& b);
    c_vec _sub(c_vec& a, c_vec& b);
    c_vec _mul(Complex& c, c_vec& a);
    // compute complex conjugate of vector element-wise
    c_vec _conjugate(c_vec& a);

    // basic operations on complex matrices
    c_mat _add(c_mat& a, c_mat& b);
    c_mat _sub(c_mat& a, c_mat& b);
    c_mat _mul(Complex& c, c_mat& a);
    // compute transpose and conjugate of a complex matrix
    c_mat _dagger(c_mat& a);

    // compute Mx in complex numbers
    c_vec _matvecmul(c_mat& m, c_vec& x);
    // compute x^TM in complex numbers
    c_vec _vecmatmul(c_vec& m, c_mat& x);
    // compute AB in complex numbers
    c_mat _matmul(c_mat& a, c_mat& b);

    // compute inner product of two complex vectors a^Tb
    Complex _inner(c_vec& a, c_vec& b);
    // compute outer product of two complex vectors ab^T
    c_mat _outer(c_vec& a, c_vec& b);

    // tensor product of two complex vectors
    c_vec _tensor(c_vec& a, c_vec& b);
    // tensor product of two complex matrices
    c_mat _tensor(c_mat& a, c_mat& b);

    void print_vec(c_vec& a, bool transpose = false);
    void print_mat(c_mat& a);
};

#endif
