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
#if defined MAZESOLVER_ASSERTS_ALLOWED
	default:
		assert(false); // Tried to solve using a not implemented solution
#endif
	}

	// Code should never be able to get here
	return false;
}

bool MazeSolver::Solver::GetShortestPath(int initialPoint, List<Obstacle>& obstacles, Stack<int> &path)
{
	/*
		let M be a maze
		let Q be a queue
		let F be a path
		let p be a point
		let a be a point

		Q.push(initial point)
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

	int point = queue.Peek();
	do
	{
		// Direction previousDirection = GetDirection(queue.Peek(), point);

		point = queue.Pop();
		if (point == EndPoint)
			break;

		// 4 possible directions
		for (unsigned int i = 0; i < 4; i++)
		{
			// int adjacent = GetAdjacent(i, point, previousDirection);
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

	// Return the result

	if (!path.IsEmpty())
		path = Stack<int>(); // Initialize to an empty path

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

int MazeSolver::Solver::GetAdjacent(Direction direction, int currentPoint)
{
	int point;
	switch (direction)
	{
	case Direction::Up:
		point = currentPoint + Width;
		if (point >= Width * Height)
			return -1;

		return point;
	case Direction::Right:
		point = currentPoint + 1;
		if (point % Width == 0)
			return -1;

		return point;
	case Direction::Left:
		if (currentPoint % Width == 0)
			return -1;

		point = currentPoint - 1;
		return point;
	case Direction::Down:
		if (currentPoint < Width)
			return -1;

		point = currentPoint - Width;
		return point;
	default:
		return -1;
	}
}

int MazeSolver::Solver::GetAdjacent(unsigned int index)
{
	return GetAdjacent(index, CurrentPoint);
}

int MazeSolver::Solver::GetAdjacent(unsigned int index, int currentPoint, bool reverse)
{
#if defined MAZESOLVER_ASSERTS_ALLOWED
	assert(index < 4);
#endif

	// Directional preference: Right -> Up -> Left -> Down

	if (reverse)
		index = 3 - index;

	switch (index)
	{
	case 0: // Right
		return GetAdjacent(Direction::Right, currentPoint);
	case 1: // Up
		return GetAdjacent(Direction::Up, currentPoint);
	case 2: // Left
		return GetAdjacent(Direction::Left, currentPoint);
	case 3: // Down
		return GetAdjacent(Direction::Down, currentPoint);
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

int MazeSolver::Solver::GetAdjacent(unsigned int index, Direction previousDirection)
{
	return GetAdjacent(index, CurrentPoint, previousDirection);
}

int MazeSolver::Solver::GetAdjacent(unsigned int index, int currentPoint, Direction previousDirection)
{
#if defined MAZESOLVER_ASSERTS_ALLOWED
	assert(index < 4);
#endif

	// Directional preference: Move in a straight line as much as possible
	// Right -> Up -> Left -> Down
	
	switch (index)
	{
	case 0:
		if (previousDirection == Direction::Up)
			return GetAdjacent(Direction::Up, currentPoint);
		else
			return GetAdjacent(Direction::Right, currentPoint);
	case 1:
		if (previousDirection == Direction::Up)
			return GetAdjacent(Direction::Right, currentPoint);
		else
			return GetAdjacent(Direction::Up, currentPoint);
	case 2:
		if (previousDirection == Direction::Down)
			return GetAdjacent(Direction::Down, currentPoint);
		else
			return GetAdjacent(Direction::Left, currentPoint);
	case 3:
		if (previousDirection == Direction::Down)
			return GetAdjacent(Direction::Left, currentPoint);
		else
			return GetAdjacent(Direction::Down, currentPoint);
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
		if (point / Width != currentPoint / Width)
			return Direction::Invalid;

		return Direction::Right;
	case -1:
		if (point / Width != currentPoint / Width)
			return Direction::Invalid;

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
#if defined MAZESOLVER_ASSERTS_ALLOWED
	assert(direction != Direction::Invalid);
#endif

	CurrentPoint = point;
	moveFunction(direction);
}