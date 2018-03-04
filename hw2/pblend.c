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
//#define	FFMPEG	"/opt/ffmpeg/bin/ffmpeg"

typedef enum {ORDERED, STAGGERED, RANDOMIZED} modes;

int get_param(int * x, char ** arg);

int main(int argc, char * argv[]) {
	modes mode = ORDERED;
	int i = 0; // counter
	int child_count = 4; // number of children
	char filename[256];
	int rate = 24; // frame rate of movie
	int start = 1; // number of initial frame
	int end = 250; // number of final frame
	int frames; // count of frames
	char blend_out[256];

	// parameter parsing
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
				case 'r':
					mode = RANDOMIZED;
				break;
				case 't':
					mode = STAGGERED;
				break;
				case 'b':
					i++;
					strcpy(blend_out, argv[i]);
					printf("%s\n", blend_out);
				break;
				case 'c':
					i += get_param(&child_count,
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

	// number of frames needing rendered
	frames = end - start + 1;

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

	if (mode == ORDERED) {

		int pid;
		int this;
		int next;
		char start_s[32]; // string form of start frame index
		char end_s[32]; // string form of end frame index

		next = start - 1;

		// for each child blender process requested
		for (i = 0; i < child_count; i++) {
			// fork/exec
			pid = fork();
			if (pid == 0) { // if child
				this = next + 1;
				next = start + i * frames / child_count - 1;
				// convert to strings for arguments
				sprintf(start_s, "%d", start + i * frames / child_count);
				sprintf(end_s, "%d", start + (i+1) * frames / child_count - 1);
				// ouptut the command to stderr before running
				fprintf(stderr, "%s -t1 -b %s -s %s -e %s -a\n", BLENDER, filename, start_s, end_s);
				execl(BLENDER, "-t1", "-b", filename, "-s", start_s, "-e", end_s, "-a",
						(char *)NULL);

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

		// exec, no fork needed
		fprintf(stderr, "%s -r %s -start_number %s -i %s -vcodec mpeg4 output.mp4\n", FFMPEG, arg_rate, arg_start, blend_out);
		execl(FFMPEG, "ffmpeg", "-i", blend_out, "-r", arg_rate, "-start_number", arg_start,
				"-vcodec", "mpeg4", "output.mp4", (char *)NULL);
		fprintf(stderr, "error: `%s' is not a valid executable\n", FFMPEG);

		return 0;
	}

	int pid;
	for (i = 0; i < child_count; i++) {
		pid = fork();
	}

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
