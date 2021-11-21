#include "complex.h"
#include <cmath>

// set real part of number
void Complex::set_real(int n)
{
    real = n;
}

// set imaginary part
void Complex::set_imaginary(int n)
{
    imaginary = n;
}

// get real part
int Complex::get_real()
{
    return real;
}

// get imaginary part
int Complex::get_imaginary()
{
    return imaginary;
}

// absolute value =
float Complex::abs()
{
    return sqrt(pow(real, 2) + pow(imaginary, 2));
}

// overloading of ==
bool Complex::operator==(const Complex& other) const
{
    return other.real == this->real && other.imaginary == this->imaginary;
}

//
bool Complex::operator!=(const Complex& other) const
{
    return !(*this == other);
}

// complex + complex
Complex Complex::operator+(const Complex& other) const
{
    Complex sum = Complex(real + other.real, imaginary + other.imaginary);
    return sum;
}

// complex - complex
Complex Complex::operator-(const Complex& other) const
{
    Complex diff = Complex(real - other.real, imaginary - other.imaginary);
    return diff;
}

// complex * int
Complex Complex::operator*(const int n) const
{
    Complex mul = Complex(real*n, imaginary*n);
    return mul;
}

// =
Complex& Complex::operator=(const Complex& other){
    real = other.real;
    imaginary = other.imaginary;
    return *this;
}


// cout
std::ostream& operator<<(std::ostream& os, const Complex& obj)
{
    if (obj.imaginary < 0)
        os << obj.real << obj.imaginary << "i";
    else if (obj.imaginary == 0)
        os << obj.real;
    else 
        os << obj.real << " + " << obj.imaginary << "i";
    return os;
}
