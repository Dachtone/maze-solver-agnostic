#pragma once

namespace MazeSolver
{

	class Obstacle
	{

	public:
		/// <summary>
		/// Constructor.
		/// </summary>
		/// <returns></returns>
		Obstacle();

		/// <summary>
		/// Constructor of the obstacle between two points.
		/// </summary>
		/// <param name="first">The first point.</param>
		/// <param name="second">The second point.</param>
		/// <returns></returns>
		Obstacle(int first, int second);

		/// <summary>
		/// The first point (less than or equal to second).
		/// </summary>
		int First;

		/// <summary>
		/// The second point (greater than or equal to first).
		/// </summary>
		/// <returns>The point.</returns>
		int Second;

	};

}