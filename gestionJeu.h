//
// Created by yaon on 13/12/22.
//

#ifndef PROJET_SYSTEMES_RESEAUX_GESTIONJEU_H
#define PROJET_SYSTEMES_RESEAUX_GESTIONJEU_H

struct carte{
    int numero;
    int nbTete;
};

struct joueur{
    struct carte cartes[10];
    int score;
};

void sigint_trap(int);

int init_serveur();

void ajout_joueur(int, int *, int);

void showToClient(int, char *);

void promptFromClient(int, char *);

void boucleJeu(int *, int);

#endif //PROJET_SYSTEMES_RESEAUX_GESTIONJEU_H
