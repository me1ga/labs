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

int send_message(int qid, struct m_buf *buffer);
int recv_message(int qid, long type, struct m_buf *buffer);

#ifdef  __cplusplus
}
#endif

#endif  /* FUNC_H */
