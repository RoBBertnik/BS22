#include "sub.h"
#include "KeyValueStore.h"
#include "MessageQueues.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>




int readCommand(char input[], char key[], char value[]) {

    memset(key, 0, strlen(key));
    memset(value, 0, strlen(value));

    printf("gehe in Methode \n");
    char input2[strlen(input)];
    strcpy(input2, input);

    char *token = strtok(input2, " ");
    if (token == NULL) {
        return 0;
    }
    char command[strlen(token)];
    strcpy(command, token);

    if (strcmp(command, "QUIT") == 0) {
        return 3;
    }

    int getOrDel = 0;
    int counter = 0;
    if (strcmp(command, "GET") == 0 || strcmp(command, "DEL") == 0) {
        getOrDel = 1;
    }

    while (token != NULL) {
        token = strtok(NULL, " ");
        if (token != NULL) {
            if (counter == 0) {
                strcpy(key, token);

            }
            if (counter == 1 && !getOrDel) {
                strcpy(value, token);
            }
            counter++;
        }

    }
    if (strcmp(command, "PUT") == 0) {
        return 0;
    } else if (strcmp(command, "GET") == 0) {
        return 1;
    } else if (strcmp(command, "DEL") == 0) {
        return 2;
    } else if (strcmp(command, "QUIT") == 0){
        return 3;
    } else if (strcmp(command, "BEG") == 0){
        return 4;
    } else if (strcmp(command, "END") == 0){
        return 5;
    } else if (strcmp(command, "SUB") == 0){
        return 6;
    } else {
        return -1;
    }
}

void commandPrint(int ClientSocket, int command, char key[], char value[], int success, char output[]) {

    memset(output, 0, strlen(output));
    int del = 0;
    int put = 0;

    char * deleted = "key_deleted";
    char * nonexistent = "key_nonexistent";
    char * put_failed = "put_failed";

    if (command == 0) {
        strcat(output, "PUT");
        put = 1;
    } else if (command == 1) {
        strcat(output, "GET");
    } else if (command == 2) {
        strcat(output, "DEL");
        del = 1;
    } else if (command == 6){
        strcat(output, "SUB");
    }

    strcat(output, ":");
    strcat(output, key);
    strcat(output, ":");

    if (success) {
        if (del) {
            strcat(output,  deleted);
        }
        else{
            strcat(output, value);
        }
    }
    else{
        if(put){
            strcat(output, put_failed);
        }
        else{
            strcat(output, nonexistent);
        }
    }
    strcat(output, "\n");

    if(put || del){
        notifySubscribers(key,output);
    }

    write(ClientSocket, output, strlen(output));
}

