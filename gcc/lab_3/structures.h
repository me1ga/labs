//  File:       structures.h
//  Autor:      me1ga
//  Created:    14 июнь 2015 г. 11:00
//  Compile string:
//  1) 
#ifndef STRUCTURES_H
#define STRUCTURES_H

#ifdef  __cplusplus
extern "C" {
#endif

struct m_data{
	char message[1024];
};
struct m_buf{
	long mtype; /* Тип сообщения */
	struct m_data data; /* Пользовательские данные */
};

#ifdef  __cplusplus
}
#endif

#endif  /* STRUCTURES_H */
