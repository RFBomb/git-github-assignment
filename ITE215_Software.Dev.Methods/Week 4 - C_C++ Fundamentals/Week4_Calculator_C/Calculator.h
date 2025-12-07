//
// Robert Brenckman
// ITE215 - Week 4 - Calculator in C
// 11/16/2025
//
#ifndef CALCULATOR_H
#define CALCULATOR_H

// returns 1 if valid, otherwise returns 0
inline int IsDivisorValid(double b) { return b != 0; }

inline double Add(int a, int b) { return a + b; }
inline double Subtract(int  a, int  b) { return a - b; }
inline double Multiply(int  a, int  b) { return a * b; }
double Divide(int  a, int  b);
double Modulus(int  a, int  b);

// C does not support overloads!
double Add_D(double a, double b);
double Subtract_D(double a, double b);
double Multiply_D(double a, double b);
double Divide_D(double a, double b);
double Modulus_D(double a, double b);
double Round_D(double value, double digits);

#endif