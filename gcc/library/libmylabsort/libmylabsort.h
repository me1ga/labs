/* 
 * File:   libmylabsort.h
 * Author: me1ga
 *
 * Created on 10 Июль 2015 г., 15:13
 */

#ifndef LIBMYLABSORT_H
#define LIBMYLABSORT_H

#ifdef  __cplusplus
extern "C" {
#endif

unsigned int countFiles(FILE *file);
void readFiles(FILE *file, char **strMass, unsigned int nos);
int compare(char **a, char **b);
int sortFiles(char **strMass, unsigned int nThreads, unsigned int nos);
void writeFiles(FILE *file, char **strMass, unsigned int nos);
void *pthread_qsort(void *args);

struct data{
    int count;
    char** mass;
};

#ifdef  __cplusplus
}
#endif

#endif  /* LIBMYLABSORT_H */

