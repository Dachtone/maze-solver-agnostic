#pragma once

namespace MazeSolver
{

	enum class Direction : unsigned char
	{
		Invalid,
		Up,
		Right,
		Left,
		Down
	};

	/// <summary>
	/// A function type for the scan function with signature:
	/// bool Scan(int point)
	/// </summary>
	/// <param>The direction of the point to scan.</param>
	/// <returns>True if there is direct path to the point, false otherwise.</returns>
	using ScanFunction = bool (*)(Direction);

	/// <summary>
	/// A function type for the movement function with signature:
	/// void Move(int point)
	/// </summary>
	/// <param>The direction of the adjacent point to move to.</param>
	using MovementFunction = void (*)(Direction);

	class Solver
	{

	private:
		// The dimensions of the maze are constant
		static const unsigned int width = 5;
		static const unsigned int height = 5;

		bool discoveredPoints[width * height];

		// The index of the start point
		const int startPoint = 0;

		// The index of the exit point
		const int endPoint = 24;

		// The external functions that the solver calls
		ScanFunction scanFunction;
		MovementFunction moveFunction;

	public:
		// The index of the current point
		int CurrentPoint = startPoint;

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="scanFunction">Function that tells the solver whether there is a direct path to a point.</param>
		/// <param name="moveFunction">Function that provides movement between adjacent cells.</param>
		Solver(ScanFunction scanFunction, MovementFunction moveFunction);

		/// <summary>
		/// Solves the maze.
		/// </summary>
		/// <returns>True if the maze is solved, false if there are no paths to the exit.</returns>
		bool Solve();

	private:
		bool IsDiscovered(int point);
		void Discover(int point);

		bool IsAdjacent(int point);
		int GetAdjacent(unsigned int index);

		Direction GetDirection(int point);

		bool IsAccessible(int point);
		void Move(int point);
	};

}