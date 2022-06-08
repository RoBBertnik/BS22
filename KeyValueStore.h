#ifndef TEST3_KEYVALUESTORE_H
#define TEST3_KEYVALUESTORE_H


int put(char key[], char value[]);
int get(char key[], char res[]);
int del(char key[]);
void initSharedMemory();
void detachSharedMemory();

int beg();
int end();
int sub(char key[]);

void pub(char key[]);

int initSemaphore();
void deleteSemaphore();
void initializeMessageQueue();
void CloseMessageQueue();
void notifySubscribers(char key[], char *input);

#endif //TEST3_KEYVALUESTORE_H
