#pragma once

#if defined MAZESOLVER_ASSERTS_ALLOWED
#include <cassert>
#endif

// #include <vector>

namespace MazeSolver
{
	// Iterator Class
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
		{
			size = 0;
			capacity = grow;
			array = new T[capacity];
		}

		/// <summary>
		/// Copy constructor.
		/// </summary>
		List(List<T>& other)
		{
			CopyFrom(other);
		}

		/// <summary>
		/// Destructor.
		/// </summary>
		~List()
		{
			if (array != nullptr)
			{
				delete[] array;
				array = nullptr;
			}
		}

		/// <summary>
		/// Assignment operator.
		/// </summary>
		/// <param name="other">The list to copy to this list.</param>
		/// <returns>This list.</returns>
		List<T>& operator=(const List<T>& other)
		{
			CopyFrom(other);

			return *this;
		}

		/// <summary>
		/// Gets the list size.
		/// </summary>
		/// <returns>The size of the list.</returns>
		int Size()
		{
			return size;
		}

		/// <summary>
		/// Checks whether the list is empty.
		/// </summary>
		/// <returns>True if empty, false otherwise.</returns>
		bool IsEmpty()
		{
			return size == 0;
		}

		/// <summary>
		/// Push a value to the list.
		/// </summary>
		/// <param name="value">A value to push.</param>
		void Push(T value)
		{
			if (size == capacity)
				Resize(capacity + grow);

			array[size++] = value;
		}

		/// <summary>
		/// Peek into the list.
		/// </summary>
		/// <returns>A value.</returns>
		T Peek()
		{
			// The stack should be non-empty
#if defined MAZESOLVER_ASSERTS_ALLOWED
			assert(!IsEmpty());
#endif

			return array[size - 1];
		}

		/// <summary>
		/// Pop a value from the list.
		/// </summary>
		/// <returns>A popped value.</returns>
		T Pop()
		{
			// The stack should be non-empty
#if defined MAZESOLVER_ASSERTS_ALLOWED
			assert(!IsEmpty());
#endif

			T value = array[size - 1];
			size--;

			return value;
		}

		/// <summary>
		/// Peek into the front of the list.
		/// </summary>
		/// <returns>A value.</returns>
		T PeekFront()
		{
			// The stack should be non-empty
#if defined MAZESOLVER_ASSERTS_ALLOWED
			assert(!IsEmpty());
#endif

			return array[0];
		}

		/// Dequeue a value from the list.
		/// </summary>
		/// <returns>A dequeued value.</returns>
		T Dequeue()
		{
			// The stack should be non-empty
#if defined MAZESOLVER_ASSERTS_ALLOWED
			assert(!IsEmpty());
#endif

			T value = array[0];
			for (unsigned int i = 1; i < size; i++)
				array[i - 1] = array[i];
			size--;

			return value;
		}

		// Iterating
		Iterator begin()
		{
			if (IsEmpty())
				return Iterator(nullptr);

			return Iterator(&array[0]);
		}

		Iterator end()
		{
			if (IsEmpty())
				return Iterator(nullptr);

			return Iterator((&array[size - 1]) + 1);
		}

	private:
		/// <summary>
		/// Copy from another list.
		/// </summary>
		/// <param name="other">The list to copy from.</param>
		void CopyFrom(const List<T>& other)
		{
			size = other.size;
			capacity = size;

			if (array != nullptr)
				delete[] array;
			array = new T[capacity];

			for (unsigned int i = 0; i < size; i++)
				array[i] = other.array[i];
		}

		/// <summary>
		/// Resizes the array.
		/// </summary>
		/// <param name="newCapacity">The new size of the array.</param>
		void Resize(unsigned int newCapacity)
		{
			if (capacity == newCapacity)
				return;

			T* newArray = new T[newCapacity];
			for (unsigned int i = 0; i < size && i < newCapacity; i++)
				newArray[i] = array[i];
			
			delete[] array;
			array = newArray;

			capacity = newCapacity;
			if (size > capacity)
				size = capacity;
		}

	private:
		// The implementation
		T* array = nullptr;
		// std::vector<T> vector;

		unsigned int size;
		unsigned int capacity;

		const unsigned int grow = 5;

	};

}