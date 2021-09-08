#include <stdio.h>
#include <mpi.h>
#include <iostream>
#define np 8
using namespace std;

int main(int argc, char *argv[])
{
  int npes, myrank, i, j;
  int a[np+2][np+2];
  int left, right;
  MPI_Status stat;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &npes);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
 
  for(j =0; j<= np+1; j++){
	  for(i=0; i<= np+1; i++)
		  a[i][j] = myrank*100+10*j+i;
  }
  
  left = myrank - 1;
  right= myrank + 1;
  
  if(myrank == 1 ) left = npes -1 ;
  if(myrank == (npes-1)) right = 1;
  
if (myrank != 0 ) {
	  if(myrank%2==0){
		MPI_Send(&a[1][0],np+2,MPI_INT,left,1,MPI_COMM_WORLD);
		MPI_Send(&a[np][0],np+2,MPI_INT,right,1,MPI_COMM_WORLD);
		MPI_Recv(&a[0][0],np+2,MPI_INT,left,1,MPI_COMM_WORLD,&stat);
		MPI_Recv(&a[np+1][0],np+2,MPI_INT,right,1,MPI_COMM_WORLD,&stat);
	  }else{
		MPI_Recv(&a[np+1][0],np+2,MPI_INT,right,1,MPI_COMM_WORLD,&stat);
		MPI_Recv(&a[0][0],np+2,MPI_INT,left,1,MPI_COMM_WORLD,&stat);
 		MPI_Send(&a[np][0],np+2,MPI_INT,right,1,MPI_COMM_WORLD);
		MPI_Send(&a[1][0],np+2,MPI_INT,left,1,MPI_COMM_WORLD);
	  }
}

if (myrank!= 0) {
  MPI_Send(&a[0][0],(np+2)*(np+2),MPI_INT,0,1,MPI_COMM_WORLD);
}
for (int z=1; z<npes; z++){
  MPI_Recv(&a[0][0],(np+2)*(np+2),MPI_INT,z,1,MPI_COMM_WORLD,&stat);

  cout<<" This is Master printing data from process: "<<z<<'\n';
  for(j=0; j<=np+1; j++){
    for(i=0; i<=np+1; i++){
      cout<<a[i][j]<<" ";
    }
    cout<<'\n';       
  }
}

  MPI_Finalize();
  return 0;
}