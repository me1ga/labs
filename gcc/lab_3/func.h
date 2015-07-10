//  File:       func.h
//  Autor:      me1ga
//  Created:    10 июнь 2015 г.
//  Compile string:
//  1) 
#ifndef FUNC_H
#define FUNC_H

#ifdef  __cplusplus
extern "C" {
#endif

struct m_data{
	char login[8];
//	char password[8];
	char message[1024];
};
struct m_buf{
	long mtype; /* Тип сообщения */
	struct m_data data; /* Пользовательские данные */
};

#ifdef  __cplusplus
}
#endif

#endif  /* FUNC_H */
