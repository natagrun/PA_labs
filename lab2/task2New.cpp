#include <cstdlib>
#include <iostream>
#include <ctime>
#include "mpi.h" 
using namespace std;

int main(int argc, char *argv[])
{
    int rank, size; 
   MPI_Status status;
   int n= 20;
   int vector[n];
   srand(time(NULL));
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   int local_max=0, max=0;
   if (rank==0){
    for (int i=0;i<n;i++){
      vector[i]= (rand() % 100);
      printf("%d ",vector[i]);
    }
    printf("\n");
   }
   MPI_Bcast(&vector,n,MPI_INT,0,MPI_COMM_WORLD);
   for (int i=rank;i<n;i=i+size){
    if(vector[i]> local_max){
      local_max = vector[i];
    }
  }
   MPI_Reduce(&local_max,&max,1,MPI_INT,MPI_MAX,0, MPI_COMM_WORLD);
   if(rank==0){
      printf("max = %d\n",max);
   }
    MPI_Finalize();
    return 0;

}