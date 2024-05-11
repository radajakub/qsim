#ifndef __GEM_HPP__
#define __GEM_HPP__

#include <iostream>
#include <string>
#include <vector>

namespace qs {
    typedef std::vector<int> b_vec;
    typedef std::vector<std::vector<int>> b_mat;

    b_mat _augmented_matrix(std::vector<std::string> &outcomes, int m, int n);

    void _row_echelon(b_mat &mat, int m, int n);

    std::vector<b_vec> _extract(b_mat &mat, int m, int n);

    void print_vec(b_vec &a);
    void print_mat(b_mat &a);
};

#endif
