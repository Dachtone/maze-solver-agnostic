#pragma once

#include <cassert>
#include <stack>

namespace MazeSolver
{

	// This class hides an implementation of the stack
	template<typename T>
	class Stack
	{

	public:
		/// <summary>
		/// Constructor.
		/// </summary>
		Stack() {}

		/// <summary>
		/// Checks whether the stack is empty.
		/// </summary>
		/// <returns>True if empty, false otherwise.</returns>
		bool IsEmpty()
		{
			return stack.empty();
		}

		/// <summary>
		/// Push a value on to the stack.
		/// </summary>
		/// <param name="value">A value to push.</param>
		void Push(T value)
		{
			stack.push(value);
		}

		/// <summary>
		/// Pop a value from the stack.
		/// </summary>
		/// <returns>A popped value.</returns>
		T Pop()
		{
			// The stack should be non-empty
			assert(!IsEmpty());

			T value = stack.top();
			stack.pop();

			return value;
		}

	private:
		// The implementation
		std::stack<T> stack;

	};

}