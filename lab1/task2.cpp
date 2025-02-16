#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include "mpi.h" 
#define MAX 10 
using namespace std;

int main(int argc, char *argv[])
{
   int rank, size, n, i, ibeg, iend;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   printf ("process %d, pid: %d\n", rank, getpid());
   MPI_Finalize();
   return 0;
}