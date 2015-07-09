#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(){
	int w_fifo,result;
	ssize_t size;
	char *myfifo = "/tmp/mfifo";
	char readf[1024], prov[4];
	unsigned int counter_0=0;

	result=(w_fifo=open(myfifo, O_WRONLY));

	if ( result == -1 ){
		printf("Server Offline...\n");
		return 0;
	}
	if ( result < 0 ){
		perror("w_fifo open");
		return 0;
	}

	while(prov[0] != 'e' && prov[1] != 'x' && prov[2] != 'i' && prov[3] != 't' ){
		fgets(readf, 1024, stdin);
		size = write(w_fifo, readf, strlen(readf)+1);
		if( size != (strlen(readf)+1) ){
			printf("Can't write all string to FIFO\n");
		}
		if (readf[0]=='e'){
			prov[0]=readf[0]; prov[1]=readf[1]; prov[2]=readf[2]; prov[3]=readf[3]; 
		}
	}

	close(w_fifo);
	return 0;
}
