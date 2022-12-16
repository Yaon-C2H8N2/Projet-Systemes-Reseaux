#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "gestionJeu.h"

#define MAX_PLAYER_COUNT 4

int *SIG_CLIENT;
int SIG_SD;
int SIG_NBJ;

void sigint_trap(int sig) {
    for (int i = 0; i < SIG_NBJ; i++) {
        char message[256] = "[quit]";
        send(SIG_CLIENT[i], message, 256 * sizeof(char), 0);
        close(SIG_CLIENT[i]);
        printf("Déconnexion client %d\n", i);
    }

    //fermeture du socket
    close(SIG_SD);
    printf("Fermeture du socket\n");
    free(SIG_CLIENT);
    exit(0);
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

void showToClient(int client, char *contenu) {
    char message[256];
    memset(message, 0, sizeof(message));
    strcat(message, "[show]");
    send(client, message, 256 * sizeof(char), 0);
    memset(message, 0, sizeof(message));
    strcat(message, contenu);
    send(client, message, 256 * sizeof(char), 0);
}

void promptFromClient(int client, char *content) {
    char message[256];
    memset(message, 0, sizeof(message));
    strcat(message, "[prompt]");
    send(client, message, 256 * sizeof(char), 0);
    memset(message, 0, sizeof(message));
    recv(client, message, sizeof(message), 0);
    strcpy(content, message);
}

void boucleJeu(int *client, int nbJoueur) {
    int fin = 0;
    struct joueur joueurs[nbJoueur];
    struct carte pioche[104];
    struct carte plateau[4][10];

    // init de la pioche
    for (int i = 0; i < 104; i++) {
        pioche[i].numero = i + 1;
        pioche[i].nbTete = (rand() % 7) + 1;
    }

    //init du plateau
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            plateau[i][j].numero = 0;
            plateau[i][j].nbTete = 0;
        }
        int index = 0;
        do {
            index = rand() % 104;
            plateau[i][0].numero = pioche[index].numero;
            plateau[i][0].nbTete = pioche[index].nbTete;
        } while (pioche[index].numero == 0);
        pioche[index].numero = 0;
        pioche[index].nbTete = 0;
    }

    //DEBUG affichage du plateau
    for (int i = 0; i < 4; i++) {
        char message[256];
        memset(message, 0, sizeof(message));
        for (int j = 0; j < 6; j++) {
            if (plateau[i][j].numero > 0) {
                char buffer[25];
                snprintf(buffer, 25, "[%d - %d]", plateau[i][j].numero, plateau[i][j].nbTete);
                strcat(message, buffer);
            }
        }
        printf("[%d]%s\n", i, message);
    }

    //init des joueurs
    for (int i = 0; i < nbJoueur; i++) {
        joueurs[i].score = 0;
        for (int j = 0; j < 10; j++) {
            joueurs[i].cartes[j].numero = 0;
            joueurs[i].cartes[j].nbTete = 0;
        }
        int index = 0;
        for (int j = 0; j < 10; j++) {
            do {
                index = rand() % 104;
                joueurs[i].cartes[j].numero = pioche[index].numero;
                joueurs[i].cartes[j].nbTete = pioche[index].nbTete;
            } while (pioche[index].numero == 0);
            pioche[index].numero = 0;
            pioche[index].nbTete = 0;
        }
    }

    //DEBUG affichage des jeux des joueurs
    for (int i = 0; i < nbJoueur; i++) {
        char message[256];
        memset(message, 0, sizeof(message));
        for (int j = 0; j < 10; j++) {
            if (joueurs[i].cartes[j].numero > 0) {
                char buffer[25];
                snprintf(buffer, 25, "%d.[%d - %d]", j, joueurs[i].cartes[j].numero, joueurs[i].cartes[j].nbTete);
                strcat(message, buffer);
            }
        }
        printf("Jeu du joueur %d :\n%s\n", i, message);
    }

    //début de la boucle de jeu
    do {
        int i = 0;
        while (i < nbJoueur && fin == 0) {
            int valid = 0;
            char message[256];

            //Affichage du plateau à tous les joueurs et indication du joueur dont c'est le tour
            for (int j = 0; j < nbJoueur; j++) {
                memset(message, 0, sizeof(message));
                if (i == j) {
                    strcat(message, "      Début de votre tour");
                } else {
                    snprintf(message, 256, "   Début du tour du joueur %d", (i + 1));
                }
                showToClient(client[j], "===============================");
                showToClient(client[j], message);
                showToClient(client[j], "===============================");
                showToClient(client[j], "PLATEAU DE JEU : ");
                for (int k = 0; k < 4; k++) {
                    memset(message, 0, sizeof(message));
                    snprintf(message, 256, "[%d]", k);
                    for (int l = 0; l < 6; l++) {
                        if (plateau[k][l].numero > 0) {
                            char buffer[25];
                            snprintf(buffer, 25, "[%d - %d]", plateau[k][l].numero, plateau[k][l].nbTete);
                            strcat(message, buffer);
                        }
                    }
                    showToClient(client[j], message);
                }
            }
            //affichage des points du joueur
            memset(message, 0, sizeof(message));
            snprintf(message, 256, "Vos points : %d", joueurs[i].score);
            showToClient(client[i], message);

            //affichage du jeu du joueur
            memset(message, 0, sizeof(message));
            for (int j = 0; j < 10; j++) {
                if (joueurs[i].cartes[j].numero > 0) {
                    char buffer[25];
                    snprintf(buffer, 25, "%d.[%d - %d]", j+1, joueurs[i].cartes[j].numero, joueurs[i].cartes[j].nbTete);
                    strcat(message, buffer);
                }
            }
            showToClient(client[i], "Votre jeu :");
            showToClient(client[i], message);

            //Début du tour du joueur i
            do {
                showToClient(client[i], "Quelle carte souhaitez vous jouer ?");
                promptFromClient(client[i], message);
                printf("[DEBUG][CLIENT %d]Réponse client : %s\n", i, message);
                int carteJouee = atoi(message);
                if (carteJouee < 11 && carteJouee >=1 && joueurs[i].cartes[carteJouee-1].numero != 0) {
                    valid++;
                    showToClient(client[i], "Réponse valide");
                } else if (strcmp(message, "quit") == 0) {
                    valid++;
                    fin++;
                    showToClient(client[i], "Réponse valide");
                    showToClient(client[i], "Fin du programme");
                } else {
                    showToClient(client[i], "Réponse invalide");
                }
            } while (valid == 0);
            i++;
        }
    } while (fin == 0);
}

