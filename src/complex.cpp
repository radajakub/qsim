#include "complex.hpp"

qs::Complex qs::Complex::operator+(qs::Complex& other) {
    return qs::Complex(this->Re + other.Re, this->Im + other.Im);
}

qs::Complex qs::Complex::operator-(qs::Complex& other) {
    return qs::Complex(this->Re - other.Re, this->Im - other.Im);
}

qs::Complex qs::Complex::operator*(qs::Complex& other) {
    return qs::Complex(this->Re * other.Re - this->Im * other.Im, this->Re * other.Im + this->Im * other.Re);
}

qs::Complex& qs::Complex::operator+=(qs::Complex& other) {
    this->Re += other.Re;
    this->Im += other.Im;
    return *this;
}

qs::Complex qs::Complex::conjugate() {
    return qs::Complex(this->Re, -this->Im);
}

std::ostream& qs::operator<<(std::ostream& os, qs::Complex const& complex) {
    os << complex.Re;
    if (complex.Im > 0) {
        os << "+" << complex.Im << "i";
    } else if (complex.Im < 0) {
        os << "-" << -complex.Im << "i";
    }
    return os;
}
