//	File: 		libmylabsort.c
//	Autor: 		me1ga
//	Created: 	10 июнь 2015 г.
//	Compile string:
//	1) gcc -fPIC -c libmylabsort.c -pthread
//	2) gcc -shared -o libmylabsort.so libmylabsort.o
//	export LD_LIBRARY_PATH
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strdup();
#include <pthread.h>

#include "libmylabsort.h"

unsigned int countFiles(FILE *file){
    char temp[1024];
    unsigned int counter=0;
    
    while ( fgets(temp,sizeof(temp),file) != NULL ){
        counter++;
    }
    rewind(file);   //fseek(file, 0, SEEK_SET);
    
    return counter;
}

void readFiles(FILE *file, char **strMass, unsigned int nos){
    char temp[1024];
    unsigned int counter;
    
    for( counter=0; counter<nos; counter++ ){
        if( fgets(temp,sizeof(temp),file) == NULL ){
            break;
        }
        strMass[counter]=strdup(temp);
    }
}

int compare(char **a, char **b){
    return strcmp(*a,*b);
}

int sortFiles(char **strMass, unsigned int nThreads, unsigned int nos){
    unsigned int i,counter;
    unsigned int part;
    unsigned int ost;
    struct data tData[nThreads];
    int rc;
    pthread_t thread[nThreads];
    
    part=nos/nThreads;
    ost=nos%nThreads;
    
    for( i=0; i<nThreads; i++ ){
        if( i+1==nThreads && ost > 0){
            tData[i].count=part+ost;
        }else{
            tData[i].count=part;
        }
        tData[i].mass = (char**)malloc(tData[i].count*sizeof(char*));
        for(counter=0; counter<tData[i].count; counter++){
            tData[i].mass[counter]=strMass[i*part+counter];
        }
    }
    
    for( i=0; i<nThreads; i++ ){
        rc = pthread_create(&thread[i], NULL, pthread_qsort, (void*)&tData[i]);
        if(rc != 0){
            printf("Ошибка создания потока #%i\n", i);
            return -1;
        }
    }
    for( i=0; i<nThreads; i++ ){
        rc = pthread_join(thread[i], NULL);
        if(rc != 0){
            perror("Joining the # thread");
            return -1;
        }
    }
    
    for(i=0; i<nThreads; i++){
        for(counter=0; counter<tData[i].count; counter++){
            strMass[i*part+counter]=tData[i].mass[counter];
        }
        free(tData[i].mass);
    }
}

void *pthread_qsort(void *args){
    struct data *tData = (struct data *)args;
    qsort(tData->mass, tData->count, sizeof(char *), compare);
    return NULL;
}

void writeFiles(FILE *file, char **strMass, unsigned int nos){
    unsigned int counter;
    for(counter=0; counter<nos; counter++){
        fprintf(file, "%s", strMass[counter]);
        free(strMass[counter]);
    }
}