int main() {
    srand(time(NULL));
    int sd;
    int nbJoueur = 0;
    SIG_NBJ = 0;
    int *client = (int *) (malloc(sizeof(int)));
    SIG_CLIENT = client;

    sd = init_serveur();
    SIG_SD = sd;

    char user_input = ' ';
    unsigned short quit = 0;

    signal(SIGINT, sigint_trap);
    signal(SIGTERM, sigint_trap);

    do {
        if (nbJoueur < MAX_PLAYER_COUNT) {
            printf("Voulez-vous ajouter un joueur ? [o/n] (q pour quitter)\n");
            scanf(" %c", &user_input);
            switch (user_input) {
                case 'o':
                    printf("Humain ou robot ? [h/r]\n");
                    scanf(" %c", &user_input);
                    if (strcmp(&user_input, "h") == 0) {
                        nbJoueur++;
                        SIG_NBJ = nbJoueur;
                        client = (int *) (realloc(client, nbJoueur * sizeof(int)));
                        SIG_CLIENT = client;
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
        } else {
            printf("Nombre maximum de joueur atteint. Lancement de la partie.\n");
            boucleJeu(client, nbJoueur);
            quit++;
        }
    } while (!quit);

    //déconnexion des clients
    for (int i = 0; i < nbJoueur; i++) {
        char message[256] = "[quit]";
        send(client[i], message, 256 * sizeof(char), 0);
        close(client[i]);
        printf("Déconnexion client %d\n", i);
    }

    //fermeture du socket
    close(sd);
    printf("Fermeture du socket\n");
    free(client);
    return 0;
}
