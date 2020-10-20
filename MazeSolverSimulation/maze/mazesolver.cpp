#include "mazesolver.h"

MazeSolver::Solver::Solver(ScanFunction scanFunction, MovementFunction moveFunction)
	: discoveredPoints()
{
	this->scanFunction = scanFunction;
	this->moveFunction = moveFunction;
}

bool MazeSolver::Solver::Solve(Solution solution)
{
	switch (solution)
	{
	case Solution::AlgorithmicRunner:
		return AlgorithmicRunnerSolution();
	case Solution::PeriodicCorrection:
		return PeriodicCorrectionSolution();
	default:
		assert(false); // Tried to solve using a not implemented solution
	}

	// Code should never be able to get here
	return false;
}

bool MazeSolver::Solver::PeriodicCorrectionSolution()
{
	/*
		let I be a runner
		let L be a list
		let F be a path
		do
			F = shortest path from I.CurrentPoint to the exit with L considered
			do
				if I reached the exit
					end
				L.Add(I.AdjacentObstacles)
				if I.CanGoTo(F.NextPoint)
					I.Go(F.NextPoint)
			while I.CanGoTo(F.NextPoint)
		while F not empty
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
				for (Obstacle &obstacle : obstacles)
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
					{
						pathIsBlocked = true;
						path.Pop(); // No longer the preferred direction
					}
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

bool MazeSolver::Solver::GetShortestPath(int initialPoint, List<Obstacle> obstacles, Stack<int> &path)
{
	/*
		let M be a maze
		let Q be a queue
		let F be a path
		let p be a point
		let a be a point

		do
			p = Q.Pop()

			if p is the exit
				while p.Parent is not start
					a = p.Parent
					F.Add(a)
					p = a
				return F

			for w in M.AdjacentEdges(p)
				if w is not discovered and is accessible
					w.Parent = p
					label w as discovered
					Q.Push(w)
		while Q is not empty
	*/


	// The parent point for each point
	int previous[Width * Height] = { 0 }; // Initialized to zero
	Queue<int> queue;

	ClearDiscovered();

	Discover(initialPoint);
	queue.Push(initialPoint);

	int point;
	do
	{
		point = queue.Pop();

		if (point == EndPoint)
			break;

		// 4 possible directions
		for (unsigned int i = 0; i < 4; i++)
		{
			int adjacent = GetAdjacent(i, point, false);
			if (IsDiscovered(adjacent) || GetDirection(adjacent, point) == Direction::Invalid)
				continue;

			bool pathIsBlocked = false;
			for (Obstacle &obstacle : obstacles)
			{
				if ((adjacent == obstacle.First && point == obstacle.Second) ||
					(adjacent == obstacle.Second && point == obstacle.First))
				{
					pathIsBlocked = true;
					break;
				}
			}

			if (pathIsBlocked)
				continue;
			
			previous[adjacent] = point;
			Discover(adjacent);
			queue.Push(adjacent);
		}
	}
	while (!queue.IsEmpty());

	if (point != EndPoint) // The exit is unreachable
		return false;

	path.Push(point);
	while (previous[point] != initialPoint)
	{
		path.Push(previous[point]);
		point = previous[point];
	}
	
	return true;
}

