//  File:       functions.c
//  Autor:      me1ga
//  Created:    16 июнь 2015 г. 15:00
//  Compile string:
//  1) 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define MAX_BUFFER 1024
#define FIFO_PATH_LENGTH 16

unsigned int c_sum(char *path){
	unsigned int sum=0;
	unsigned char ch;
	FILE *file;
	file=fopen(path, "r");
	while( !feof(file) ){
		ch=fgetc(file);
		if( ch > 47 && ch < 58 ){
			sum += ch-48;
		}
	}
	fclose(file);
	return sum;
}

unsigned int count_files(int num, char const **strings, char * mass){
	unsigned int i, count;
	char *pointer;
	mass[0]='0';
	count=0;
	for ( i = 1; i < num; i++ ){
		pointer = strstr(strings[i], ".txt");
		if ( pointer != NULL ){
			if ( (strncmp(pointer, ".txt\0", 5)) == 0 ){
				mass[i] = '1';
				count++;
			}
		}else{
			mass[i] = '0';
		}
	}
	return count;
}

unsigned int print_path(char *marker, char const **in, char **out, int max, unsigned int count){
	unsigned int ncount = 0, i;
	for ( i = 0; i < max; ++i){
		if ( ncount == count ){
			return 0;
		}
		if (marker[i] == '1'){
			out[ncount]=strdup(in[i]);
			ncount++;
		}
	}
	return EXIT_FAILURE;
}

unsigned int init_fifos(char **fifo, unsigned int count){
	char temp[FIFO_PATH_LENGTH];
	unsigned int i;
	for (i = 0; i < count; i++){
		sprintf(temp, "/tmp/myfifo.%d", i);
		fifo[i]=strdup(temp);
		unlink(fifo[i]);
		if ( (mkfifo(fifo[i], FILE_MODE))	 == -1 ){
			fprintf(stderr, "Невозможно создать FIFO #%d\n", i);
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

unsigned int open_write_close(int int_fifo, char* char_fifo, char* path){
	char buffer[MAX_BUFFER];
	ssize_t size;
	sprintf(buffer, "%d\n", c_sum(path));
	if ( (int_fifo=open(char_fifo, O_WRONLY)) < 0 ){
		perror("fifo open");
		return EXIT_FAILURE;
	}
	size = write(int_fifo, buffer, sizeof(char)*(strlen(buffer)+1));
	if( size != (strlen(buffer)+1) ){
		printf("Can't write all string to FIFO\n");
		return EXIT_FAILURE;
	}
	close(int_fifo);
	return EXIT_SUCCESS;
}

unsigned int open_read_close(char** char_fifo, char** path, unsigned int max){
	char buffer[MAX_BUFFER];
	ssize_t size;
	unsigned int i;
	int int_fifo;
	for (i = 0; i < max; i++){
		if ( (int_fifo=open(char_fifo[i], O_RDONLY)) < 0 ){
			perror("r_fifo open");
			return EXIT_FAILURE;
		}
		size = read(int_fifo, buffer, sizeof(buffer));
		if(size < 0){
			printf("Can't read string from FIFO\n");
			return EXIT_FAILURE;
		}
		fprintf(stdout, "CHILD #%d: Сумма всех цифр в файле %s = %s", i, path[i], buffer);
		close(int_fifo);
	}
	return EXIT_SUCCESS;
}
