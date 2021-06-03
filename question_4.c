#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>
#include <string.h>
/* Global variables */
char depot[] = "";
int k = 0;
sem_t cons;
sem_t prod;

void produire(char* message){
    k++;
    printf("Message : %d", k);
}

void consommer(char* message){
    char *chain = message;
    /*while (*chain){
        *chain = toupper(*chain);
        *chain++;        
    }*/
    message += toupper(*chain);
    printf("%s\n", message);
}


void deposer(char* message){
    strcpy(depot, message);
    
}


void retirer(char* message) {
    strcpy(message, depot);
    strcpy(depot, "");
    
}

/* Thread function */
void *producteur(void *arg){
    
    char message[255];
    int i;
    
    for (i = 0; i < 20; i++){
        sem_wait(&prod);
        produire(message);
        deposer(message);
        sem_post(&cons);
    }
    
}

void *consommateur(void *arg){
    char message[255];
    int i;
    
    for (i = 0; i < 20; i++){
        sem_wait(&cons);
        consommer(message);
        retirer(message);
        sem_post(&prod);
        
    }
}

void main () {
    
    pthread_t prod_thread, cons_thread;
    
    /* semaphore m should be initialized by 1 */
    if (sem_init(&prod, 0, 1) == -1) {
        perror("Could not initialize cons semaphore");
        exit(2);
    }
    if (sem_init(&cons, 1, 0) == -1) {
        perror("Could not initialize prod semaphore");
        exit(2);
    }
    /* create TEN threads */
    if (pthread_create(&prod_thread, NULL, producteur, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }
    
    if (pthread_create(&cons_thread, NULL, consommateur, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }
    
    /*wait for created thread to terminate */
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);
    
    sem_destroy(&prod);
    sem_destroy(&cons);
    
    
    exit(0);
}
