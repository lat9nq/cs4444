#include "thread_work.h"

void * thread_work(void * a) {
	row_arg_t * args = (row_arg_t *)a;

	float *** plate = args->plate;
	int i, x, y;
	int which_plate = 1;
	int last_plate = 0;

	int rows = args->rows;
	int start_at = rows * args->whom + 1;

	for (i = 0; i < iterations; i++) {
		for (y = start_at; y <= rows + start_at - 1; y++) {
			for (x = 1; x <= num_cols; x++) {
				plate[which_plate][y][x] = (plate[last_plate][y-1][x]
						+ plate[last_plate][y+1][x]
						+ plate[last_plate][y][x-1]
						+ plate[last_plate][y][x+1]) * 0.25;
			}
		}

		plate[which_plate][hotSpotRow][hotSptCol] = hotSpotTemp;

		last_plate = which_plate;
		which_plate = !which_plate;

		pthread_barrier_wait(args->bar);
	}
	return NULL;
}

void * thread_work_less_looping(void * a) {
	row_arg_t * args = (row_arg_t *)a;

	float *** plate = args->plate;
	int i, x, y;

	int rows = args->rows;
	int start_at = rows * args->whom + 1;

	for (i = 0; i < iterations / 2; i++) {
		for (y = start_at; y <= rows + start_at - 1; y++) {
			for (x = 1; x <= num_cols; x++) {
				plate[1][y][x] = (plate[0][y-1][x]
						+ plate[0][y+1][x]
						+ plate[0][y][x-1]
						+ plate[0][y][x+1]) * 0.25;
			}
		}

		plate[1][hotSpotRow][hotSptCol] = hotSpotTemp;

		for (y = start_at; y <= rows + start_at - 1; y++) {
			for (x = 1; x <= num_cols; x++) {
				plate[0][y][x] = (plate[1][y-1][x]
						+ plate[1][y+1][x]
						+ plate[1][y][x-1]
						+ plate[1][y][x+1]) * 0.25;
			}
		}

		plate[0][hotSpotRow][hotSptCol] = hotSpotTemp;

		pthread_barrier_wait(args->bar);
	}
	return NULL;
}
