#include <iostream>
#include <thread>
#include <chrono>
#include <limits>
#include <vector>
#include <utility>

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
void SetCursorToStartOfTheMaze();

// Obstacles
std::vector<std::pair<int, int>> obstacles;

// The maze
MazeSolver::Solver maze = MazeSolver::Solver(Scan, Move);

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
	for (std::pair<int, int>& obstacle : obstacles)
	{
		if ((obstacle.first == maze.CurrentPoint && obstacle.second == point) ||
			(obstacle.first == point && obstacle.second == maze.CurrentPoint))
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
	std::pair<int, int> obstacle;
	while (true)
	{
		std::cin >> temp;
		if (temp == -1)
			break;
		obstacle.first = temp;

		std::cin >> temp;
		if (temp == -1)
			break;
		obstacle.second = temp;

		if (obstacle.first == obstacle.second)
		{
			std::cout << "Invalid obstacle (same point)." << std::endl;
			continue;
		}

		if (obstacle.first < 0 || obstacle.second < 0 ||
			obstacle.first >= 25 || obstacle.second >= 25)
		{
			std::cout << "Invalid obstacle (points don't exist)." << std::endl;
			continue;
		}

		if (obstacle.first > obstacle.second)
		{
			obstacle.second ^= obstacle.first;
			obstacle.first ^= obstacle.second;
			obstacle.second ^= obstacle.first;
		}

		int difference = obstacle.second - obstacle.first;
		if (difference != 1 && difference != 5)
		{
			std::cout << "Invalid obstacle (points aren't adjacent)." << std::endl;
			continue;
		}

		for (std::pair<int, int>& otherObstacle : obstacles)
		{
			if (otherObstacle.first == obstacle.first && otherObstacle.second == obstacle.second)
			{
				std::cout << "Repeating obstacle." << std::endl;
				continue;
			}
		}

		obstacles.push_back(obstacle);
	}

	std::cout << std::endl;

	DrawMaze(maze.CurrentPoint);
	bool solved = maze.Solve();

	std::cout << "The maze " << (solved ? "is" : "cannot be") << " solved." << std::endl;

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.clear();
	std::cin.get();

	return 0;
}

void DrawMaze(int point)
{
	std::string screen = "-----------\n";
	for (int i = 4; i >= 0; i--)
	{
		screen += "|";
		for (int j = 0; j < 5; j++)
		{
			screen += (i * 5 + j == point ? "O" : "*");
			if (j != 4)
			{
				bool foundObstacle = false;
				for (std::pair<int, int>& obstacle : obstacles)
				{
					if ((obstacle.first == i * 5 + j && obstacle.second == i * 5 + j + 1) ||
						(obstacle.first == i * 5 + j + 1 && obstacle.second == i * 5 + j))
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
				for (std::pair<int, int>& obstacle : obstacles)
				{
					if ((obstacle.first == i * 5 + j && obstacle.second == (i - 1) * 5 + j) ||
						(obstacle.first == (i - 1) * 5 + j && obstacle.second == i * 5 + j))
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