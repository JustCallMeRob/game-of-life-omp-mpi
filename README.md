# Game of Life Parallelized

## Two parallelizations for the game of life

### One with a shared memory approach using [Open Multi-Processing (OMP)](https://www.openmp.org/)
### One with a message passing approach using [Message Passing Interface (MPI)](https://www.mcs.anl.gov/research/projects/mpi/) written both in C++ and Python 

###[What is the game of life ?](https://en.wikipedia.org/wiki/Conway's_Game_of_Life)

<img src="https://media.giphy.com/media/Y3Y7AHKxr8ccP2Difm/giphy.gif" width="500" height="500" />

In order to execute the .py file successfully your python interpreter is required to have both the numpy and mpi4py packages installed.
Run the following command in the directory of the GOL_MPI.py file:
```
mpirun -n 4 python GOL_MPI.py
```
Where the -n parameter tells MPI to use the number of provided processes for execution.
