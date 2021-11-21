#pragma once
#include <iostream>

class Complex
{
private:
    int real;
    int imaginary;

public:
    Complex() : real(0), imaginary(0){}; // 0 + 0i = 0
    Complex(int r) : real(r), imaginary(0){}; // r + 0i = r
    Complex(int r, int i) : real(r), imaginary(i){}; // r + i

    Complex(const Complex& other) = default; // copy

    void set_real(int n);
    void set_imaginary(int n);
    int get_real();
    int get_imaginary();
    float abs();

    bool operator==(const Complex& other) const;
    bool operator!=(const Complex& other) const;
    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const int n) const;
    
    Complex& operator=(const Complex& other);
    friend std::ostream& operator<<(std::ostream& os, const Complex& obj);

};