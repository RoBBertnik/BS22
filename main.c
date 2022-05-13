#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "KeyValueStore.h"
#include "sub.h"


#include <arpa/inet.h>
#include <string.h>


#define ITERATION 1
#define PORT 5678

//--------------------------------------------------- Server with Socket ----------------------------------------------------------

int main() {

    int connection; // Verbindungs-Descriptor

    struct sockaddr_in client;
    socklen_t client_len;
    char socket_message[2000];
    int bytes_read_size;


    //Socket erstellen Variant 1
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        printf("Could not create Socket");
    }
    puts("Socket created");


    // Socket Optionen setzen für schnelles wiederholtes Binden der Adresse
    int option = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *) &option, sizeof(int));


    struct sockaddr_in server;
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
    int lsock = listen(sock, 3);
    if (lsock < 0) {
        perror("listen failed. Error");
        return 1;
    }
    puts("listen successful");

    while (ITERATION) {
        // Verbindung eines Clients wird entgegengenommen
        connection = accept(sock, (struct sockaddr *) &client, &client_len);
        if (connection < 0) {
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");


        //Zurückschicken der Daten, solange der Client welche schickt (und kein Fehler passiert)
        while (1) {

            bytes_read_size = read(connection, socket_message, 2000);

            if (socket_message[0] != '\0') {
                int message_length = strlen(socket_message);
                socket_message[message_length - 2] = '\0';


                //Hier kommen Put / Get / etc. rein

                char key[50];
                char value[50];
                char output[100];
                int success = 0;

                int command = readCommand(socket_message, key, value);
                printf("%s \n", key);
                printf("%s \n", value);

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
                        printf("CLOSE");
                        close(connection);
                        close(sock);
                        return 0;
                    default:
                        printf("Normal stop\n");
                        printf("sending back the %d bytes I received... \n", bytes_read_size);
                        write(connection, socket_message, bytes_read_size);
                        break;
                }
                if (command > -1) {
                    commandPrint(command, key, value, success, output);
                    write(connection, output, strlen(output));
                }
            }


        }
        if (bytes_read_size == 2) {
            puts("Client disconnected");
            fflush(stdout);
            close(sock);
        }
    }
}