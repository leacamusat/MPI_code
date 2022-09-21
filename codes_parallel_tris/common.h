#ifndef _COMMON_H
#define _COMMON_H

#include <errno.h>

//equivalent to a function that would do perror and exit
//The do /while is a trick so that the macro can be executed in all kind of context
//(e.g. in a if without brackets)
#define die_errno(msg) do { perror(msg); exit(errno); } while (0)

//errors messages
#define ALLOCATION_MEMORY_MESSAGE_ERROR "Allocation memory failed\n"
#define FPRINT_ERROR_MESSAGE "fprintf failed.\n"
#define FOPEN_ERROR_MESSAGE "fopen failed.\n"
#define MPI_SEND_MESSAGE_ERROR "MPI_Send failed\n"
#define MPI_RECV_MESSAGE_ERROR "MPI_Recv failed\n"
#define MPI_SENDRECV_MESSAGE_ERROR "MPI_Sendrecv\n"
#define MPI_REDUCE_MESSAGE_ERROR "MPI_Reduce failed\n"
#define MPI_SCATTER_MESSAGE_ERROR "MPI_Scatter failed\n"
#define MPI_GATHER_MESSAGE_ERROR "MPI_Gather failed\n"
#define MPI_INIT_MESSAGE_ERROR "Init MPI failed\n"
#define MPI_COMM_SIZE_MESSAGE_ERROR "MPI_Comm_size failed\n"
#define MPI_COMM_RANK_MESSAGE_ERROR "MPI_Comm_rank failed\n"
#define MPI_FINALIZE_MESSAGE_ERROR "MPI_Finalize failed.\n"

#define ROOT 0
#define N_ARRAY 4
#define MAX 500000

void print_vec(int * vec, int size);
void bubble_sort(int *arr, int len);
void fill_vec(int* vec, int size, int max);
int *fusion(int *vec1, int *vec2, int size1, int size2);
void calculate_result(int rank, int *merge, int n_proc, int tabsize, double start_time, char *filename);
int cmpfunc (const void * a, const void * b);

#endif
