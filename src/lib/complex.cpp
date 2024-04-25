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

qs::Complex& qs::Complex::operator-=(qs::Complex& other) {
    this->Re -= other.Re;
    this->Im -= other.Im;
    return *this;
}

qs::Complex& qs::Complex::operator*=(qs::Complex& other) {
    this->Re = this->Re * other.Re - this->Im * other.Im;
    this->Im = this->Re * other.Im + this->Im * other.Re;
    return *this;
}

qs::Complex qs::Complex::conjugate() {
    return qs::Complex(this->Re, -this->Im);
}

double qs::Complex::magnitude() {
    return sqrt(this->Re * this->Re + this->Im * this->Im);
}

std::string qs::Complex::str() {
    std::stringstream ss;
    ss << this->Re;
    if (this->Im > 0) {
        ss << "+" << this->Im << "i";
    } else if (this->Im < 0) {
        ss << "-" << -this->Im << "i";
    }
    return ss.str();
}
