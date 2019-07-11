import numpy as np
from mpi4py import MPI

gridSize = 10
gridChunk = int(gridSize/2)
p = 0.5
grid = np.random.choice(a=[False, True], size=(gridSize, gridSize), p=[p, 1 - p])


def showgrid(grid):
    rows = grid.shape[0]
    cols = grid.shape[1]
    for y in range(0, rows):
        for x in range(0, cols):
            if grid[x, y]:
                print(" 0 ", end="")
            else:
                print(" . ", end="")
        print("")


def main():

    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    print(rank)
    moves = [[-1, 0], [-1, -1], [0, -1], [1, -1], [1, 0], [1, 1], [0, 1], [-1, 1]]
    sub1 = grid[:int(gridChunk) + 1, :int(gridChunk) + 1]
    sub2 = grid[:int(gridChunk) + 1, int(gridChunk) - 1:]
    sub3 = grid[int(gridChunk) - 1:, int(gridChunk) - 1:]
    sub4 = grid[int(gridChunk) - 1:, :int(gridChunk) + 1]

    if rank == 0:
        showgrid(grid)
        comm.send(sub1, dest=1, tag=11)
        comm.send(sub2, dest=2, tag=12)
        comm.send(sub3, dest=3, tag=13)
        comm.send(sub4, dest=4, tag=14)

    elif rank == 1:
        sub1 = np.empty(shape=(gridChunk + 1, gridChunk + 1))
        comm.recv(sub1, source=0, tag=11)
        localCopy = np.empty(shape=(gridChunk + 1, gridChunk + 1))
        for y in range(0, gridChunk):
            for x in range(0, gridChunk + 1):
                aliveNeighbours = 0
                for l in range(0, 8):
                    ty = y + moves[l][0]
                    tx = x + moves[l][1]
                    if ty < 0 or ty >= gridChunk or tx < 0 or tx >= gridChunk:
                        continue

                    if sub1[ty][tx]:
                        aliveNeighbours += 1
                print(aliveNeighbours, y, x, sub1[y][x])
                if aliveNeighbours < 2:
                    localCopy[y][x] = False

                elif aliveNeighbours > 3:
                    localCopy[y][x] = False

                elif sub1[y][x] and (aliveNeighbours == 2 or aliveNeighbours == 3):
                    localCopy[y][x] = True

                elif not sub1[y][x] and aliveNeighbours == 3:
                    localCopy[y][x] = True

        sub1 = localCopy
        showgrid(sub1)

    elif rank == 2:
        sub2 = np.empty(shape=(gridChunk + 1, gridChunk + 1))
        comm.recv(sub2, source=0, tag=12)
        localCopy = np.empty(shape=(gridChunk + 1, gridChunk + 1))
        for y in range(0, gridChunk):
            for x in range(0, gridChunk + 1):
                aliveNeighbours = 0
                for l in range(0, 8):
                    ty = y + moves[l][0]
                    tx = x + moves[l][1]
                    if ty < 0 or ty >= gridChunk or tx < 0 or tx >= gridChunk:
                        continue

                    if sub2[ty][tx]:
                        aliveNeighbours += 1
                print(aliveNeighbours, y, x, sub2[y][x])
                if aliveNeighbours < 2:
                    localCopy[y][x] = False

                elif aliveNeighbours > 3:
                    localCopy[y][x] = False

                elif sub2[y][x] and (aliveNeighbours == 2 or aliveNeighbours == 3):
                    localCopy[y][x] = True

                elif not sub2[y][x] and aliveNeighbours == 3:
                    localCopy[y][x] = True

        sub1 = localCopy
        showgrid(sub1)

    elif rank == 3:
        sub3 = np.empty(shape=(gridChunk + 1, gridChunk + 1))
        comm.recv(sub3, source=0, tag=13)
        localCopy = np.empty(shape=(gridChunk + 1, gridChunk + 1))
        for y in range(0, gridChunk):
            for x in range(0, gridChunk + 1):
                aliveNeighbours = 0
                for l in range(0, 8):
                    ty = y + moves[l][0]
                    tx = x + moves[l][1]
                    if ty < 0 or ty >= gridChunk or tx < 0 or tx >= gridChunk:
                        continue

                    if sub3[ty][tx]:
                        aliveNeighbours += 1
                print(aliveNeighbours, y, x, sub3[y][x])
                if aliveNeighbours < 2:
                    localCopy[y][x] = False

                elif aliveNeighbours > 3:
                    localCopy[y][x] = False

                elif sub3[y][x] and (aliveNeighbours == 2 or aliveNeighbours == 3):
                    localCopy[y][x] = True

                elif not sub3[y][x] and aliveNeighbours == 3:
                    localCopy[y][x] = True

        sub1 = localCopy
        showgrid(sub1)

    elif rank == 4:
        sub4 = np.empty(shape=(gridChunk + 1, gridChunk + 1))
        comm.recv(sub4, source=0, tag=14)
        localCopy = np.empty(shape=(gridChunk + 1, gridChunk + 1))
        for y in range(0, gridChunk):
            for x in range(0, gridChunk + 1):
                aliveNeighbours = 0
                for l in range(0, 8):
                    ty = y + moves[l][0]
                    tx = x + moves[l][1]
                    if ty < 0 or ty >= gridChunk or tx < 0 or tx >= gridChunk:
                        continue

                    if sub4[ty][tx]:
                        aliveNeighbours += 1
                print(aliveNeighbours, y, x, sub4[y][x])
                if aliveNeighbours < 2:
                    localCopy[y][x] = False

                elif aliveNeighbours > 3:
                    localCopy[y][x] = False

                elif sub4[y][x] and (aliveNeighbours == 2 or aliveNeighbours == 3):
                    localCopy[y][x] = True

                elif not sub4[y][x] and aliveNeighbours == 3:
                    localCopy[y][x] = True

        sub1 = localCopy
        showgrid(sub1)


if __name__ == "__main__":
    main()
