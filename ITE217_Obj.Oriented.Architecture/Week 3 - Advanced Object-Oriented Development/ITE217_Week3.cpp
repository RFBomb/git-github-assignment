//
// Robert Brenckman
// Charter Oak - ITE217 ( Fall 2025 )
// 11/5/2025
//

#include <stdexcept>
#include <iostream>
#include <string>
#include "Stack.h"
#include "ConsoleHelpers.h"

using namespace ITE217;
using namespace std;


void UnitTestStack(Stack<int>& stack)
{
	// validate stack is empty on construction
	if (!stack.IsEmpty()) {
		throw std::logic_error("Stack reported not empty after construction");
	}

	int x = 1;
	stack.Push(x);

	if (stack.Size() != 1) {
		throw std::logic_error("Stack reported incorrect size - expected 1");
	}
	if (stack.IsEmpty()) {
		throw std::logic_error("Stack reported empty when it has a stack size of 1");
	}
	if (stack.Peek() != 1) {
		throw std::logic_error("Stack returned incorrect value from Peek()");
	}
	stack.Pop();
	if (!stack.IsEmpty()) {
		throw std::logic_error("Stack reported NOT EMPTY after popping the only item in collection");
	}
	if (stack.Size() != 0) {
		throw std::logic_error("Stack reported a non-zero size after popping the only item in collection");
	}

	try {
		stack.Pop();
		throw std::logic_error("Stack did not throw 'out_of_range' when popping an empty stack.");
	}
	catch (std::out_of_range) { /*OK*/ }

	try {
		stack.Peek();
		throw std::logic_error("Stack did not throw 'out_of_range' when peeking an empty stack.");
	}
	catch (std::out_of_range) { /*OK*/ }

	int arr[] = { 1, 2, 3 };
	stack.Push(arr[0]);
	stack.Push(arr[1]);
	stack.Push(arr[2]);
	x = 2;
	while (!stack.IsEmpty())
	{
		if (stack.Peek() == arr[x])
		{
			x--;
			stack.Pop();
		}
		else
		{
			throw std::logic_error("Stack was stored in incorrect order");
		}
	}

	std::cout << "Unit Test Successfull !!!" << endl << endl;
}

int GetMenuChoice(bool showText) {

	const string s = "Enter '0' to print the stacked values. \nOtherwise, enter a number from 1 to 100 to add it to the stack.\nInput: ";
	if (showText)
		return getInt(s, 0, 100);
	else
		return getInt("Input: ", 0, 100);
}

int main() {

	// This program simply acts as a unit test for the stack implementation

	ITE217::Stack<int> stack;
	int x;
	try {
		
		UnitTestStack(stack);
		
		bool run = true;
		while (run) 
		{
			bool showText = true;
			while (run)
			{
				x = GetMenuChoice(showText);
				showText = false;
				switch (x)
				{
				case 0:
					run = false;
					break;
				default:
					stack.Push(x);
					break;
				}
			}
			std::cout << ConsoleColor_Green << "Printing " << stack.Size() << " Values from the stack." << endl << ConsoleColor_Normal << "------------------------------------------" << endl;
			x = 0;
			while (!stack.IsEmpty())
			{
				std::cout << ConsoleColor_Green << x++ << " : " << ConsoleColor_Normal << stack.Peek() << endl;
				stack.Pop();
			}
			std::cout << ConsoleColor_Normal << "------------------------------------------" << endl;
			run = (1 == getInt("Enter '1' to build another stack, otherwise enter any other value to quit.", -999, 999));
		}
	}
	catch (exception e)
	{
		printErr(e.what());
		std::cin >> x;
	}
}
