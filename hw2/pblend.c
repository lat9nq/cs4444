#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define	BLENDER	"/usr/bin/blender"

typedef enum {ORDERED, STAGGERED, RANDOMIZED} modes;

int get_param(int * x, char ** arg);

int main(int argc, char * argv[]) {
	modes mode = ORDERED;
	int i;
	int child_count;
	char filename[256];
	int rate;
	int start;
	int end;
	int frames;

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
				case 'r':
					mode = RANDOMIZED;
				break;
				case 't':
					mode = STAGGERED;
				break;
				case 'c':
					i += get_param(&child_count, &(argv[i]));
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

	frames = end - start + 1;

	if (filename[0] != '/') {
		const char * pwd = getenv("PWD");
		char temp[256];
		strcpy(temp, "/net");
		strcat(temp, pwd);
		strcat(temp, "/");
		strcat(temp, filename);
		strcpy(filename, temp);
	}

	int children[child_count];

	if (mode == ORDERED) {

		int pid;
		char start_s[32];
		char end_s[32];
		for (i = 0; i < child_count; i++) {
			pid = fork();
			sprintf(start_s, "%d", start + i * frames / child_count);
			sprintf(end_s, "%d", start + (i+1) * frames / child_count - 1);
			if (pid == 0) {
				fprintf(stderr, "%s -b %s -s %s -e %s -a\n", BLENDER, filename, start_s, end_s);
				execl(BLENDER, "-b", filename, "-s", start_s, "-e", end_s, "-t", "1", "-o", "./out/##.png", "-x", "PNG", "-a",
						(char *)NULL);

				fprintf(stderr, "error: `%s' is not a valid executable\n", BLENDER);
				exit(0);
			}
			else {
				children[i] = pid;
			}
		}

		for (i = 0; i < child_count; i++) {
			waitpid(children[i], NULL, 0);
		}

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
