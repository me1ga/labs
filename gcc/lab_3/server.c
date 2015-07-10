//  File:       server.c
//  Autor:      me1ga
//  Created:    10 июнь 2015 г.
//  Compile string:
//  1) 
#include <stdio.h>
//#include<stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>

#include "func.h"
//#include "func.c"

//#define CR_MSGQ ( 0660 | IPC_CREAT )

int main(/*int argc, char const *argv[]*/){
	/* Инициализация переменных */
	key_t token;
	int msgq_id;
	struct m_buf buffer;
	char command[5], 
		while_exit[5] = {'e', 'x', 'i', 't', 0},
		while_send[5] = {'s', 'e', 'n', 'd', 0},
		while_recv[5] = {'r', 'e', 'c', 'v', 0};

	/* Создание очереди */
	/*token = ftok(".", 'X');
	msgq_id = msgget(token, CR_MSGQ);
	if ( msgq_id == -1 ){
		perror("msgget");
		return 0;
	}*/

	/* Тело программы */
	do{
		fgets(command, 5, stdin);
		if((strncmp(command, while_send, 5)) == 0) printf("send message\n");
		if((strncmp(command, while_recv, 5)) == 0) printf("receive message\n");
	}while( (strncmp(command, while_exit, 5)) != 0 );

	/* Удаление очереди */
	//msgctl(msgq_id, IPC_RMID, 0);

	return 0;
}
