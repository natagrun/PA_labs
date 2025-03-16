#include <cstdlib>
#include <iostream>
#include <ctime>
#include "mpi.h" 
using namespace std;

int main(int argc, char *argv[])
{
    int rank, size; 
   MPI_Status status;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   int ai=0, max=0;
   srand(rank* time(NULL));
   ai = (rand() % 100) + 1;
   printf("rank = %d, a%d = %d\n", rank,rank,ai);
   MPI_Reduce(&ai,&max,1,MPI_INT,MPI_MAX,0, MPI_COMM_WORLD);
   if(rank==0){
     printf("max = %d\n",max);
   }
    MPI_Finalize();
    return 0;

}