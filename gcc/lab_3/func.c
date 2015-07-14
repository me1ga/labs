//  File:       func.c
//  Autor:      me1ga
//  Created:    10 июнь 2015 г.
//  Compile string:
//  1) gcc -c threadqsort.c
//  2) gcc threadqsort.o -L. -lmylabsort -o lab_4 -pthread
//  libmylabsort.so in libraries required !
// #include <string.h>

#include "func.h"
#include "structures.h"

int send_message(int qid, struct m_buf *buffer){
	int result, length;
	length = sizeof(struct m_buf) - sizeof(long);
	result = msgsnd(qid, buffer, length, 0);
	if (result == -1){
		return -1;
	}
	return result;
}

int recv_message(int qid, long type, struct m_buf *buffer){
	int result, length;
	length = sizeof(struct m_buf) - sizeof(long);
	result = msgrcv(qid, buffer, length, type, IPC_NOWAIT);
	if(result == -1){
		return -1;
	}
	return result;
}

