#ifndef __ERR_HPP__
#define __ERR_HPP__

#include <functional>
#include <iostream>

namespace qs {
    void check_err(bool condition, std::string caller, std::string message);
    void check_range(std::string caller, int value, int ub, int lb = 0);
    void check_dims(std::string caller, int a_size, int b_size);
};

#endif
