#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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


    // Socket erstellen Variante 2
    //rfd = socket(AF_INET, SOCK_STREAM, 0);
    //if(rfd < 0) {
    //fprintf(stderr, "Socket konnte nicht erstellt werden\n");
    //exit(-1);
    //}

    // Socket Optionen setzen für schnelles wiederholtes Binden der Adresse
    int option = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *) &option, sizeof(int));


    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    //Socket binden
    int bsock = bind(sock,(struct sockaddr *) &server, sizeof(server));
    if(bsock < 0){
        perror("bind failed. Error");
        return 1;
    }
    puts("bind successful");

    //Socket lauschen lassen
    //n = amount of clients that can connect to the server
    int lsock = listen(sock, 3);
    if(lsock < 0){
        perror("listen failed. Error");
        return 1;
    }
    puts("listen successful");

    while(ITERATION) {
        // Verbindung eines Clients wird entgegengenommen
        connection = accept(sock, (struct sockaddr *) &client, &client_len);
        if(connection < 0){
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");

        //Lesen von Daten, die der Client schickt
        bytes_read_size = read(connection, socket_message, 2000);

        //Zurückschicken der Daten, solange der Client welche schickt (und kein Fehler passiert)
        while (bytes_read_size > 2) {
            printf("sending back the %d bytes I received... \n", bytes_read_size);

            write(connection, socket_message, bytes_read_size);
            bytes_read_size = read(connection, socket_message, 2000);
        }
        if(bytes_read_size == 2){
            puts("Client disconnected");
            fflush(stdout);
        }
        close(sock);
        close(connection);
    }





//--------------------------------------- Client with Socket -----------------------------------------

/*
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 22 );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    //keep communicating with server
    while(1)
    {
        printf("Enter message : ");
        scanf("%s" , message);

        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }

        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }

        puts("Server reply :");
        puts(server_reply);
    }

    close(sock);
    return 0;

    **/


}