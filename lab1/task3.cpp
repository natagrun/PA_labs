#include <cstdlib>
#include <iostream>
#include "mpi.h" 
#define MAX 10 
using namespace std;

int main(int argc, char *argv[])
{
   int rank, size, n, i,j, ibeg, iend;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   n=(MAX-1)/size+1;
   ibeg=rank*n+1;
   iend=(rank+1)*n;
   for(j=1; j<=10; j++){
    printf ("process %d, %d ^ %d = %d \n", rank, rank, j,rank*j);}
    printf ("\n");
   MPI_Finalize();
   return 0;
}