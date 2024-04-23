#include "vec_op.hpp"

qs::c_vec qs::_add(qs::c_vec& a, qs::c_vec& b) {
    if (a.size() != b.size()) {
        std::cerr << "_add: vector dimension mismatch" << std::endl;
        exit(1);
    }

    int dim = a.size();

    qs::c_vec res(a);
    for (int i = 0; i < dim; ++i)
        res[i] += b[i];
    return res;
}

qs::c_vec qs::_sub(qs::c_vec& a, qs::c_vec& b) {
    if (a.size() != b.size()) {
        std::cerr << "_sub: vector dimension mismatch" << std::endl;
        exit(1);
    }

    int dim = a.size();

    qs::c_vec res(a);
    for (int i = 0; i < dim; ++i)
        res[i] -= b[i];
    return res;
}

qs::c_vec qs::_mul(qs::Complex& c, qs::c_vec& a) {
    int dim = a.size();

    qs::c_vec res(a);
    for (int i = 0; i < dim; ++i)
        res[i] *= c;
    return res;
}

qs::c_vec qs::_conjugate(qs::c_vec& a) {
    int dim = a.size();

    qs::c_vec res(dim);
    for (int i = 0; i < dim; ++i)
        res[i] = a[i].conjugate();
    return res;
}

qs::c_mat qs::_add(qs::c_mat& a, qs::c_mat& b) {
    if (a.size() != b.size()) {
        std::cerr << "_add: matrix dimension mismatch" << std::endl;
        exit(1);
    }

    int dim = a.size();

    qs::c_mat res(a);
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j)
            res[i][j] += b[i][j];
    return res;
}

qs::c_mat qs::_sub(qs::c_mat& a, qs::c_mat& b) {
    if (a.size() != b.size()) {
        std::cerr << "_sub: matrix dimension mismatch" << std::endl;
        exit(1);
    }

    int dim = a.size();

    qs::c_mat res(a);
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j)
            res[i][j] -= b[i][j];
    return res;
}

qs::c_mat qs::_mul(qs::Complex& c, qs::c_mat& a) {
    int dim = a.size();

    qs::c_mat res(a);
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j)
            res[i][j] *= c;
    return res;
}

qs::c_mat qs::_dagger(qs::c_mat& a) {
    int dim = a.size();

    qs::c_mat res(dim, qs::c_vec(dim));
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j)
            res[i][j] = a[j][i].conjugate();
    return res;
}

qs::c_vec qs::_matvecmul(qs::c_mat& m, qs::c_vec& x) {
    if (m.size() != x.size()) {
        std::cerr << "_matvecmul: matrix vector dimension mismatch" << std::endl;
        exit(1);
    }

    int dim = m.size();
    qs::c_vec res(dim);
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            qs::Complex term = x[i] * m[i][j];
            res[i] += term;
        }
    }
    return res;
}

qs::c_vec qs::_vecmatmul(qs::c_vec& x, qs::c_mat& m) {
    if (m.size() != x.size()) {
        std::cerr << "_vecmatmul: vector matrix dimension mismatch" << std::endl;
        exit(1);
    }

    int dim = m.size();
    qs::c_vec res(dim);
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            qs::Complex term = x[i] * m[j][i];
            res[i] += term;
        }
    }
    return res;
}

qs::c_mat qs::_matmul(qs::c_mat& a, qs::c_mat& b) {
    if (a.size() != b.size()) {
        std::cerr << "_matmul: matrix dimension mismatch" << std::endl;
        exit(1);
    }

    int dim = a.size();
    qs::c_mat res(dim, qs::c_vec(dim));
    for (int r = 0; r < dim; ++r) {
        for (int c = 0; c < dim; ++c) {
            for (int i = 0; i < dim; ++i) {
                qs::Complex term = a[r][i] * b[i][c];
                res[r][c] += term;
            }
        }
    }
    return res;
}

qs::Complex qs::_inner(c_vec& a, c_vec& b) {
    if (a.size() != b.size()) {
        std::cerr << "_inner: vector dimension mismatch" << std::endl;
        exit(1);
    }

    int dim = a.size();
    qs::Complex res;
    for (int i = 0; i < dim; ++i) {
        qs::Complex term = a[i] * b[i];
        res += term;
    }
    return res;
}

qs::c_mat qs::_outer(c_vec& a, c_vec& b) {
    if (a.size() != b.size()) {
        std::cerr << "_outer: vector dimension mismatch" << std::endl;
        exit(1);
    }

    int dim = a.size();
    qs::c_mat res(dim, qs::c_vec(dim));
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            res[i][j] = a[i] * b[j];
        }
    }
    return res;
}

qs::c_vec qs::_tensor(c_vec& a, c_vec& b) {
    int dim_a = a.size();
    int dim_b = b.size();

    qs::c_vec res(dim_a * dim_b);
    int idx = 0;
    for (int i = 0; i < dim_a; ++i) {
        for (int j = 0; j < dim_b; ++j) {
            res[idx++] = a[i] * b[j];
        }
    }
    return res;
}

qs::c_mat qs::_tensor(c_mat& a, c_mat& b) {
    int dim_a = a.size();
    int dim_b = b.size();
    int dim = dim_a * dim_b;

    qs::c_mat res(dim, qs::c_vec(dim));
    int row_offset;
    int col_offset;
    int row;
    int col;
    for (int r1 = 0; r1 < dim_a; ++r1) {
        row_offset = r1 * dim_b;
        for (int c1 = 0; c1 < dim_a; ++c1) {
            col_offset = c1 * dim_b;
            for (int r2 = 0; r2 < dim_b; ++r2) {
                row = row_offset + r2;
                for (int c2 = 0; c2 < dim_b; ++c2) {
                    col = col_offset + c2;
                    res[row][col] = a[r1][c1] * b[r2][c2];
                }
            }
        }
    }
    return res;
}

void qs::print_vec(c_vec& a, bool transpose) {
    std::cout << "[";
    for (int i = 0; i < a.size(); ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << a[i].str();
    }
    std::cout << "]";
    if (transpose) {
        std::cout << "^T";
    }
}

void qs::print_mat(c_mat& a) {
    int dim = a.size();

    size_t cell_size = 1;
    std::vector<std::vector<std::string>> numbers(dim, std::vector<std::string>(dim));
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            numbers[i][j] = a[i][j].str();
            cell_size = std::max(cell_size, numbers[i][j].length());
        }
    }

    for (int r = 0; r < dim; r++) {
        std::cout << "[";
        for (int c = 0; c < dim; c++) {
            if (c != 0) {
                std::cout << ", ";
            }
            std::cout << std::setw(cell_size) << numbers[r][c];
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
}
