/* 
 * File:   zag.h
 * Author: mega
 *
 * Created on 6 Июль 2015 г., 13:43
 */

#ifndef ZAG_H
#define	ZAG_H

#ifdef	__cplusplus
extern "C" {
#endif

unsigned int countFiles(FILE *file);
void readFiles(FILE *file, char **strMass, unsigned int nos);
int compare(char **a, char **b);
int sortFiles(char **strMass, unsigned int nThreads, unsigned int nos);
void writeFiles(FILE *file, char **strMass, unsigned int nos);
void usage();
void *pthread_qsort(void *args);

struct data{
    int count;
    char** mass;
};

#ifdef	__cplusplus
}
#endif

#endif	/* ZAG_H */

