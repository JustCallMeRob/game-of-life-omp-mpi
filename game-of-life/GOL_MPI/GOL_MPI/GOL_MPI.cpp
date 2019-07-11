// GOL_MPI_S.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>
#include <iostream>


using namespace std;
const int gridsize = 10;
const int num_elements_per_process = 9;
void showGrid(int *grid, int gridsize);
void showRow(int *row, int size);
int *getNextRowOfNeighbours(int *grid, int z);

// Creates an array of random numbers. Each number has a value of either 0 or 1
int *create_rand_nums() {
	int num_elements = gridsize * gridsize;
	int *rand_nums = (int *)malloc(sizeof(int) * num_elements);
	assert(rand_nums != NULL);
	for (int i = 0; i < num_elements; i++) {
		rand_nums[i] = (rand() % 2);
	}
	return rand_nums;
}

// Returns the cell state for a vector of neighbours
int get_cell_state(int *row_of_neighbours) {
	int cell_state = 0;
	int aliveNeighbours = 0;
	for (int x = 0; x < num_elements_per_process - 1; x++) {
		aliveNeighbours = aliveNeighbours + row_of_neighbours[x];
	}

	if (aliveNeighbours < 2)
		cell_state = 0;

	else if (aliveNeighbours > 3)
		cell_state = 0;

	else if (row_of_neighbours[num_elements_per_process - 1] == 1 && (aliveNeighbours == 2 || aliveNeighbours == 3))
		cell_state = 1;

	else if (row_of_neighbours[num_elements_per_process - 1] == 0 && aliveNeighbours == 3)
		cell_state = 1;
	return cell_state;
}


int main(int argc, char** argv) {

	// Create initial grid
	int *initial_grid = NULL;
	initial_grid = create_rand_nums();
	showGrid(initial_grid, gridsize);
	int counter = 0;

	int num_elements = gridsize * gridsize;
	int *next_itteration = (int *)malloc(sizeof(int) * num_elements);
	assert(rand_nums != NULL);

	MPI_Init(NULL, NULL);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int row_divisor = gridsize / world_size;

	while (true) {
		// For every row in the grid, scatter the neighbours of each element in the row to all the processes for state determination
		for (int y = 0; y < gridsize; y++) {

			// Get next row of neighbours
			int *row_of_neighbours = NULL;
			row_of_neighbours = getNextRowOfNeighbours(initial_grid, y);

			for (int q = 0; q < row_divisor; q++) {

				// For each process, create a buffer that will hold a subset of the current row of neighbours
				int *sub_new_array = (int *)malloc(sizeof(int) * num_elements_per_process / row_divisor);
				assert(sub_new_array != NULL);

				int *current_row_of_neighbours = NULL;
				for (int x = 0; x < gridsize / row_divisor; x++) {
					current_row_of_neighbours[x] = row_of_neighbours[x + ((gridsize/row_divisor) * q)];
				}

				// Scatter the neighbours of each cell in the row from the root process to all processes
				MPI_Scatter(current_row_of_neighbours, num_elements_per_process, MPI_INT, sub_new_array, num_elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);

				counter++;
				//cout << counter << " process nr: " << world_rank << " received ";
				//showRow(sub_new_array, num_elements_per_process);

				// Figure out the cell states
				int cell = get_cell_state(sub_new_array);

				// Gather all cell states down to root process
				int *processed_subgrids = NULL;
				if (world_rank == 0) {
					processed_subgrids = (int *)malloc(sizeof(int) * world_size);
					assert(cell_states != NULL);
				}
				MPI_Gather(&cell, 1, MPI_INT, processed_subgrids, 1, MPI_INT, 0, MPI_COMM_WORLD);

				if (world_rank == 0) {
					//cout << "processed row nr: " << y;
					//showRow(processed_subgrids, gridsize);
					//cout << endl;
					int x = 0;
					for (int z = (y * gridsize); z < ((y * gridsize) + gridsize) / row_divisor; z++) {
						next_itteration[z] = processed_subgrids[x++ + ((gridsize / row_divisor) * q)];
					}

					//If we finished all the rows in the grid, print out the result and make the initial grid equal to the result for the next itteration
					if ((counter * row_divisor) % gridsize == 0) {
						showGrid(next_itteration, gridsize);
						//Sleep(100);
						system("cmd /c cls");
						for (int i = 0; i < gridsize * gridsize; i++) {
							initial_grid[i] = next_itteration[i];
						}
					}
				}

				// Dealocated memory
				if (world_rank == 0) {
					free(processed_subgrids);
					free(sub_new_array);
				}
				MPI_Barrier(MPI_COMM_WORLD);
			}
		}
	}
	MPI_Finalize();
}


void showGrid(int *grid, int size) {
	for (int y = 0; y < size * size; y++)
	{
		if (y % size == 0) {
			cout << endl;
		}
		if (grid[y] == 1) {
			cout << " O ";
		}
		else
		{
			cout << " . ";
		}
		if (y == (size * size) - 1) {
			cout << endl;
		}
	}
}

void showRow(int *row, int size) {
	for (int y = 0; y < size; y++)
	{
		if (row[y] == 1) {
			cout << " O ";
		}
		else
		{
			cout << " . ";
		}
		if (y == size - 1) {
			cout << endl;
		}
	}
}

int *getNextRowOfNeighbours(int *grid, int z) {
	int num_elements = gridsize * 9;
	int *neighbours = (int *)malloc(sizeof(int) * num_elements);
	static short int legal_neighbours[8] = { {-gridsize - 1}, {-gridsize}, {-gridsize + 1}, {-1}, {+1}, {gridsize - 1}, {gridsize}, {gridsize + 1} };
	static short int legal_neighbours_edges_right[8] = { {-gridsize - 1}, {-gridsize}, {gridsize * gridsize}, {-1}, {gridsize * gridsize}, {gridsize - 1}, {gridsize}, {gridsize * gridsize} };
	static short int legal_neighbours_edges_left[8] = { {gridsize * gridsize}, {-gridsize}, {-gridsize + 1}, {gridsize * gridsize}, {+1}, {gridsize * gridsize}, {gridsize}, {gridsize + 1} };
	int y = 0;
	int tx;

	for (int x = z * gridsize; x < z * gridsize + gridsize; x++) {
		if (x % gridsize == 0) {
			for (int l = 0; l < 8; ++l) {
				tx = x + legal_neighbours_edges_left[l];
				if ((tx < 0) || (tx >= (gridsize * gridsize))) {
					neighbours[y++] = 0;
					continue;
				}
				neighbours[y++] = grid[tx];
			}
		}
		else if ((x + 1) % gridsize == 0) {
			for (int l = 0; l < 8; ++l) {
				tx = x + legal_neighbours_edges_right[l];
				if ((tx < 0) || (tx >= (gridsize * gridsize))) {
					neighbours[y++] = 0;
					continue;
				}
				neighbours[y++] = grid[tx];
			}
		}
		else {
			for (int l = 0; l < 8; ++l) {
				tx = x + legal_neighbours[l];
				if ((tx < 0) || (tx >= (gridsize * gridsize))) {
					neighbours[y++] = 0;
					continue;
				}
				neighbours[y++] = grid[tx];
			}
		}
		neighbours[y++] = grid[x];
	}
	return neighbours;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
