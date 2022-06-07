
#ifndef TEST3_MESSAGEQUEUES_H
#define TEST3_MESSAGEQUEUES_H

void initializeMessageQueue();
void CloseMessageQueue();
void notifySubscribers(char key[], int pID);
void sendMessages();

#endif //TEST3_MESSAGEQUEUES_H
