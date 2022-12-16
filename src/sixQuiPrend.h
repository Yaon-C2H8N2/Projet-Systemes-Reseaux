//
// Created by yaon on 16/12/22.
//

#ifndef PROJET_SYSTEMES_RESEAUX_SIXQUIPREND_H
#define PROJET_SYSTEMES_RESEAUX_SIXQUIPREND_H

struct carte {
    int numero;
    int nbTete;
};

struct joueur {
    struct carte cartes[10];
    int score;
};

void joueCarte(struct joueur *, struct carte, struct carte **, int);

void initPioche(struct carte *);

void initPlateau(struct carte *, struct carte **);

void initJoueurs(struct carte *, struct joueur *, int nbJoueurs);

#endif //PROJET_SYSTEMES_RESEAUX_SIXQUIPREND_H
