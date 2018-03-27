#ifndef THREAD_WORK_H
#define THREAD_WORK_H

#include <pthread.h>

int hotSpotRow;
int hotSptCol;
#define hotSpotTemp 1000

typedef struct {
	float *** plate;
	int whom, rows, cols;
	pthread_barrier_t * bar;
	int iterations;
} row_arg_t;

typedef struct {
} checker_arg_t;

void * thread_work_basic(void * a);
void * thread_work_less_looping(void * a);
void * thread_work_checkerboard(void * a);

#endif
