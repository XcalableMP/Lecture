#include <xmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define N    1024
double A[N][N]:[*], B[N][N]:[*], C[N][N]:[*];  // c = a x b
struct timeval start_t, end_t;

void init_dmat(){
  srand48(0);

  for(int i=0;i<N;i++)
    for(int j=0;j<N;j++){
      A[i][j] = -1 * drand48() + 0.5;
      B[i][j] = -1 * drand48() + 0.5;
    }
}

double verify(){
  double sum = 0.0;

  for(int i=0;i<N;i++)
    for(int j=0;j<N;j++)
      sum += C[i][j];

  return sum;
}

void print_mat(){
  printf("A:\n");
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
      printf(" %.3f ", A[i][j]);
    }
    printf("\n");
  }

  printf("B:\n");
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
      printf(" %.3f ", B[i][j]);
    }
    printf("\n");
  }

  printf("C:\n");
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
      printf(" %.3f ", C[i][j]);
    }
    printf("\n");
  }
}

void start_timer(){
  gettimeofday(&start_t, NULL);
}

double end_timer(){
  gettimeofday(&end_t, NULL);
  return (end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)*1.0E-6;
}

void mul_dmat(int start_i, int end_i, int start_k, int end_k){
  int i, j, k;

  for(i=0;i<N;i++)
    for(j=0;j<N;j++)
      C[i][j] = 0.0;

  for(i=start_i;i<end_i;i++)
    for(j=0;j<N;j++)
      for(k=start_k;k<end_k;k++)
	C[i][k] += A[i][j] * B[j][k];
}

void gather_data(){
  if(xmpc_this_image() == 0){
    C[0:N/2][N/2:N/2] = C[0:N/2][N/2:N/2]:[1];
    C[N/2:N/2][0:N/2] = C[N/2:N/2][0:N/2]:[2];
    C[N/2:N/2][N/2:N/2] = C[N/2:N/2][N/2:N/2]:[3];
  }
}

void move_data(){
  if(xmpc_this_image() == 0){
    // Please implement THIS
  }
}

int main(){
  double time;
  int me = xmpc_this_image();

  if(xmp_num_nodes() != 4){
    fprintf(stderr, "This program must be executed by 4 nodes.");
    exit(1);
  }

  // Serial Implementation
  if(me == 0){
    init_dmat();            // matrix initialization

    start_timer();
    mul_dmat(0, N, 0, N);     // matrix multiply   

    time = end_timer();

    printf("Serial: \n");
    printf("  Time = %f sec\n", time);
    printf("  verify : %.3f\n", verify());
  }
  // End Serial Implementation

  // Parallel Implementation
  if(me == 0)
    init_dmat();

  xmp_sync_all(NULL);
  start_timer();
  move_data();               // transports data
  xmp_sync_all(NULL);

  if(me == 0){
    mul_dmat(0, N/2, 0, N/2);
  }
  else if(me == 1){
    mul_dmat(0, N/2, N/2, N);
  }
  else if(me == 2){
    mul_dmat(N/2, N, 0, N/2);
  } 
  else if(me == 3){
    mul_dmat(N/2, N, N/2, N);
  }
  xmp_sync_all(NULL);

  gather_data();
  // END Parallel Implementation

  xmp_sync_all(NULL);
  time = end_timer();

  if(me == 0){
    printf("\nParallel:\n");
    printf("  Time = %f sec\n", time);
    printf("  verify : %.3f\n", verify());
  }

  return 0;
}
