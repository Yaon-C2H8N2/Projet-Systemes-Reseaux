#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


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
        printf("Erreur lors de la connexion du bot au serveur\n");
        return -1;
    }
    printf("Bot connecté au serveur !\n");

    unsigned short quit = 0;

    do {
        memset(message, 0, sizeof(message));
        recv(sd, message, sizeof(message), 0);
        if (strcmp(message, "[quit]") == 0) {
            quit++;
        } else if (strcmp(message, "[show]") == 0) {
            memset(message, 0, sizeof(message));
            recv(sd, message, sizeof(message), 0);
        } else if (strcmp(message, "[prompt]") == 0) {
            memset(message, 0, sizeof(message));
            int reponse = rand() % 10 + 1;
            snprintf(message, 256, "%d", reponse);
            printf("Réponse du robot : %d\n", reponse);
            send(sd, message, 256 * sizeof(char), 0);
        }
    } while (!quit);

    close(sd);
    return 0;
}
