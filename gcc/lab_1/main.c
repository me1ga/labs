#include<stdio.h>
#include<stdlib.h>
#include<string.h> //strdup();
#include<getopt.h>
#include<pthread.h>
#include<time.h>

#include "zag.h" 

int main(int argc, char **argv){
    struct option loptions[]={
        {"in",      required_argument,  NULL,'i'},
        {"out",     required_argument,  NULL,'o'},
        {"threads", required_argument,  NULL,'t'},
        {"help",    no_argument,        NULL,'h'},
        {0,0,0,0}
    };
    int errflag=0,opt,helpflag=0;
    char *infile = NULL;
    char *outfile = NULL;
    int threadnum = 1;
    
    while((opt = getopt_long(argc, argv, "i:o:t:", loptions, NULL)) != -1){
        switch(opt){
            case 'i': infile = optarg; break;
            case 'o': outfile = optarg; break;
            case 't': threadnum = atoi(optarg); break;
            case 'h': helpflag = 1; break;
            case '0': break;
            case ':': errflag = 1; break;
            case '?': errflag = 1; break;
        }
    }
    
    if (infile == NULL) {
        errflag = 1;
        printf("Отсутствует путь входного файла!\n");
    }
    if (outfile == NULL) {
        errflag = 1;
        printf("Отсутствует путь выходного файла!\n");
    }
    if (errflag == 1 || helpflag == 1) {
        usage();
        return errflag;
    }
    if( threadnum<1 || threadnum>10 ){
        errflag = 1;
        usage();
        return errflag;
    }
    
    FILE *input,*output;
    unsigned int numberOfStrings=0;
    
    input=fopen(infile, "r");
    if(input == NULL || (output = fopen(outfile, "w")) == NULL) {
        perror("fopen");
        return -1;
    }
    
    numberOfStrings = countFiles(input);
    
    if( numberOfStrings == 0 ) return -1;
    char **strings = (char**)malloc(sizeof(char*)*numberOfStrings);
    
    readFiles(input, strings, numberOfStrings);
    sortFiles(strings, threadnum, numberOfStrings);
    writeFiles(output, strings, numberOfStrings);
    
    free(strings);
    fclose(input);
    fclose(output);
    
    //printf("%u\n", clock()*1000/CLOCKS_PER_SEC);
    return 0;
}

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

void usage(){
    printf("usage: lab1 \t[-i file | --in file] \n\t\t[-o file | --out file] \n\t\t[-t nthreads | --threads nthreads][default : 1] \n\t\t[--help]\n");
}