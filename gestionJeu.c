#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_PLAYER_COUNT 4

void gameLoop(int *client) {
    printf("[boucleDeJeu]");
}

int init_serveur() {
    int sd;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 1085;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //lancement du socket serveur
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("Erreur lors du lancement du serveur\n");
        exit(1);
    }
    return sd;
}


void ajout_joueur(int sd, int *client, int nbJoueur) {
    struct sockaddr_in client_addr;

    int newClient;

    //ecoute de client
    listen(sd, MAX_PLAYER_COUNT);
    socklen_t client_size = sizeof(client_addr);
    printf("En attente de connexion du joueur ...\n");
    newClient = accept(sd, (struct sockaddr *) &client_addr, &client_size);

    if (newClient < 0) {
        printf("Une erreur est survenue lors de la connexion avec le client (%d)\n", newClient);
        printf("%s\n", strerror(errno));
        exit(1);
    } else {
        printf("Joueur %d connecté !\n", nbJoueur);
        client[nbJoueur - 1] = newClient;
    }
}

int main() {
    int sd;
    int nbJoueur = 0;
    int *client = (int *) (malloc(sizeof(int)));

    sd = init_serveur();

    printf("Bienvenue sur le jeu du 6 qui prend.\n");
    printf("1. Ajouter un joueur\n");
    printf("2. Ajouter un robot\n");
    printf("3. Lancer la partie\n");
    printf("4. Quitter le programme\n");

    char user_input = ' ';
    unsigned short quit = 0;

    do {
        printf("nbJoueur : %d\n", nbJoueur);
        printf("> ");
        scanf("%c", &user_input);
        switch (user_input) {
            case '1':
                if (nbJoueur < MAX_PLAYER_COUNT) {
                    nbJoueur++;
                    client = (int *) (realloc(client, nbJoueur * sizeof(int)));
                    ajout_joueur(sd, client, nbJoueur);
                } else {
                    printf("Nombre maximum de joueurs connectés, vous ne pouvez pas ajouter de joueur supplémentaire\n");
                }
                break;
            case '3':
                for (int i = 0; i < nbJoueur; i++) {
                    char message[] = "[startingGame]";
                    send(client[i], message, strlen(message), 0);
                }
                gameLoop(client);
                quit++;
                break;
            case '4':
                quit++;
                break;
            default:
                printf("%c", user_input);
                printf("Commande non reconnue\n");
                break;
        }
    } while (!quit);

    for (int i = 0; i < nbJoueur; i++) {
        close(client[i]);
    }
    close(sd);
    return 0;
}
