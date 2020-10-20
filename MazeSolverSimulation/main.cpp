#include <iostream>
#include <thread>
#include <chrono>
#include <limits>

#define NOMINMAX
#include <Windows.h>

#include "maze/mazesolver.h"

const char* const directions[]
{
	"Invalid",
	"Up",
	"Right",
	"Left",
	"Down"
};

bool Scan(MazeSolver::Direction direction);
void Move(MazeSolver::Direction direction);

void DrawMaze(int point);
void DrawMaze(MazeSolver::List<int> points);

void SetCursorToStartOfTheMaze();

// Obstacles
MazeSolver::List<MazeSolver::Obstacle> obstacles;

// The maze
MazeSolver::Solver maze(Scan, Move);

bool Scan(MazeSolver::Direction direction)
{
	int point = 0;
	switch (direction)
	{
	case MazeSolver::Direction::Up:
		point = maze.CurrentPoint + 5;
		break;
	case MazeSolver::Direction::Right:
 	   point = maze.CurrentPoint + 1;
		break;
	case MazeSolver::Direction::Left:
		point = maze.CurrentPoint - 1;
		break;
	case MazeSolver::Direction::Down:
		point = maze.CurrentPoint - 5;
		break;
	}

	bool found = false;
	for (MazeSolver::Obstacle &obstacle : obstacles)
	{
		if ((obstacle.First == maze.CurrentPoint && obstacle.Second == point) ||
			(obstacle.First == point && obstacle.Second == maze.CurrentPoint))
			found = true;
	}

	// std::cout << "Scanning " << directions[((int)direction)] << ": " << (found ? "Obstacle" : "Path") << std::endl;

	return !found;
}

void Move(MazeSolver::Direction direction)
{
	// std::cout << "-> " << directions[((int)direction)] << std::endl;

	SetCursorToStartOfTheMaze();

	using namespace std::literals::chrono_literals;
	std::this_thread::sleep_for(1s);

	DrawMaze(maze.CurrentPoint);
}

int main()
{
	std::cout << "Enter the obstacles (two numbers seperated by a space)" << std::endl;
	std::cout << "To stop listing the obstacles, write -1" << std::endl;

	std::cout << std::endl;
	std::cout << "Obstacles:" << std::endl;

	int temp;
	MazeSolver::Obstacle obstacle;
	while (true)
	{
		std::cin >> temp;
		if (temp == -1)
			break;
		obstacle.First = temp;

		std::cin >> temp;
		if (temp == -1)
			break;
		obstacle.Second = temp;

		if (obstacle.First == obstacle.Second)
		{
			std::cout << "Invalid obstacle (same point)." << std::endl;
			continue;
		}

		if (obstacle.First < 0 || obstacle.Second < 0 ||
			obstacle.First >= 25 || obstacle.Second >= 25)
		{
			std::cout << "Invalid obstacle (points don't exist)." << std::endl;
			continue;
		}

		if (obstacle.First > obstacle.Second)
		{
			obstacle.Second ^= obstacle.First;
			obstacle.First ^= obstacle.Second;
			obstacle.Second ^= obstacle.First;
		}

		int difference = obstacle.Second - obstacle.First;
		if (difference != 1 && difference != 5)
		{
			std::cout << "Invalid obstacle (points aren't adjacent)." << std::endl;
			continue;
		}

		for (MazeSolver::Obstacle &otherObstacle : obstacles)
		{
			if (otherObstacle.First == obstacle.First && otherObstacle.Second == obstacle.Second)
			{
				std::cout << "Repeating obstacle." << std::endl;
				continue;
			}
		}

		obstacles.Push(obstacle);
	}

	std::cout << std::endl;

	// DrawMaze(maze.CurrentPoint);
	// bool solved = maze.Solve(MazeSolver::Solution::AlgorithmicRunner);

	/*
	MazeSolver::Stack<int> path;
	bool solved = maze.GetShortestPath(0, obstacles, path);
	path.Push(0);
	DrawMaze(path.ToList());
	*/

	DrawMaze(maze.CurrentPoint);
	bool solved = maze.Solve(MazeSolver::Solution::PeriodicCorrection); // MazeSolver::Solution::PeriodicCorrection

	std::cout << "The maze " << (solved ? "is" : "cannot be") << " solved." << std::endl;

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.clear();
	std::cin.get();

	return 0;
}

void DrawMaze(int point)
{
	MazeSolver::List<int> points;
	points.Push(point);

	DrawMaze(points);
}

void DrawMaze(MazeSolver::List<int> points)
{
	std::string screen = "-----------\n";
	for (int i = 4; i >= 0; i--)
	{
		screen += "|";
		for (int j = 0; j < 5; j++)
		{
			int drawPoint = i * 5 + j;
			bool specialPoint = false;

			for (int point : points)
			{
				if (point == drawPoint)
				{
					specialPoint = true;
					break;
				}
			}

			screen += (specialPoint ? "O" : "*");

			if (j != 4)
			{
				bool foundObstacle = false;
				for (MazeSolver::Obstacle &obstacle : obstacles)
				{
					if ((obstacle.First == drawPoint && obstacle.Second == drawPoint + 1) ||
						(obstacle.First == drawPoint + 1 && obstacle.Second == drawPoint))
					{
						foundObstacle = true;
						break;
					}
				}

				if (foundObstacle)
					screen += "|";
				else
					screen += " ";
			}
		}
		screen += "|\n";

		if (i != 0)
		{
			screen += "|";
			for (int j = 0; j < 5; j++)
			{
				bool foundObstacle = false;
				for (MazeSolver::Obstacle &obstacle : obstacles)
				{
					if ((obstacle.First == i * 5 + j && obstacle.Second == (i - 1) * 5 + j) ||
						(obstacle.First == (i - 1) * 5 + j && obstacle.Second == i * 5 + j))
					{
						foundObstacle = true;
						break;
					}
				}

				if (foundObstacle)
					screen += "-";
				else
					screen += " ";

				if (j != 4)
					screen += " ";
			}
			screen += "|\n";
		}
	}
	screen += "-----------\n";

	std::cout << screen;
}

void SetCursorToStartOfTheMaze()
{
	const int length = 132;

	// HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(console, &consoleInfo);

	COORD coordinates = consoleInfo.dwCursorPosition;
	coordinates.Y -= 11;

	SetConsoleCursorPosition(console, coordinates);
}