#ifndef __VEC_OP_HPP__
#define __VEC_OP_HPP__

#include <iomanip>
#include <iostream>
#include <vector>

#include "complex.hpp"

// NOTE: consider only square matrices
namespace qs {

    typedef std::vector<Complex> c_vec;
    typedef std::vector<std::vector<Complex>> c_mat;

    c_vec _add(c_vec& a, c_vec& b);
    c_vec _sub(c_vec& a, c_vec& b);
    c_vec _mul(Complex& c, c_vec& a);
    c_vec _conjugate(c_vec& a);

    c_mat _add(c_mat& a, c_mat& b);
    c_mat _sub(c_mat& a, c_mat& b);
    c_mat _mul(Complex& c, c_mat& a);
    c_mat _dagger(c_mat& a);

    c_vec _matvecmul(c_mat& m, c_vec& x);
    c_vec _vecmatmul(c_vec& m, c_mat& x);
    c_mat _matmul(c_mat& a, c_mat& b);

    Complex _inner(c_vec& a, c_vec& b);
    c_mat _outer(c_vec& a, c_vec& b);

    c_vec _tensor(c_vec& a, c_vec& b);
    c_mat _tensor(c_mat& a, c_mat& b);

    void print_vec(c_vec& a, bool transpose = false);
    void print_mat(c_mat& a);
};

#endif
