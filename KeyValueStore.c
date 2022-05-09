#include <stdio.h>
#include "KeyValueStore.h"
#include <string.h>


struct KeyAndValue{
    char key[100];
    char value[100];
} KeyAndValue;

struct KeyAndValue database[1000];

int put(char key[], char value[]) {
    int counter = 0;
    while(database[counter].key[0] != '\0') {
        if (strcmp(database[counter].key, key) == 0) {
            strcpy(database[counter].value, value);
            return 1;
        }
        counter++;
        if (counter == 1000) {
            return -1;
        }
    }

    strcpy(database[counter].key, key);
    strcpy(database[counter].value, value);
    database[counter+1].key[0] = '\0';
    database[counter+1].value[0] = '\0';
    return 0;

}



int get(char key[], char res[]){
    int counter = 0;
    while(database[counter].key[0] != '\0') {
        if (strcmp(database[counter].key, key) == 0) {
            strcpy(res, database[counter].value);
            return 0;
        }
        counter++;
        if (counter == 1000) {
            return -1;
        }
    }
    return -1;
}

int del(char key[]){
    int counter = 0;
    while(database[counter].key[0] != '\0'){
        counter++;
        if(strcmp(database[counter].key, key) == 0){
            strcpy(database[counter].key, '\0');
            for(int i = counter; counter < 1000; i++){
                database[i] = database[i + 1];
            }
        }
    }
    return 0;
}
