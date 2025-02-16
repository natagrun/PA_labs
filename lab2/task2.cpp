//3.	Максимальный элемент;

#include <cstdlib>
#include <iostream>
#include <ctime>
#include "mpi.h" 
#define MAX 10 
using namespace std;

int main(int argc, char *argv[])
{
    const int tag = 198;
   int rank, size, n, i, ibeg, iend, ai,bi,ci,max,temp;
   MPI_Status status;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(rank* time(NULL));
   ai = (rand() % 100) + 1;
    printf("rank = %d, a%d = %d\n", rank,rank,ai);
max=ai;
    if (rank==0){
        for (i=1;i<size;i++){
            MPI_Recv(&temp,1,MPI_INT,i,tag,MPI_COMM_WORLD,&status);
            max = max>temp?max:temp;
        }
        printf("max = %d\n",max);
    }else{
        MPI_Send(&ai,1,MPI_INT,0,tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
    //printf("max = %d",max);
}

// #include <stdio.h>
// #include "mpi.h"
// #include <vector>
// #include <cstdlib>
// #include <iostream>

// int main(int argc, char **argv){
//     double global_sum =0 , local_sum =0;
//     int rank,size;
//     int k;
//     srand(time(NULL));
//     int vector_size = 10;
//     std::vector<int> global_vector(vector_size);
//     MPI_Status status;
//     for(int i=0;i<vector_size;i++){
//         global_vector[i]=rand()%10;
//         std:: cout<<" "<<global_vector[i];
//     }
//     std:: cout<<"\n ";
//     MPI_Init(&argc, &argv);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);
//     for( int i =rank;i<vector_size;i=i+size){
//         local_sum+=global_vector[i];
//     }
//     std:: cout<<"local:"<<local_sum<<"\n ";
//     if (rank ==0){
//         global_sum+=local_sum;
//         for(int i=1;i<size;i++){
//             MPI_Recv(&local_sum,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD,&status);
//             global_sum+=local_sum;
//         }
//         std:: cout<<"Averahge:"<<global_sum/vector_size<<"\n ";
//     }else{
//         MPI_Send(&local_sum,1,MPI_DOUBLE,0,0, MPI_COMM_WORLD);
//     }
//     MPI_Finalize();
// }