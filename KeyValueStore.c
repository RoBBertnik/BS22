#include <stdio.h>
#include "KeyValueStore.h"
#include <string.h>
#include "KeyAndValue.h"

struct KeyAndValue database[1000];


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
