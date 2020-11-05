#pragma once

#include "list.h"

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
		/// Constructor from a list.
		/// </summary>
		Stack(List<T> other)
			: list(other) {}

		/// <summary>
		/// Checks whether the stack is empty.
		/// </summary>
		/// <returns>True if empty, false otherwise.</returns>
		bool IsEmpty()
		{
			return list.IsEmpty();
		}

		/// <summary>
		/// Push a value on to the stack.
		/// </summary>
		/// <param name="value">A value to push.</param>
		void Push(T value)
		{
			list.Push(value);
		}

		/// <summary>
		/// Peek into the stack.
		/// </summary>
		/// <returns>A value.</returns>
		T Peek()
		{
			return list.Peek();
		}

		/// <summary>
		/// Pop a value from the stack.
		/// </summary>
		/// <returns>A popped value.</returns>
		T Pop()
		{
			return list.Pop();
		}

		/// <summary>
		/// Converts the stack to a list.
		/// </summary>
		/// <returns>A list.</returns>
		List<T> &ToList()
		{
			return list;
		}

	private:
		// The implementation
		List<T> list;
		// std::stack<T> stack;

	};

}