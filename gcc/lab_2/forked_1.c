//  File:       forked_1.c
//  Autor:      me1ga
//  Created:    16 июнь 2015 г. 15:00
//  Compile string:
//  1) 
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


#include "functions.c"

int main(int argc, char const *argv[]){

	mtrace();

	int w_fifo;
	unsigned int nfiles, i;
	char mm[argc], ichild = '-';

	nfiles = count_files(argc, argv, &mm[0]);

	pid_t pid[nfiles];
	char **myfifo = (char**)malloc(nfiles*sizeof(char*));
	char **path = (char**)malloc(nfiles*sizeof(char*));

	print_path(&mm[0], argv, path, argc, nfiles);

	init_fifos(myfifo, nfiles);

	for (i = 0; i < nfiles; i++){
		pid[i] = fork();
		if ( pid[i] == -1 ){
			printf("Error... #%d\n", i);
		}
		if ( pid[i] == 0 ){
			ichild = '+';
			open_write_close(w_fifo, myfifo[i], path[i]);
			break;
		}
	}

	if (ichild == '-'){
		open_read_close(myfifo, path, nfiles);
	}

	for (i = 0; i < nfiles; i++){
		if (ichild == '-'){
			unlink(myfifo[i]);
		}
		free(myfifo[i]);
		free(path[i]);
	}
	free(path);
	free(myfifo);
	return 0;
}
