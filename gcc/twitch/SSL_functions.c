//  File:       functions.c
//  Autor:      me1ga
//  Created:    22 июнь 2015 г. 19:06
//  Compile string:
//  1)
 
SSL_CTX* InitCTX(void){
    SSL_CTX *ctx;
 
    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
	const SSL_METHOD *method = SSLv23_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
