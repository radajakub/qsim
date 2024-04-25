#include "err.hpp"

void qs::check_err(bool condition, std::string caller, std::string message) {
    if (condition) {
        std::cerr << caller << ": " << message << std::endl;
        exit(1);
    }
}

void qs::check_range(std::string caller, int value, int ub, int lb) {
    check_err(value < lb || value >= ub, caller, "value out of range [" + std::to_string(lb) + "," + std::to_string(ub - 1) + "]");
}

void qs::check_dims(std::string caller, int a_size, int b_size) {
    check_err(a_size != b_size, caller, "dimension mismatch");
}
