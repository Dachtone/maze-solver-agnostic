#include "../mazesolver.h"

bool MazeSolver::Solver::PeriodicCorrectionSolution()
{
	/*
		let I be a runner
		let L be a list
		let F be a path
		repeat
			F = shortest path from I.CurrentPoint to the exit with L considered
			if F is not found
				end
			repeat
				if I reached the exit
					end
				L.Add(I.AdjacentObstacles)
				if I.CanGoTo(F.NextPoint)
					I.Go(F.NextPoint)
				else
					break
	*/

	List<Obstacle> obstacles;
	int previousPoint = CurrentPoint;

	Stack<int> path;
	while (GetShortestPath(CurrentPoint, obstacles, path))
	{
		bool pathIsBlocked = false;
		do
		{
			if (CurrentPoint == EndPoint) // Reached the end
				return true;

			// 4 possible directions
			for (unsigned int i = 0; i < 4; i++)
			{
				int adjacent = GetAdjacent(i);

				if (GetDirection(adjacent) == Direction::Invalid)
					continue;

				if (adjacent == previousPoint)
					continue;

				// Skip if we already know there's an obstacle
				bool obstacleMemorized = false;
				for (Obstacle& obstacle : obstacles)
				{
					if ((adjacent == obstacle.First && CurrentPoint == obstacle.Second) ||
						(adjacent == obstacle.Second && CurrentPoint == obstacle.First))
					{
						obstacleMemorized = true;
						break;
					}
				}

				if (obstacleMemorized)
					continue;

				if (!IsAccessible(adjacent))
				{
					obstacles.Push(Obstacle(adjacent, CurrentPoint));

					// Path is blocked if there's an obstacle in front of the next point in the calculated path
					if (adjacent == path.Peek())
						pathIsBlocked = true;
				}
			}

			if (!pathIsBlocked)
			{
				previousPoint = CurrentPoint;
				Move(path.Pop());
			}
		}
		while (!pathIsBlocked);
	}

	return false;
}