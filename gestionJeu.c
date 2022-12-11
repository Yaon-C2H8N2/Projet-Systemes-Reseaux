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

void boucleJeu(int *client, int nbJoueur) {
    for (int i = 0; i < nbJoueur; i++) {
        int valid = 0;
        char message[256];
        do {
            memset(message, 0, sizeof(message));
            strcat(message, "[show]");
            send(client[i], message, 256 * sizeof(char), 0);
            memset(message, 0, sizeof(message));
            strcat(message, "Test affichage");
            send(client[i], message, 256 * sizeof(char), 0);
            memset(message, 0, sizeof(message));
            strcat(message, "[prompt]");
            send(client[i], message, 256 * sizeof(char), 0);
            memset(message, 0, sizeof(message));
            recv(client[i], message, sizeof(message), 0);
            printf("Réponse client : %s\n", message);
            if (strcmp(message,"123456")==0) {
                valid++;
                memset(message, 0, sizeof(message));
                strcat(message, "[show]");
                send(client[i], message, 256 * sizeof(char), 0);
                memset(message, 0, sizeof(message));
                strcat(message, "Réponse valide");
                send(client[i], message, 256 * sizeof(char), 0);
            }else{
                memset(message, 0, sizeof(message));
                strcat(message, "[show]");
                send(client[i], message, 256 * sizeof(char), 0);
                memset(message, 0, sizeof(message));
                strcat(message, "Réponse invalide");
                send(client[i], message, 256 * sizeof(char), 0);
            }
        } while (valid == 0);
    }
}

int main() {
    int sd;
    int nbJoueur = 0;
    int *client = (int *) (malloc(sizeof(int)));

    sd = init_serveur();

    char user_input = ' ';
    unsigned short quit = 0;

    do {
        printf("Voulez-vous ajouter un joueur ? [o/n] (q pour quitter)\n");
        scanf(" %c", &user_input);
        switch (user_input) {
            case 'o':
                printf("Humain ou robot ? [h/r]\n");
                scanf(" %c", &user_input);
                if (strcmp(&user_input, "h") == 0) {
                    nbJoueur++;
                    client = (int *) (realloc(client, nbJoueur * sizeof(int)));
                    ajout_joueur(sd, client, nbJoueur);
                } else if (strcmp(&user_input, "r") == 0) {
                    printf("Robot pas encore implémenté\n");
                    //TODO Fork puis exec d'un joueur robot qui répondra automatiquement aux requêtes du serveur.
                } else printf("Commande non reconnue\n");
                break;
            case 'n':
                if (nbJoueur >= 2) {
                    boucleJeu(client, nbJoueur);
                    quit++;
                } else printf("Pas assez de joueur présents pour lancer la partie\n");
                break;
            case 'q':
                quit++;
                break;
            default:
                printf("Commande non reconnue\n");
                break;
        }
    } while (!quit);

    for (int i = 0; i < nbJoueur; i++) {
        char message[256] = "[quit]";
        send(client[i], message, 256 * sizeof(char), 0);
        close(client[i]);
    }
    close(sd);
    free(client);
    return 0;
}
