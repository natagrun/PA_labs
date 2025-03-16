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
                matrix[i][j] = (rand() % 21) - 10; // Генерация чисел от -10 до 10
                std::cout << matrix[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    // Определяем количество строк для каждого процесса
    int *send_counts = new int[size]; // Количество строк для каждого процесса
    int *displs = new int[size];      // Смещения для каждого процесса

    int rows_per_process = ROWS / size;
    int remainder = ROWS % size;

    for (int i = 0; i < size; i++)
    {
        send_counts[i] = (i < remainder) ? (rows_per_process + 1) * COLUMNS : rows_per_process * COLUMNS;
        displs[i] = (i == 0) ? 0 : displs[i - 1] + send_counts[i - 1];
    }

    // Количество строк, которые обрабатывает текущий процесс
    int local_rows = (rank < remainder) ? rows_per_process + 1 : rows_per_process;
    int local_elements = local_rows * COLUMNS;

    // Локальный буфер для хранения строк, обрабатываемых текущим процессом
    int *local_matrix = new int[local_elements];

    // Распределение строк матрицы с использованием MPI_Scatterv
    MPI_Scatterv(matrix, send_counts, displs, MPI_INT, local_matrix, local_elements, MPI_INT, 0, MPI_COMM_WORLD);

    // Обработка локальной части матрицы
    for (int i = 0; i < local_rows; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            int index = i * COLUMNS + j;
            if (local_matrix[index] > 0)
            {
                local_matrix[index] = 1;
            }
            else if (local_matrix[index] < 0)
            {
                local_matrix[index] = -1;
            }
        }
    }

    // Сбор обработанных данных обратно в процесс с рангом 0
    MPI_Gatherv(local_matrix, local_elements, MPI_INT, modified_matrix, send_counts, displs, MPI_INT, 0, MPI_COMM_WORLD);

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
    }

    // Освобождение памяти
    delete[] send_counts;
    delete[] displs;
    delete[] local_matrix;

    MPI_Finalize();
    return 0;
}