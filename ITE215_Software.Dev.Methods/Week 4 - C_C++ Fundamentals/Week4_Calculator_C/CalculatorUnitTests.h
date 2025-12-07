//
// Robert Brenckman
// ITE215 - Week 4 - Calculator in C
// 11/16/2025
//

#pragma once
#ifndef CalcUnitTests
#define CalcUnitTests

#include "Calculator.h"
#include <stdlib.h>
#include <stdio.h>

inline void Test_IsDivisorValid()
{
    
    int expected = 0;
    int loops = 0;
    double value = 0;
    int error = 0;
    while (loops++ < 3)
    {
        int isDivValud = IsDivisorValid(value);
        if (isDivValud != expected)
        {
            fprintf(stderr, "Error: IsDivisorValid(%.3f) returned unexpected result. Expected : [%d]     Received: [%d]\n", value, expected, isDivValud);
            error = 1;
        }
        expected = 1;
        if (loops == 1)
            value = 10;
        else if (loops == 2)
            value = -10.2;
    }
    if (error == 0) 
    {
        fprintf(stdout, "Unit Test : IsDivisorValid(double x) Tested OK\n");
    }
}

inline void CompareAndPrint(double result, double expected, const char* success, const char* failure)
{
    if (result == expected)
        fprintf(stdout, success);
    else
        fprintf(stderr, failure);
}

inline void Test_DoubleMath()
{
    CompareAndPrint(Add_D(2, 3), 5, "Unit Test : Add(double x, double y) Tested OK\n", "Error:  Add(double x, double y) FAILED THE TEST\n");
    CompareAndPrint(Subtract_D(5, 3), 2, "Unit Test : Subtract(double x, double y) Tested OK\n", "Error:  Subtract(double x, double y) FAILED THE TEST\n");
    CompareAndPrint(Multiply_D(5, 3), 15, "Unit Test : Multiply(double x, double y) Tested OK\n", "Error:  Multiply(double x, double y) FAILED THE TEST\n");
    CompareAndPrint(Divide_D(15, 3), 5, "Unit Test : Divide(double x, double y) Tested OK\n", "Error:  Divide(double x, double y) FAILED THE TEST\n");

    if (Round_D(Modulus_D(22, 3.2),3) == 2.8)
        fprintf(stdout, "Unit Test : Modulus(double x, double y) Tested OK\n");
    else
        fprintf(stderr, "Error:  Modulus(double x, double y) returned unexpected result. Expected : [%f]     Received: [%f]\n", 2.8, Modulus_D(22, 3.2));
}

#endif // !CalcUnitTests