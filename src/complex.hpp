#ifndef __COMPLEX_HPP__
#define __COMPLEX_HPP__

#include <iostream>
#include <sstream>

namespace qs {

    class Complex {
    public:
        double Re;
        double Im;

        Complex(double Re, double Im) : Re(Re), Im(Im) {}
        Complex(double Re) : Re(Re), Im(0) {}
        Complex() : Re(0), Im(0) {}

        Complex operator+(Complex& other);
        Complex operator-(Complex& other);
        Complex operator*(Complex& other);
        Complex& operator+=(Complex& other);
        Complex& operator-=(Complex& other);
        Complex& operator*=(Complex& other);

        Complex conjugate();

        double magnitude();

        std::string str();
    };
};

#endif
