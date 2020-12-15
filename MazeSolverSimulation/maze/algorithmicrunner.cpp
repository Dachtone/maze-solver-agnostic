#include "mazesolver.h"

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
				I.Backtrack(B) until p is adjacent and accessible
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
						if (adjacent == point && IsAccessible(adjacent))
						{
							foundNextPoint = true;
							break;
						}
					}

					if (foundNextPoint)
						break;
				}

				// The point should always be found by backtracking
#if defined MAZESOLVER_ASSERTS_ALLOWED
				assert(foundNextPoint);
#endif
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