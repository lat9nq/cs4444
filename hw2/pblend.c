/* pblend.c
 * C
 *
 * Louis Thomas
 * Prof. Andrew Grimshaw
 * Intro to Parallel Computing
 * 1 March 2018
 *
 * This is a simple C program that runs Blender in parallel. After which,
 * the program finishes with a call to ffmpeg, stitching together all the
 * rendered frames into a single movie.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define	BLENDER	"/usr/bin/blender"
#define	FFMPEG	"/usr/bin/ffmpeg"
#define CHILDREN_PER_NODE	20
//#define	FFMPEG	"/opt/ffmpeg/bin/ffmpeg"

typedef enum {ORDERED, STAGGERED, RANDOMIZED} modes;

int get_param(int * x, char ** arg);

int main(int argc, char * argv[]) {
	modes mode = ORDERED;
	int i = 0; // counter
	int child_count = 4; // number of children
	int total_children = 0;
	char filename[256];
	int rate = 24; // frame rate of movie
	int start = 1; // number of initial frame
	int end = 250; // number of final frame
	int frames; // count of frames
	char blend_out[256];
	strcpy(blend_out, "%04d.png");

	// parameter parsing
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
				case 'r':
					mode = RANDOMIZED;
				break;
				case 'j':
					mode = STAGGERED;
				break;
				case 'b':
					i++;
					strcpy(blend_out, argv[i]);
					//printf("%s\n", blend_out);
				break;
				case 'c':
					i += get_param(&child_count,
							&(argv[i]));
				break;
				case 't':
					i += get_param(&total_children,
							&(argv[i]));
				break;
				case 'f':
					i += get_param(&rate, &(argv[i]));
				break;
				case 's':
					i += get_param(&start, &(argv[i]));
				break;
				case 'e':
					i += get_param(&end, &(argv[i]));
				break;
			}
		}
		else {
			strcpy(filename, argv[i]);
		}
	}

	if (total_children < child_count)
		total_children = child_count;

	// number of frames needing rendered
	frames = end - start + 1;

	int pid;
	/*
	int allocations = child_count / CHILDREN_PER_NODE + 1;

	for (i = 0; i < allocations; i++) {
		pid = fork();
		if (!pid) {
			execl("salloc", "-w", "granger[4]", "-t", "1:00:00", "-c", "20", "--output=\"job.out\"", "--error=\"job.err\"", (char *) NULL);
			fprintf(stderr, "error: salloc is not a valid executable\n");
			exit(1);
		}
		int status;
		waitpid(pid, &status, 0);
		if (status) {
			fprintf(stderr, "error: salloc failed\n");
			fprintf(stderr, "stopping...\n");
			exit(0);
		}
	}
	*/

	// get the working directory to append to the front of directories
	const char * pwd = getenv("PWD");
	char temp[256];
	if (filename[0] != '/') {
		temp[0] = 0;
		strcat(temp, pwd);
		strcat(temp, "/");
		strcat(temp, filename);
		strcpy(filename, temp);
	}

	// array of all the children
	int children[child_count];

	int this = 0;
	int next = 0;
	int stagger = 0;
	char start_s[32]; // string form of start frame index
	char end_s[32]; // string form of end frame index
	char stagger_s[32];

	if (mode == ORDERED)
		next = start - 1;
	else if (mode == STAGGERED)
		stagger = total_children;

	time_t init = time(NULL);

	// for each child blender process requested
	for (i = 0; i < child_count; i++) {
		if (mode == ORDERED) {
			this = next + 1;
			next = start + (i+1) * frames / child_count - 1;
		}
		else if (mode == STAGGERED) {
			this = start + i * total_children / child_count;
			if (i >= end)
				break;
		}
		// fork/exec
		pid = fork();
		if (pid == 0) { // if child
			// convert to strings for arguments
			sprintf(start_s, "%d", this);
			sprintf(end_s, "%d", next);
			sprintf(stagger_s, "%d", stagger);
			// ouptut the command to stderr before running
			if (mode == ORDERED) {
				fprintf(stderr, "%s -t1 -b %s -s %s -e %s -a\n", BLENDER, filename, start_s, end_s);
				execl(BLENDER, "-t1", "-b", filename, "-s", start_s, "-e", end_s, "-a", (char *)NULL);
			}
			else if (mode == STAGGERED) {
				fprintf(stderr, "%s -t1 -b %s -s %s -j %s -a\n", BLENDER, filename, start_s, stagger_s);
				execl(BLENDER, "-t1", "-b", filename, "-s", start_s, "-j", stagger_s, "-a", (char *)NULL);
			}

			// if we're here, something went wrong!
			fprintf(stderr, "error: `%s' is not a valid executable\n", BLENDER);
			exit(1);
		}
		else {
			// if parent, remember who the child is
			children[i] = pid;
		}
	}

	// wait for all the children to finish
	int err; // error code
	for (i = 0; i < child_count; i++) {
		waitpid(children[i], &err, 0);
		if (err) { // on error, stop
			fprintf(stderr, "error: child %d exited with status code %d\n", children[i], err);
			exit(0);
		}
		else
			fprintf(stderr, "child %d finished!\n", children[i]);
	}

	time_t render = time(NULL);

	char arg_rate[8]; // string of frame rate
	char arg_start[8]; // string of start frame

	// append working directory to file names
	if (blend_out[0] != '/') {
		strcpy(temp, pwd);
		strcat(temp, "/");
		strcat(temp, blend_out);
		strcpy(blend_out, temp);
	}

	// convert numbers to strings
	sprintf(arg_rate, "%d", rate);
	sprintf(arg_start, "%d", start);

	pid = fork();

	if (pid == 0) {
		// exec
		fprintf(stderr, "%s -r %s -start_number %s -i %s -vcodec mpeg4 output.mp4\n", FFMPEG, arg_rate, arg_start, blend_out);
		execl(FFMPEG, "ffmpeg", "-y", "-i", blend_out, "-r", arg_rate, "-start_number", arg_start,
				"-vcodec", "mpeg4", "output.mp4", (char *)NULL);
		fprintf(stderr, "error: `%s' is not a valid executable\n", FFMPEG);
		exit(0);
	}
	waitpid(pid, NULL, 0);

	time_t total = time(NULL);

	fprintf(stderr, "time at init: %ld\n", init);
	fprintf(stderr, "time after rendering: %ld (+%lds)\n", render, render - init);
	fprintf(stderr, "time now: %ld (+%lds)\n", total, total - init);

	return 0;
}

int get_param(int * x, char ** arg) {
	int skip = 0;
	if (arg[0][2] == '\0') {
		skip ++;
		*x = atoi(arg[1]);
	}
	else {
		*x = atoi(arg[0]+2);
	}
	return skip;
}
