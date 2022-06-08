
#ifndef TEST3_MESSAGEQUEUES_H
#define TEST3_MESSAGEQUEUES_H

#define TEXT_LENGTH 100
#define SUBSCRIPTION_SIZE 100

typedef struct Subscriptions_{
    char key[SUBSCRIPTION_SIZE];
    int pID;
}Subscriptions;

typedef struct Message_{
    long msg_type;
    char msg_text[TEXT_LENGTH];
}Message;

extern int msID;

#endif //TEST3_MESSAGEQUEUES_H
