#include "sub.h"
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
    } else {
        return -1;
    }
}

int commandPrint(int command, char key[], char value[], int success, char output[]) {

    memset(output, 0, strlen(output));
    int putOrGet = 0;
    int getOrDel = 0;

    char * deleted = "key_deleted";
    char * nonexistent = "key_nonexistent";
    char * put_failed = "put_failed";

    if (command == 0) {
        strcat(output, "PUT");
        putOrGet = 1;
    } else if (command == 1) {
        strcat(output, "GET");
        putOrGet = 1;
        getOrDel = 1;
    } else if (command == 2) {
        strcat(output, "DEL");
        getOrDel = 1;
    }

    strcat(output, ":");
    strcat(output, key);
    strcat(output, ":");

    if (success) {
        if (putOrGet) {
            strcat(output, value);
        }
        else{
            strcat(output,  deleted);
        }
    }
    else{
        if(getOrDel){
            strcat(output, nonexistent);
        }
        else{
            strcat(output, put_failed);
        }
    }
    strcat(output, "\n");
}
