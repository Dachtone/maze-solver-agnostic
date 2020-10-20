#include "obstacle.h"

MazeSolver::Obstacle::Obstacle()
{
	First = 0;
	Second = 0;
}

MazeSolver::Obstacle::Obstacle(int first, int second)
{
	if (first <= second)
	{
		First = first;
		Second = second;
	}
	else
	{
		First = second;
		Second = first;
	}
}