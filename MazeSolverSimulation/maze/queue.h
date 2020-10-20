#pragma once

#include "list.h"

namespace MazeSolver
{

	// This class hides an implementation of the queue
	template<typename T>
	class Queue
	{

	public:
		/// <summary>
		/// Constructor.
		/// </summary>
		Queue() {}

		/// <summary>
		/// Constructor from a list.
		/// </summary>
		Queue(List<T> other)
			: list(other) {}

		/// <summary>
		/// Checks whether the queue is empty.
		/// </summary>
		/// <returns>True if empty, false otherwise.</returns>
		bool IsEmpty()
		{
			return list.IsEmpty();
		}

		/// <summary>
		/// Push a value to the queue.
		/// </summary>
		/// <param name="value">A value to push.</param>
		void Push(T value)
		{
			list.Push(value);
		}

		/// <summary>
		/// Pop a value from the queue.
		/// </summary>
		/// <returns>A popped value.</returns>
		T Pop()
		{
			return list.Dequeue();
		}

		/// <summary>
		/// Converts the queue to a list.
		/// </summary>
		/// <returns>A list.</returns>
		List<T>& ToList()
		{
			return list;
		}

	private:
		// The implementation
		List<T> list;
		// std::queue<T> queue;

	};

}