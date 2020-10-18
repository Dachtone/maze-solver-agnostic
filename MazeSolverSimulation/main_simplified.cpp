#include "maze/mazesolver.h"

bool Scan(MazeSolver::Direction direction)
{
	// Scanning
}

void Move(MazeSolver::Direction direction)
{
	// Moving
}

int main()
{
	MazeSolver::Solver maze = MazeSolver::Solver(Scan, Move);
	maze.Solve();

	return 0;
}