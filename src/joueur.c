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
    int connected = 0;
    do {
        if (connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
            printf("Erreur lors de connexion au serveur, tentative de reconnexion dans 5 secondes...\n");
            sleep(5);
        } else {
            connected = 1;
        }
    } while (!connected);
    printf("Connecté au serveur !\n");
    printf("En attente du début de la partie ...\n");

    unsigned short quit = 0;
    char user_input = ' ';

    do {
        memset(message, 0, sizeof(message));
        recv(sd, message, sizeof(message), 0);
        if (strcmp(message, "[quit]") == 0) {
            printf("Received instructions to quit\n");
            quit++;
        } else if (strcmp(message, "[show]") == 0) {
            memset(message, 0, sizeof(message));
            recv(sd, message, sizeof(message), 0);
            printf("%s\n", message);
        } else if (strcmp(message, "[prompt]") == 0) {
            memset(message, 0, sizeof(message));
            scanf(" %s", &message);
            send(sd, message, 256 * sizeof(char), 0);
        }
    } while (!quit);

    printf("Press any key to quit\n");
    scanf(" %c", &user_input);

    close(sd);
    return 0;
}
