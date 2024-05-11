#include "gem.hpp"

qs::b_mat qs::_augmented_matrix(std::vector<std::string>& outcomes, int m, int n) {
    b_mat mat(m, b_vec(n + m, 0));

    // fill ones from outcomes
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            mat[i][j] = (outcomes[j][i] == '1' ? 1 : 0);
        }
    }

    // fill identity matrix
    for (int i = 0; i < m; ++i) {
        mat[i][n + i] = 1;
    }

    return mat;
}

void qs::_row_echelon(qs::b_mat& mat, int m, int n) {
    int h = 0;  // pivot row
    int k = 0;  // pivot column

    while (h < m && k < n) {
        // find k-th pivot (we have a binary matrix -> just first 1)
        int i_max = h;
        for (int i = h + 1; i < m; ++i) {
            if (mat[i][k] == 1) {
                i_max = i;
                break;
            }
        }

        if (mat[i_max][k] == 0) {
            ++k;
        } else {
            // swap rows h and i_max
            std::swap(mat[h], mat[i_max]);
            for (int i = h + 1; i < m; ++i) {
                int f = mat[i][k] / mat[h][k];
                mat[i][k] = 0;
                for (int j = k + 1; j < m + n; ++j) {
                    mat[i][j] -= f * mat[h][j];
                }
            }
            ++k;
            ++h;
        }
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m + n; ++j) {
            mat[i][j] = (mat[i][j] % 2 + 2) % 2;
        }
    }
}

std::vector<qs::b_vec> qs::_extract(b_mat& mat, int m, int n) {
    std::vector<b_vec> s;

    // find solutions for every zero row in the matrix
    for (int i = 0; i < m; ++i) {
        bool zero = true;
        for (int j = 0; j < n; ++j) {
            if (mat[i][j] == 1) {
                zero = false;
                break;
            }
        }
        if (zero) {
            b_vec v(m, -1);
            for (int j = 0; j < m; ++j) {
                v[j] = mat[i][n + j];
            }
            s.push_back(v);
        }
    }

    // if no solution was found, add the trivial solution
    if (s.empty()) {
        s.push_back(b_vec(m, 0));
    }

    return s;
}

void qs::print_vec(qs::b_vec& a) {
    std::cout << "[";
    for (int i = 0; i < a.size(); ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << a[i];
    }
    std::cout << "]";
}

void qs::print_mat(qs::b_mat& a) {
    for (int r = 0; r < a.size(); r++) {
        std::cout << "[";
        for (int c = 0; c < a[r].size(); c++) {
            if (c != 0) {
                std::cout << ", ";
            }
            std::cout << a[r][c];
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
}
