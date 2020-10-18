#pragma once

#include <cassert>
#include <vector>

namespace MazeSolver
{

	// This class hides an implementation of the list
	template<typename T>
	class List
	{

	public:
		/// <summary>
		/// Constructor.
		/// </summary>
		List() {}

		/// <summary>
		/// Checks whether the list is empty.
		/// </summary>
		/// <returns>True if empty, false otherwise.</returns>
		bool IsEmpty()
		{
			return vector.empty();
		}

		/// <summary>
		/// Push a value to the list.
		/// </summary>
		/// <param name="value">A value to push.</param>
		void Push(T value)
		{
			vector.push_back(value);
		}

		/// <summary>
		/// Pop a value from the list.
		/// </summary>
		/// <returns>A popped value.</returns>
		T Pop()
		{
			// The stack should be non-empty
			assert(!IsEmpty());

			T value = vector.back();
			vector.pop_back();

			return value;
		}

		// A quick hack
		std::vector<T> &GetImplementation()
		{
			return vector;
		}
	private:
		// The implementation
		std::vector<T> vector;

	};

}