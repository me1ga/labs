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
#include "SOCKET_functions.c"
#include "PARSE_functions.c"

#define FAIL -1
#define h_addr h_addr_list[0]

int main(int argc, char const *argv[]){
	/* Инициализация переменных */
	int tcp_sock;
	char host[] = "api.twitch.tv";
	int port = 443;
	char message[]="GET https://api.twitch.tv/kraken/users/ws_mega HTTP/1.1\r\nHost: api.twitch.tv\r\nConnection: keep-alive\r\nAccept: application/vnd.twitchtv.v3+json\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n\r\n";
	char buffer[1024];
	SSL_CTX *ctx;
	SSL *ssl;

	SSL_library_init();
	/* Инициализация сокетов */
	ctx = InitCTX();
	tcp_sock = InitSOCKET(&host[0], port);
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
