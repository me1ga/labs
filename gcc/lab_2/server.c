#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(){
	int r_fifo,result;
	ssize_t size;
	char *myfifo = "/tmp/mfifo";
	char buffer[1024], prov[4];

	unlink(myfifo);
	if ( (mkfifo(myfifo, 0666))	 == -1 ){
		fprintf(stderr, "Невозможно создать FIFO\n");
		return 0;
	}

	if ( (r_fifo=open(myfifo, O_RDONLY)) < 0 ){
		perror("r_fifo open");
		return 0;
	}

	while(prov[0] != 'e' && prov[1] != 'x' && prov[2] != 'i' && prov[3] != 't' ){
		size = read(r_fifo, buffer, sizeof(buffer));
		if(size < 0){
			printf("Can't read string from FIFO\n");
			return 0;
		}
		if (buffer[0]=='e'){
			prov[0]=buffer[0]; prov[1]=buffer[1]; prov[2]=buffer[2]; prov[3]=buffer[3]; 
		}
		printf("%s", buffer);
	}

	close(r_fifo);
	unlink(myfifo);
	return 0;
}