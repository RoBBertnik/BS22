#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "KeyValueStore.h"
#include "sub.h"
#include <sys/shm.h>
#include <sys/sem.h>


#include <arpa/inet.h>
#include <string.h>


#define ITERATION 1
#define PORT 5678




//--------------------------------------------------- Server with Socket ----------------------------------------------------------

int main() {
    //Für Semaphore und UP/DOWN Operationen
    struct sembuf enter, leave;
    enter.sem_num = leave.sem_num = 0;
    enter.sem_flg = leave.sem_flg = SEM_UNDO;
    enter.sem_op = -1;
    leave.sem_op = 1;

    struct sockaddr_in server;
    socklen_t client_len;

    struct sockaddr_in clientAddr;

    //Client socket
    int ClientSocket;

    //Child process id
    pid_t childpid;

    //Semaphore ID
    int semID;

    char socket_message[2000];
    int bytes_read_size;

    //Socket erstellen (Server Socket)
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        printf("Could not create Socket");
    }
    puts("Server Socket created");

    memset(&server, '\0', sizeof(server));

    // Socket Optionen setzen für schnelles wiederholtes Binden der Adresse
    int option = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *) &option, sizeof(int));


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    //Socket binden
    int bsock = bind(sock, (struct sockaddr *) &server, sizeof(server));
    if (bsock < 0) {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind successful");

    //Socket lauschen lassen
    //n = amount of clients that can connect to the server
    int lsock = listen(sock, 5);
    if (lsock < 0) {
        perror("listen failed. Error");
        return 1;
    }
    puts("listen successful");

    initSemaphore();
    semID = initSemaphore();
    initSharedMemory();

    while (ITERATION) {
        // Verbindung eines Clients wird entgegengenommen
        ClientSocket = accept(sock, (struct sockaddr *) &clientAddr, &client_len);
        if (ClientSocket < 0) {
            perror("accept failed");
        }
        puts("Connection accepted");
        printf("Connection accepted from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));



        if (fork() == 0) {
        while(1){

                //Zurückschicken der Daten, solange der Client welche schickt (und kein Fehler passiert)


                bytes_read_size = read(ClientSocket, socket_message, 2000);

                int i = 0;
                if (socket_message[0] != '\0') {
                    while (socket_message[i] != '\0') {
                        if (socket_message[i] == '\r') {
                            socket_message[i] = '\0';
                            break;
                        }
                        i++;
                    }
                }

                char key[50];
                char value[50];
                char output[100];
                int success = 0;

                int command = readCommand(socket_message, key, value);

                switch (command) {
                    case 0:
                        printf("Gehe in put\n");
                        success = put(key, value);
                        break;
                    case 1:
                        printf("Gehe in get\n");
                        success = get(key, value);
                        break;
                    case 2:
                        printf("Gehe in del\n");
                        success = del(key);
                        break;
                    case 3:
                        printf("CLOSE\n");
                        close(sock);
                        close(ClientSocket);
                        return 0;
                    case 4:
                        printf("Gehe in Beg\n");
                        beg();
                        semop(semID, &enter, 1);
                        break;
                    case 5:
                        printf("Gehe in End\n");
                        end();
                        semop(semID, &leave, 1);
                        break;
                    case 6:
                        printf("Gehe in Sub\n");
                        success = sub(key, ClientSocket);
                        if(success == 1){
                            get(key, value);
                        }
                        break;
                    default:
                        printf("Normal stop\n");
                        printf("sending back the %d bytes I received... \n", bytes_read_size);
                        write(ClientSocket, socket_message, bytes_read_size);
                        break;
                }
                if (command > -1) {
                    commandPrint(ClientSocket, command, key, value, success, output);
                }
            }
        }
        if(close(ClientSocket)) {
            detachSharedMemory();
            deleteSemaphore();
            return 0;
        }
    }

}


