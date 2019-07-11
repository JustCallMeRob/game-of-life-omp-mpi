// GOL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <windows.h>
#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#define N 8

using namespace std;
using namespace std::chrono;

const int gridSize = 50;
void showGrid(bool grid[gridSize + 1][gridSize + 1]);
void getNextState(bool grid[gridSize + 1][gridSize + 1]);

int main(){

	bool grid[gridSize + 1][gridSize + 1] = {};
	for (int y = 0; y < gridSize + 1; y++)
	{
		for (int x = 0; x < gridSize + 1; x++)
		{
			int a = rand() % 2;
			if (a == 0) {
				grid[y][x] = false;
			}
			else {
				grid[y][x] = true;
			}
		}
	}

	while (true)
	{
		showGrid(grid);
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		getNextState(grid);
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(t2 - t1).count();
		cout << "Execution time: " << duration << " microseconds" << endl;
		getchar();
		system("CLS");
	}
}


void showGrid(bool grid[gridSize + 1][gridSize + 1]) {
	for (int y = 0; y < gridSize; y++)
	{
		for (int x = 0; x < gridSize; x++)
		{
			if (grid[y][x] == true) {
				cout << " O ";
			}
			else
			{
				cout << " . ";
			}
			if (x == gridSize - 1) {
				cout << endl;
			}
		}
	}
}

void getNextState(bool grid[gridSize + 1][gridSize + 1]) {
	bool gridCopy[gridSize + 1][gridSize + 1]{};

	int y, x = 0;
	for (int y = 0; y < gridSize + 1; y++)
	{
		for (int x = 0; x < gridSize + 1; x++)
		{
			gridCopy[y][x] = grid[y][x];
		}
	}

	static int moves[8][2] = { {-1,0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1} };
	for (int y = 0; y < gridSize + 1; y++) {
		for (int x = 0; x < gridSize + 1; x++) {

			int aliveNeighbours = 0;
			for (int l = 0; l < 8; ++l) {
				int ty = y + moves[l][0];
				int tx = x + moves[l][1];
				if (ty < 0 || ty >= gridSize || tx < 0 || tx >= gridSize) {
					continue;
				}

				if (grid[ty][tx] == true) {
					aliveNeighbours++;
				}
			}

			if (aliveNeighbours < 2)
				gridCopy[y][x] = false;

			else if (aliveNeighbours > 3)
				gridCopy[y][x] = false;

			else if (grid[y][x] == true && (aliveNeighbours == 2 || aliveNeighbours == 3))
				gridCopy[y][x] = true;

			else if (grid[y][x] == false && aliveNeighbours == 3)
				gridCopy[y][x] = true;

		}
	}

	for (int y = 0; y < gridSize; y++)
	{
		for (int x = 0; x < gridSize; x++)
		{
			grid[y][x] = gridCopy[y][x];
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

