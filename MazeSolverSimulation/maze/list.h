#pragma once

#include <cassert>
#include <vector>

namespace MazeSolver
{
	template<typename List>
	class ListIterator
	{

	public:
		using ValueType = typename List::ValueType;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;

	public:
		ListIterator(PointerType pointer)
			: pointer(pointer) {}

		ListIterator& operator++() // Prefix
		{
			pointer++;
			return *this;
		}

		ListIterator operator++(int) // Postfix
		{
			ListIterator iterator = *this;
			pointer++;
			return iterator;
		}

		ListIterator& operator--() // Prefix
		{
			pointer--;
			return *this;
		}

		ListIterator operator--(int) // Postfix
		{
			ListIterator iterator = *this;
			pointer--;
			return iterator;
		}

		ReferenceType operator[](int index)
		{
			return *(pointer + index);
		}

		ReferenceType operator*()
		{
			return *pointer;
		}

		PointerType operator->()
		{
			return pointer;
		}

		bool operator==(const ListIterator& other) const
		{
			return pointer == other.pointer;
		}

		bool operator!=(const ListIterator& other) const
		{
			return pointer != other.pointer;
		}

	private:
		PointerType pointer;

	};

	// This class hides an implementation of the list
	template<typename T>
	class List
	{

	public:
		using ValueType = T;
		using Iterator = ListIterator<List<T>>;

	public:
		/// <summary>
		/// Constructor.
		/// </summary>
		List()
			: vector() {}

		/// <summary>
		/// Copy constructor.
		/// </summary>
		List(List<T> &other)
			: vector(other.vector) {}

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

		/// <summary>
		/// Peek into the list.
		/// </summary>
		/// <returns>A value.</returns>
		T Peek()
		{
			// The stack should be non-empty
			assert(!IsEmpty());
			
			return vector.back();
		}

		/// Dequeue a value from the list.
		/// </summary>
		/// <returns>A dequeued value.</returns>
		T Dequeue()
		{
			// The stack should be non-empty
			assert(!IsEmpty());

			T value = vector.front();
			vector.erase(vector.begin());

			return value;
		}

		// Iterating

		Iterator begin()
		{
			if (IsEmpty())
				return Iterator(nullptr);

			return Iterator(&(*(vector.begin())));
		}

		Iterator end()
		{
			if (IsEmpty())
				return Iterator(nullptr);

			return Iterator(&(*(--vector.end())) + 1);
		}

	private:
		// The implementation
		std::vector<T> vector;

	};

}