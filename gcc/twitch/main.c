//  File:       server.c
//  Autor:      me1ga
//  Created:    22 июнь 2015 г. 19:03
//  Compile string:
//  1) make
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <resolv.h>

#include "SSL_functions.c"

#define FAIL -1
#define h_addr h_addr_list[0]

int main(int argc, char const *argv[]){
	/* Инициализация переменных */
	int tcp_sock;
	struct sockaddr_in tcp_addr;
	struct hostent *hst;
	char message[]="GET https://api.twitch.tv/kraken/users/ws_mega HTTP/1.1\r\nHost: api.twitch.tv\r\nConnection: keep-alive\r\nAccept: application/vnd.twitchtv.v3+json\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n\r\n";
	char buffer[1024];
	SSL_CTX *ctx;
	SSL *ssl;

	SSL_library_init();
	/* Инициализация сокетов */
	ctx = InitCTX();
	tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_sock < 0){
		perror("tcp_sock = socket");
		exit(EXIT_FAILURE);
	}
	bzero(&tcp_addr, sizeof(tcp_addr));
	hst = gethostbyname("api.twitch.tv");
	printf("HOST: %s\n", hst->h_name);
	tcp_addr.sin_family = AF_INET;
	tcp_addr.sin_port = htons(443);
	tcp_addr.sin_addr.s_addr = *(long *)hst->h_addr;
	if( (connect(tcp_sock, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr))) < 0 ){
		perror("connect");
		exit(EXIT_FAILURE);
	}
	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, tcp_sock);

	/* Тело программы */
	if( (SSL_connect(ssl)) == FAIL ){
		ERR_print_errors_fp(stderr);
	}
	SSL_write(ssl, message, sizeof(message));
	SSL_read(ssl, buffer, sizeof(buffer));
	printf("%s", buffer);

	/* Высвобождение переменных */
	SSL_free(ssl);
	close(tcp_sock);
	SSL_CTX_free(ctx);
	return 0;
}
