#include <stdio.h>
#include "KeyValueStore.h"
#include "MessageQueues.h"
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>



struct KeyAndValue{
    char key[100];
    char value[100];
} KeyAndValue;

struct Subscriptions{
    char key[100];
    int pID;
} Subscriptions;




int shID;
int subID;
int semID;
unsigned short marker[1];

struct KeyAndValue *database;
struct Subscriptions *subscribers;



int put(char key[], char value[]) {
    if(value[0] == '\0'){
        return 0;
    }
    int counter = 0;
    while(database[counter].key[0] != '\0') {
        if (strcmp(database[counter].key, key) == 0) {
            strcpy(database[counter].value, value);
            return 1;
        }
        counter++;
        if (counter == 1000) {
            return 0;
        }
    }

    strcpy(database[counter].key, key);
    strcpy(database[counter].value, value);
    database[counter+1].key[0] = '\0';
    database[counter+1].value[0] = '\0';
    return 1;

}



int get(char key[], char res[]){

    memset(res, 0, strlen(res));

    int counter = 0;
    while(database[counter].key[0] != '\0') {
        if (strcmp(database[counter].key, key) == 0) {
            strcpy(res, database[counter].value);
            return 1;
        }
        counter++;
        if (counter == 1000) {
            return 0;
        }
    }
    return 0;
}

int del(char key[]){
    int counter = 0;
    while(database[counter].key[0] != '\0'){
        if(strcmp(database[counter].key, key) == 0){
            memset(database[counter].key, 0, strlen(database[counter].key));
            for(int i = counter; counter < 1000; i++){
                if(strcmp(database[i + 1].key, "\0") == 0){
                    break;
                }
                database[i] = database[i + 1];
            }
            return 1;
        }
        counter++;
    }
    return 0;
}





void initSharedMemory(){
    shID = shmget(IPC_PRIVATE, 100 * sizeof(struct KeyAndValue), IPC_CREAT | 0777);
    database = shmat(shID,NULL,0);
    subID = shmget(IPC_PRIVATE, 100 * sizeof(struct Subscriptions), IPC_CREAT | 0777);
    subscribers = shmat(subID, NULL, 0);
}

void detachSharedMemory(){
    shmdt(database);
    shmctl(shID,IPC_RMID,NULL);
    shmdt(subscribers);
    shmctl(subID, IPC_RMID, NULL);
}

int initSemaphore(){
    semID = semget(IPC_PRIVATE, 1, IPC_CREAT | 0777);
    if(semID == -1){
        perror("Could not create Semaphore");
    }
    marker[0] = 1;
    semctl(semID,1,SETALL,marker);
}

void deleteSemaphore(){
    semctl(semID, 0, IPC_RMID);
}

int status;

int beg(){
    status = 1;
    return 0;
}
int end(){
    status = 0;
    return 0;
}

int sub(char key[], int ClientSocket){
    int counter = 0;
    while(database[counter].key[0] != '\0'){
        if(strcmp(database[counter].key, key) == 0){
            int secondCounter = 0;
            while(subscribers[secondCounter].pID != '\0'){
                secondCounter++;
            }
            subscribers[secondCounter].pID = ClientSocket;
            strcpy(subscribers[secondCounter].key, key);
            printf("Client subscribed to %s\n", key);
            return 1;
        }
        counter++;
    }
    return 0;
}

void pub(char key[]){
    int counter = 0;
    while(subscribers[counter].key[0] != '\0'){
        if(strcmp(subscribers[counter].key, key) == 0 && subscribers[counter].pID != getpid()){
            notifySubscribers(key, subscribers[counter].pID);
        }
        counter++;
    }
}


