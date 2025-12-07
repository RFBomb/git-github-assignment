//
// Robert Brenckman
// ITE215 - Week 4 - Calculator in C
// 11/16/2025
//

#include "Calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// these accept doubles instead of int to allow returning of decimal values. Integers are automatically cast to double and vice versa.

double Add_D(double a, double b) { return a + b; }

double Subtract_D(double a, double b) { return a - b; }

double Multiply_D(double a, double b) { return a * b; }

static void ThrowIfDivisorInvalid(double b)
{
    if (b == 0)
    {
        fprintf(stderr, "Error: divisor [%f] must be non-zero\n", b);
        abort();
    }
}

double Divide_D(double a, double b)
{
    ThrowIfDivisorInvalid(b);
     return a / b; 
}

double Modulus_D(double a, double b)
{
    ThrowIfDivisorInvalid(b);
    double dividend = a / b;
    long long rounded = (long long)dividend;
    return a - (rounded * b);
}

double Round_D(double value, double digits)
{
    if (digits >= 1) 
    {
        int numDigits = 10 * digits;
        double x = round(value * numDigits);
        return x / numDigits;
    }
    else
    {
        return round(value);
    }
}

double Divide(int  a, int  b)
{ 
    ThrowIfDivisorInvalid(b);
    return a / b;// this generating a truncation warning - assigment calls for integer math though
} 

double Modulus(int  a, int  b)
{
    ThrowIfDivisorInvalid(b);
    return a % b;// this generating a truncation warning - assigment calls for integer math though
}