bool MazeSolver::Solver::AlgorithmicRunnerSolution()
{
	/*
		let I be a runner
		let M be a maze
		let S be a stack
		let B be a path
		let p be a point
		p = start
		S.Push(p)
		do
			p = S.Pop()
			if p is not adjacent or not accessible
				I.Backtrack(B) until p is adjacent
				B.Minus()

			B.Plus()
			I.Go(p)

			if p is the exit
				end

			label p as discovered
			for w in M.AdjacentEdges(p)
				if w is not discovered and is accessible and not in S
					S.Push(w)
		while S is not empty
	*/

	if (CurrentPoint == EndPoint)
		return true;

	List<int> memory;
	Stack<int> path;

	int point = CurrentPoint;
	memory.Push(point);
	do
	{
		point = memory.Pop();

		if (CurrentPoint != point)
		{
			if (!IsAdjacent(point) || !IsAccessible(point)) // There are no direct paths
			{
				// Backtrack
				bool foundNextPoint = false;
				while (!path.IsEmpty())
				{
					int parent = path.Pop();
					Move(parent);

					// 4 possible directions
					for (unsigned int i = 0; i < 4; i++)
					{
						int adjacent = GetAdjacent(i);
						if (adjacent == point)
						{
							foundNextPoint = true;
							break;
						}
					}

					if (foundNextPoint)
						break;
				}

				// The point should always be found by backtracking
				assert(foundNextPoint);
			}

			path.Push(CurrentPoint);
			Move(point);

			if (point == EndPoint) // Reached the end
				return true;
		}

		Discover(point);

		// 4 possible directions
		for (unsigned int i = 0; i < 4; i++)
		{
			int adjacent = GetAdjacent(i);
			if (IsDiscovered(adjacent) || !IsAccessible(adjacent))
				continue;

			for (int memorizedPoint : memory)
			{
				if (memorizedPoint == adjacent)
					continue;
			}

			memory.Push(adjacent);
		}
	}
	while (!memory.IsEmpty());

	return false;
}

bool MazeSolver::Solver::IsDiscovered(int point)
{
	return discoveredPoints[point];
}

void MazeSolver::Solver::Discover(int point)
{
	discoveredPoints[point] = true;
}

void MazeSolver::Solver::ClearDiscovered()
{
	for (int i = 0; i < Width * Height; i++)
	{
		discoveredPoints[i] = false;
	}
}

bool MazeSolver::Solver::IsAdjacent(int point)
{
	return GetDirection(point) != Direction::Invalid;
}

bool MazeSolver::Solver::IsAdjacent(int point, int currentPoint)
{
	return GetDirection(point, currentPoint) != Direction::Invalid;
}

int MazeSolver::Solver::GetAdjacent(unsigned int index)
{
	return GetAdjacent(index, CurrentPoint);
}

int MazeSolver::Solver::GetAdjacent(unsigned int index, int currentPoint, bool reverse)
{
	assert(index < 4);

	// Directional preference: Right -> Up -> Left -> Down
	if (reverse)
		index = 3 - index;

	int point;
	switch (index)
	{
	case 0: // Right
		point = currentPoint + 1;
		if (point % Width == 0)
			return -1;

		return point;
	case 1: // Up
		point = currentPoint + Width;
		if (point >= Width * Height)
			return -1;

		return point;
	case 2: // Left
		if (currentPoint % Width == 0)
			return -1;

		point = currentPoint - 1;
		return point;
	case 3: // Down
		if (currentPoint < Width)
			return -1;

		point = currentPoint - Width;
		return point;
	/*
	case 2:
	case 3:
		bool reverse = currentPoint % 2 != 0;
		if (index == 3)
			reverse = !reverse;

		point = reverse ? currentPoint + Width : currentPoint + 1;
		if (reverse)
		{
			if (point >= Width * Height)
				return -1;
		}
		else
		{
			if (point % Width == 0)
				return -1;
		}

		return point;
	*/
	}

	// Code should never be able to get here
	return -1;
}

MazeSolver::Direction MazeSolver::Solver::GetDirection(int point)
{
	return GetDirection(point, CurrentPoint);
}

MazeSolver::Direction MazeSolver::Solver::GetDirection(int point, int currentPoint)
{
	if (point == -1)
		return Direction::Invalid;

	int difference = point - currentPoint;
	switch (difference)
	{
	case Width:
		return Direction::Up;
	case 1:
		return Direction::Right;
	case -1:
		return Direction::Left;
	case -((int)Width):
		return Direction::Down;
	default:
		return Direction::Invalid;
	}
}

bool MazeSolver::Solver::IsAccessible(int point)
{
	Direction direction = GetDirection(point);
	if (direction == Direction::Invalid)
		return false;

	return scanFunction(direction);
}

void MazeSolver::Solver::Move(int point)
{
	Direction direction = GetDirection(point);
	assert(direction != Direction::Invalid);

	CurrentPoint = point;
	moveFunction(direction);
}