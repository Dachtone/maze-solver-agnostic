#pragma once

#include "list.h"
#include "stack.h"
#include "queue.h"
#include "obstacle.h"

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

	enum class Solution : unsigned char
	{
		PeriodicCorrection,
		AlgorithmicRunner
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

	public:
		// The dimensions of the maze are constant
		static const unsigned int Width = 5;
		static const unsigned int Height = 5;

		// The index of the start point
		const int StartPoint = 0;

		// The index of the exit point
		const int EndPoint = 24;

	private:
		bool discoveredPoints[Width * Height];

		// The external functions that the solver calls
		ScanFunction scanFunction;
		MovementFunction moveFunction;

	public:
		// The index of the current point
		int CurrentPoint = StartPoint;

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="scanFunction">Function that tells the solver whether there is a direct path to a point.</param>
		/// <param name="moveFunction">Function that provides movement between adjacent cells.</param>
		Solver(ScanFunction scanFunction, MovementFunction moveFunction);

		/// <summary>
		/// Solves the maze.
		/// </summary>
		/// <param name="solution">The solution to use when solving.</param>
		/// <returns>True if the maze is solved, false if there are no paths to the exit.</returns>
		bool Solve(Solution solution);

	private:
		/// <summary>
		/// Follows the BFS shortest path, if an obstacle is hit, the shortest path is recalculated.
		/// </summary>
		/// <returns>True if the maze is solved, false if there are no paths to the exit.</returns>
		bool PeriodicCorrectionSolution();

		/// <summary>
		/// An algorithm based on DFS with backtracking.
		/// </summary>
		/// <returns>True if the maze is solved, false if there are no paths to the exit.</returns>
		bool AlgorithmicRunnerSolution();

	public:
		/// <summary>
		/// Searches for the shortest path.
		/// </summary>
		/// <param name="initialPoint">The starting point.</param>
		/// <param name="obstacles">A list of obstacles.</param>
		/// <param name="path">A place to store the shortest path.</param>
		/// <returns>True if the maze is solved, false if there are no paths to the exit.</returns>
		bool GetShortestPath(int initialPoint, MazeSolver::List<MazeSolver::Obstacle> obstacles, MazeSolver::Stack<int>& path);

	private:
		bool IsDiscovered(int point);
		void Discover(int point);
		void ClearDiscovered();

		bool IsAdjacent(int point);
		bool IsAdjacent(int point, int currentPoint);

		int GetAdjacent(unsigned int index);
		int GetAdjacent(unsigned int index, int currentPoint);

		Direction GetDirection(int point);
		Direction GetDirection(int point, int currentPoint);

		bool IsAccessible(int point);
		void Move(int point);
	};

}