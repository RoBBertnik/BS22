#include "sub.h"
#include <stdio.h>
#include <string.h>

int readCommand(char input[], char key[], char value[]) {
    key = "";
    value = "";

    printf("gehe in Methode \n");
    char input2[50];
    strcpy(input2, input);
    // Extract the first token
    char *token = strtok(input2, " ");
    if (token == NULL) {
        return -2;
    }
    char command[50];
    strcpy(command, token);
    int counter = 0;
    // loop through the string to extract all other tokens
    while (token != NULL) {
        token = strtok(NULL, " ");
        if (token != NULL) {
            if (counter == 0) {
                strcpy(key, token);
            }
            if (counter == 1) {
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
    } else if(strcmp(command, "EXIT") == 0) {
        return 3;
    } else {
        return -1;
    }
}

int commandPrint(int command, char key[], char value[],char output[]) {


}
