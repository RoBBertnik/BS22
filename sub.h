#ifndef TEST3_SUB_H
#define TEST3_SUB_H
int readCommand(char input[],char key[],char value[]);
int commandPrint(int ClientSocket, int command, char key[], char value[], int success, char output[]);
#endif //TEST3_SUB_H
