#include <stdio.h>
#include <sys/msg.h>

#define PAYLOAD_LENGTH 100
struct Message{
    long header;
    char payload[PAYLOAD_LENGTH];
}Message;



int msID;

struct Message *messages;

void initializeMessageQueue(){
    msID = msgget(IPC_PRIVATE,IPC_CREAT | 0777);
}

void CloseMessageQueue(){
    msgctl(msID,IPC_RMID, NULL);
}

void notifySubscribers(char key[], int pID){
    msgsnd(msID, &messages, PAYLOAD_LENGTH, 0);
    msgrcv(msID, &messages, PAYLOAD_LENGTH, pID, 0);
}
