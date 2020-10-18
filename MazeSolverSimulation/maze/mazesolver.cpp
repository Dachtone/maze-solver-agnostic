#include "mazesolver.h"
#include "list.h"
#include "stack.h"

MazeSolver::Solver::Solver(ScanFunction scanFunction, MovementFunction moveFunction)
    : discoveredPoints()
{
	this->scanFunction = scanFunction;
	this->moveFunction = moveFunction;
}

bool MazeSolver::Solver::Solve()
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
                    S.push(w)
        while S is not empty
	*/

    if (CurrentPoint == endPoint)
        return true;

    List<int> memory = List<int>();
    Stack<int> path = Stack<int>();

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

            if (point == endPoint) // Reached the end
                return true;
        }

        Discover(point);

        // 4 possible directions
        for (unsigned int i = 0; i < 4; i++)
        {
            int adjacent = GetAdjacent(i);
            if (IsDiscovered(adjacent) || !IsAccessible(adjacent))
                continue;

            for (int memorizedPoint : memory.GetImplementation())
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

bool MazeSolver::Solver::IsAdjacent(int point)
{
    return GetDirection(point) != Direction::Invalid;
}

int MazeSolver::Solver::GetAdjacent(unsigned int index)
{
    assert(index < 4);

    int point;
    // Directional preference: Up / Right -> Left -> Down
    // thus these directions should be provided in the reversed order
    switch (index)
    {
    case 0: // Down
        if (CurrentPoint < width)
            return -1;

        point = CurrentPoint - width;
        return point;
    case 1: // Left
        if (CurrentPoint % width == 0)
            return -1;

        point = CurrentPoint - 1;
        return point;
    /*
    case 2: // Right
        point = CurrentPoint + 1;
        if (point % width == 0)
            return -1;

        return point;
    case 3: // Up
        point = CurrentPoint + width;
        if (point >= width * height)
            return -1;

        return point;
    */
    case 2:
    case 3:
        bool reverse = CurrentPoint % 2 != 0;
        if (index == 3)
            reverse = !reverse;

        point = reverse ? CurrentPoint + width : CurrentPoint + 1;
        if (reverse)
        {
            if (point >= width * height)
                return -1;
        }
        else
        {
            if (point % width == 0)
                return -1;
        }

        return point;
    }

    // Code should never be able to get here
    return -1;
}

MazeSolver::Direction MazeSolver::Solver::GetDirection(int point)
{
    int difference = point - CurrentPoint;
    switch (difference)
    {
    case width:
        return Direction::Up;
    case 1:
        return Direction::Right;
    case -1:
        return Direction::Left;
    case -((int)width):
        return Direction::Down;
    default:
        return Direction::Invalid;
    }
}

bool MazeSolver::Solver::IsAccessible(int point)
{
    if (point == -1)
        return false;

    Direction direction = GetDirection(point);
    assert(direction != Direction::Invalid);

    return scanFunction(direction);
}

void MazeSolver::Solver::Move(int point)
{
    Direction direction = GetDirection(point);
    assert(direction != Direction::Invalid);

    CurrentPoint = point;
    moveFunction(direction);
}