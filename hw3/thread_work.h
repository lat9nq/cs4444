#ifndef THREAD_WORK_H
#define THREAD_WORK_H

#include <pthread.h>

int hotSpotRow;
int hotSptCol;
#define hotSpotTemp 1000

int iterations;
int num_rows;
int num_cols;
int threads;

typedef struct {
	float *** plate;
	int whom, rows;
	pthread_barrier_t * bar;
} row_arg_t;

typedef struct {
} checker_arg_t;

void * thread_work(void * a);
void * thread_work_less_looping(void * a);
void * thread_work_checkerboard(void * a);

#endif
