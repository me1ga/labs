int InitSOCKET(char * host, int port){
    int tcp_sock;
    struct sockaddr_in tcp_addr;
    struct hostent *hst;
    tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock < 0){
        perror("tcp_sock = socket");
        exit(EXIT_FAILURE);
    }
    bzero(&tcp_addr, sizeof(tcp_addr));
    hst = gethostbyname(host);
    if (NULL == hst){
        perror("gethostbyname");
        exit(EXIT_FAILURE);
    }
    printf("HOST: %s\n", hst->h_name);
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_port = htons(port);
    tcp_addr.sin_addr.s_addr = *(long *)hst->h_addr;
    if( (connect(tcp_sock, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr))) < 0 ){
        perror("connect");
        exit(EXIT_FAILURE);
    }
    return tcp_sock;
}