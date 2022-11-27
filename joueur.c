#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


int main() {
    int sd;
    struct sockaddr_in server_addr;
    char message[256];

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 1085;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //lancement du socket client
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("Erreur lors de connexion au serveur\n");
        return -1;
    }
    printf("Connecté au serveur !\n");
    printf("En attente du début de la partie ...\n");

    memset(message, 0, sizeof(message));
    recv(sd, message, sizeof(message), 0);
    printf("[SERVER]%s\n", message);

    close(sd);
    return 0;
}
