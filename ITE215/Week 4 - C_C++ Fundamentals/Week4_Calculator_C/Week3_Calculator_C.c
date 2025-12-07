// Week3_Calculator_C.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>


#include "Calculator.h"
#include "CalculatorUnitTests.h"

/*
* Create a simple calculator program in C that can perform the following operations:

    Addition
    Subtraction
    Multiplication
    Division
    Modulus

The program should prompt the user to select an operation and input two integers. It should then perform the selected operation and display the result. The program should continue to run until the user chooses to exit.
Requirements:

    Menu Display: Display a menu of operations for the user to choose from.
    Input Handling: Accept user input for the operation and two integers.
    Function Implementation: Implement separate functions for each operation (addition, subtraction, multiplication, division, modulus).
    Control Structure: Use a switch-case statement to call the appropriate function based on user choice.
    Loop for Continuation: Use a loop to allow the user to perform multiple operations until they choose to exit.

Complete each of the programming exercises individually. Submit screenshot of VSC or other compiler showing both code and output after successfully running each exercise.
Be sure to review the attached grading rubric.
*/


static void UnitTests()
{
    Test_IsDivisorValid();
    Test_DoubleMath();
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static char ChooseOperation()
{
    int attempted = 0;
    while (1) {
        if (attempted)
            printf("\nInvalid operation, please select from the following options:");
        else
            printf("Select an operation to perform : ");

        printf("\n1 : Addition (+)");
        printf("\n2 : Subtraction (-)");
        printf("\n3 : Multiplication (*)");
        printf("\n4 : Division (/)");
        printf("\n5 : Modulus (%%)");
        printf("\nq : Quit\n");

        char symbol;
        while ((symbol = getchar()) == '\n');
        clearInputBuffer();

        // normalize the inputs
        switch (symbol)
        {
        case '1': symbol = '+'; break;
        case '2': symbol = '-'; break;
        case '3': symbol = '*'; break;
        case '\\':
        case '/':
        case '4': 
            symbol = '/'; break;
        case '5': symbol = '%'; break;
        }

        // check for valid inputs
        switch (symbol)
        {
        case 'q':
        case 'Q':
            exit(0);
            break;
        case '+':   // addition
        case '-':   // subtraction
        case '*':   // multiplication
        case 'x':
        case 'X':
        case '/':
        case '%':   // modulus
            printf("\n");
            return symbol;
        default:
            attempted = 1;
            break;
        }
    }
}

static double GetInputNumber()
{
    int num;
    int attempted = 0;
    while(1)
    {
        if (attempted)
            printf("\nInvalid Input. Enter an integer: ");
        else
            printf("Enter an integer: ");
        
        int success = scanf_s("%d", &num);// swap for %f to accept doubles!
        if (success) 
        {
            return num;
        }
        clearInputBuffer();
        attempted = 1;
    
    }
}

// this calls methods from the calculator header, but realistically could be inlined here. 
static int DoMath(const int x, const int y, const char symbol)
{
    switch (symbol)
    {
    case '+': 
        return Add(x, y);
    
    case '-': 
        return Subtract(x, y);
    
    case '*': 
    case 'x':
    case 'X':
        return Multiply(x, y);
    
    case '\\':  // division
    case '/':
        return Divide(x, y);
    
    case '%':   // modulus
        return Modulus(x, y);
    }
}

int main()
{
    //UnitTests();
    while (1)
    {
        char symbol = ChooseOperation();
        int x = GetInputNumber();
        int y = GetInputNumber();
        switch (symbol)
        {
        case '/':
        case '%':
            if (!IsDivisorValid(y))
            {
                fprintf(stderr, "ERROR : Divisor must be non-zero!\n\n");
                break;
            }
        default:
            fprintf(stdout, "%d %c %d = %d\n\n", x, symbol, y, DoMath(x, y, symbol));
        }
        
    }
}
