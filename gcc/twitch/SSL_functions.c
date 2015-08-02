SSL_CTX* InitCTX(void){
    SSL_CTX *ctx;
 
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
	const SSL_METHOD *method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    if ( ctx == NULL ){
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
