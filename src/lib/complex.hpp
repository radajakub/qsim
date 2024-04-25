#ifndef __COMPLEX_HPP__
#define __COMPLEX_HPP__

#include <iostream>
#include <sstream>

namespace qs {

    // store complex number as pairs z = Re + Im * i
    class Complex {
    public:
        double Re;
        double Im;

        Complex(double Re, double Im) : Re(Re), Im(Im) {}
        Complex(double Re) : Re(Re), Im(0) {}
        Complex() : Re(0), Im(0) {}

        // override basic oparators needed in quantum circuits
        Complex operator+(Complex& other);
        Complex operator-(Complex& other);
        Complex operator*(Complex& other);
        Complex& operator+=(Complex& other);
        Complex& operator-=(Complex& other);
        Complex& operator*=(Complex& other);

        // create a complex conjugate of this complex number
        // z = Re + Im * i => z* = Re - Im * i
        Complex conjugate();

        // compute magnitude of this complex number
        // |z| = sqrt(Re^2 + Im^2)
        double magnitude();

        std::string str();
    };
};

#endif
