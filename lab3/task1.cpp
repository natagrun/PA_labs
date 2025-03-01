#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "mpi.h"

#define ROWS 5
#define COLUMNS 6

int main(int argc, char **argv)
{
  int rank, size;
  int unchanged_count = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int matrix[ROWS][COLUMNS];
  int modified_matrix[ROWS][COLUMNS];

  if (rank == 0)
  {
    srand(time(NULL));
    std::cout << "Old matrix:\n";
    for (int i = 0; i < ROWS; i++)
    {
      for (int j = 0; j < COLUMNS; j++)
      {
        matrix[i][j] = (rand() % 21) - 10;
        std::cout << matrix[i][j] << " ";
      }
      std::cout << "\n";
    }
  }

  for (int i = 0; i < ROWS; ++i)
  {
    MPI_Bcast(matrix[i], COLUMNS, MPI_INT, 0, MPI_COMM_WORLD);
  }

  for (int i = 0; i < ROWS; i++)
  {
    for (int j = 0; j < COLUMNS; j++)
    {
      modified_matrix[i][j] = matrix[i][j];
    }
  }

  int local_unchanged_count = 0;
  for (int i = rank; i < ROWS; i = i + size)
  {
    for (int j = 0; j < COLUMNS; j++)
    {
      if (matrix[i][j] == 0)
      {
        local_unchanged_count++;
      }
      if (modified_matrix[i][j] > 0)
      {
        if (matrix[i][j] == 1)
        {
          local_unchanged_count++;
        }
        modified_matrix[i][j] = 1;
      }
      else if (modified_matrix[i][j] < 0)
      {
        if (matrix[i][j] == -1)
        {
          local_unchanged_count++;
        }
        modified_matrix[i][j] = -1;
      }
    }
  }

  for (int i = 0; i < ROWS; ++i)
  {
    MPI_Bcast(modified_matrix[i], COLUMNS, MPI_INT, i % size, MPI_COMM_WORLD);
  }

  int global_unchanged_count = 0;
  MPI_Reduce(&local_unchanged_count, &global_unchanged_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    std::cout << "\nNew matrix:\n";
    for (int i = 0; i < ROWS; i++)
    {
      for (int j = 0; j < COLUMNS; j++)
      {
        std::cout << modified_matrix[i][j] << " ";
      }
      std::cout << "\n";
    }
    std::cout << "\nNumber of unchanged elements: " << global_unchanged_count << std::endl;
  }

  MPI_Finalize();
  return 0;
}