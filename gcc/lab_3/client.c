//  File:       client.c
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

#include "structures.h"
#include "func.c"

int main(int argc, char const *argv[]){
	/* Инициализация переменных */
	key_t token;
	int msgq_id=0, sel=0, result=0;
	struct m_buf buffer;
	char buf[4096];
	char command[5], 
		while_exit[5] = {'e', 'x', 'i', 't', 0},
		while_send[5] = {'s', 'e', 'n', 'd', 0},
		while_recv[5] = {'r', 'e', 'c', 'v', 0};

	/* Открытие очереди */
	token = ftok("sss", 1);
	msgq_id = msgget(token, 0);
	if ( msgq_id == -1 ){
		perror("msgget");
		return 0;
	}

	/* Тело программы */
	do{
		printf("Ввдите команду: ");
		fgets(command, 5, stdin);
		fgets(buf, 4096, stdin);
		if((strncmp(command, while_send, 5)) == 0) sel=1;
		if((strncmp(command, while_recv, 5)) == 0) sel=2;
		if((strncmp(command, while_exit, 5)) == 0) sel=3;
		if(sel == 0) sel=4;

		switch(sel){
			case 1:
				printf("Отправить сообщение: \n");
				fgets(buffer.data.message, 1024, stdin);
				buffer.mtype=1;
				result = send_message(msgq_id, &buffer);
				if (result == -1){
					perror("\nsend_message");
					sel=3;
				}else{
					sel=0;
				}
			break;
			case 2:
				result = recv_message(msgq_id, 0, &buffer);
				if (result == -1){
					perror("\nrecv_message");
					sel=0;
				}else{
					sel=0;
				}
				printf("Входящее сообщение: %s", buffer.data.message);
			break;
			case 3:
			break;
			case 4:
				printf("Неправильная команда.\n");
			break;
		}
	}while( sel != 3 );

	return 0;
}
