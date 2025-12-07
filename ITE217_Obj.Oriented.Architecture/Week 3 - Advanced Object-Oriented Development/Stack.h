//
// Robert Brenckman
// Charter Oak - ITE217 ( Fall 2025 )
// 11/5/2025
//

#pragma once

#include <stdexcept>
#include <vector>

namespace ITE217
{

	template <typename T>
	class Stack
	{

	private:
		std::vector<T> elements;

		static void ThrowOutOfRangeError() { throw std::out_of_range("Out of Range Exception : Stack is empty"); }

	public:

		// Check if the stack is empty.
		bool IsEmpty()
		{
			return this->elements.size() == 0;
		}

		// Get the current size of the stack
		int Size()
		{
			return this->elements.size();
		}

		// Retrieves the item from the top of the stack
		// Throws out_of_range error if stack is empty.
		T& Peek()
		{
			if (IsEmpty())
			{
				ThrowOutOfRangeError();
			}
			return this->elements.back();
		}

		// Removes the top item from the collection.
		// Throws out_of_range error if stack is empty.
		void Pop()
		{
			if (IsEmpty())
			{
				ThrowOutOfRangeError();
			}
			return this->elements.pop_back();
		}

		// Add an item to the top of the stack.
		void Push(T& value)
		{
			this->elements.push_back(value);
		}
	};
}