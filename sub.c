#include "sub.h"
#include <stdio.h>
#include <string.h>
int readCommand(char input[],char key[],char value[]){
    printf("gehe in Methode \n");
    char input2[50];
    strcpy(input2,input);
    // Extract the first token
    char * token = strtok(input2, " ");
    char command[50];
    strcpy(command,token);
    int counter = 0;
    // loop through the string to extract all other tokens
    while( token != NULL ) {
        //printf( " %s\n", token ); //printing each token
        token = strtok(NULL, " ");
        if(counter == 0){
            strcpy(key,token);
        }
        if(counter == 1){
            strcpy(value,token);
        }
        counter++;

    }
    return 0;

}