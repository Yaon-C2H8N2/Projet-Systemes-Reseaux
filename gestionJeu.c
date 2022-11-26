#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


int main() {
    int sd, client;
    struct sockaddr_in server_addr, client_addr;
    char message[256];

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 1085;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //lancement du socket serveur
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(sd, &server_addr, sizeof(server_addr)) < 0) {
        printf("Erreur lors du lancement du serveur\n");
        return -1;
    }
    printf("Seveur up\n");

    //ecoute de client
    listen(sd, 0);
    printf("Listenning for client ...\n");
    int client_size = sizeof(client_addr);
    client = accept(sd, (struct sockaddr *) &client_addr, &client_size);

    if (client < 0) {
        printf("Une erreur est survenue lors de la connexion avec le client\n");
        return -1;
    }
    printf("Connecté !\n");

    do{
        memset(message,0,strlen(message));
        recv(client, message, sizeof(message), 0);
        printf("Message reçu : %s\n", message);
    }while(strcmp(message,"quit")!=0);

    close(client);
    close(sd);

    return 0;
}
