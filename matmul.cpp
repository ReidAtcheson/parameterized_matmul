#include <iostream>
#include <chrono>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <cassert>
#include <mpi.h>
#define CACHE_ALIGNMENT 64

using namespace std;

void matmul(const float* __restrict__ A,const float* __restrict__ B,float* __restrict__ C,int m);


int main(int argc, char** argv){
  MPI_Init(&argc,&argv);
  if(argc<=1){cout << "Supply matrix dimensions" << endl; return 1;}
  int m=stoi(string(argv[1]),nullptr,10);


  float* A;
  float* B;
  float* C;
  int res;
  res=posix_memalign((void**)&A,CACHE_ALIGNMENT,m*m*sizeof(float));assert(res==0);
  res=posix_memalign((void**)&B,CACHE_ALIGNMENT,m*m*sizeof(float));assert(res==0);
  res=posix_memalign((void**)&C,CACHE_ALIGNMENT,m*m*sizeof(float));assert(res==0);
  



  for(int i=0;i<m*m;i++){A[i]=1.0;B[i]=1.0;C[i]=0.0;}
  matmul(A,B,C,m);
  for(int i=0;i<m*m;i++){A[i]=1.0;B[i]=1.0;C[i]=0.0;}


  auto start = std::chrono::steady_clock::now();
  matmul(A,B,C,m);
  auto finish = std::chrono::steady_clock::now();
  MPI_Barrier(MPI_COMM_WORLD);
  double elapsed=std::chrono::duration_cast<std::chrono::duration<double> >(finish-start).count();
  double redelapsed=0;
  MPI_Reduce(&elapsed,&redelapsed,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  if(rank==0)
    std::cout<<redelapsed<<"\n";

  for(int i=0;i<m*m;i++)assert(C[i]==1.0*m);



  free(A);
  free(B);
  free(C);

  MPI_Finalize();

}



void matmul(const float* __restrict__ A,const float* __restrict__ B,float* __restrict__ C,int m){

#define id(i,j)  ((i)*m + (j))
#define MIN(x,y) ( (x)<(y)?(x):(y) )



  for(int i=0;i<m;i+=TILE0)
  for(int j=0;j<m;j+=TILE1)
  for(int k=0;k<m;k+=TILE2)
  for(int ii=i;ii<MIN(i+TILE0,m);ii++)
  for(int kk=k;kk<MIN(k+TILE2,m);kk++)
  for(int jj=j;jj<MIN(j+TILE1,m);jj++)
    C[id(ii,jj)] += A[id(ii,kk)]*B[id(kk,jj)];

#undef id
#undef MIN

}



