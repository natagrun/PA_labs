
#include <cstdlib>
#include <iostream>
#include <ctime>
#include "mpi.h" 
#define MAX 10 
using namespace std;

int main(int argc, char *argv[])
{
    
    const int tag = 198;
   int rank, size, n, i, ibeg, iend, ai,bi,ci;
   MPI_Status status;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
srand(rank);
   ai = (rand() % 100) + 1;
   bi = (rand() % 100) + 1;

    printf("rank = %d, a%d = %d, b%d = %d\n", rank,rank,ai,rank,bi);
    printf("Sending: a%d = %d, b%d = %d\n",rank,ai,rank,bi);
    switch (rank){
        case 0:
        {
        MPI_Send(&bi, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
        MPI_Send(&ai, 1, MPI_INT, 2, tag, MPI_COMM_WORLD);
        MPI_Recv(&ai, 1, MPI_INT, 1, tag, MPI_COMM_WORLD,&status);
        MPI_Recv(&bi, 1, MPI_INT, 2, tag, MPI_COMM_WORLD,&status);
        break;
        }
        case 1:
        {
        MPI_Send(&ai, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        MPI_Send(&bi, 1, MPI_INT, 3, tag, MPI_COMM_WORLD);
        MPI_Recv(&ai, 1, MPI_INT, 3, tag, MPI_COMM_WORLD,&status);
        MPI_Recv(&bi, 1, MPI_INT, 0, tag, MPI_COMM_WORLD,&status);
        break;
        }
        case 2:
        {
        MPI_Send(&ai, 1, MPI_INT, 3, tag, MPI_COMM_WORLD);
        MPI_Send(&bi, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        MPI_Recv(&ai, 1, MPI_INT, 0, tag, MPI_COMM_WORLD,&status);
        MPI_Recv(&bi, 1, MPI_INT, 3, tag, MPI_COMM_WORLD,&status);
        break;
        }
        case 3:
        {
        MPI_Send(&ai, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
        MPI_Send(&bi, 1, MPI_INT, 2, tag, MPI_COMM_WORLD);
        MPI_Recv(&ai, 1, MPI_INT, 2, tag, MPI_COMM_WORLD,&status);
        MPI_Recv(&bi, 1, MPI_INT, 1, tag, MPI_COMM_WORLD,&status);
        break;
        }
    }
    printf("Received: a = %d, b = %d\n",ai,bi);
    printf("c%d = %d\n\n",rank,ai+bi);
    MPI_Finalize();
    return 0;
}