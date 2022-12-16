//
// Created by yaon on 16/12/22.
//

#include "sixQuiPrend.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void joueCarte(struct joueur *joueur,struct carte carte, struct carte **plateau, int ligne) {
    int i = 0;
    unsigned short trouvee = 0;
    int nbTeteLigne = 0;
    while (i < 5 && !trouvee) {
        if (plateau[ligne][i].numero == 0) {
            if(plateau[ligne][i-1].numero > carte.numero) {
                //si carte jouée inférieure à dérnière carte de la ligne
                for(int j = 0;j<6;j++){
                    //réinitialisation de la ligne
                    plateau[ligne][j].numero = 0;
                    plateau[ligne][j].nbTete = 0;
                }
                //on place la carte jouée à la première case de la ligne
                plateau[ligne][0].numero = carte.numero;
                plateau[ligne][0].nbTete = carte.nbTete;
                joueur->score += nbTeteLigne;
            }else{
                //sinon on place la carte jouée à la dernière case de la ligne
                plateau[ligne][i].numero = carte.numero;
                plateau[ligne][i].nbTete = carte.nbTete;
            }
            trouvee++;
        } else {
            nbTeteLigne += plateau[ligne][i].nbTete;
            i++;
        }
    }
    if (i >= 5) {
        //si la ligne est pleine
        for(int j = 0;j<6;j++){
            //réinitialisation de la ligne
            plateau[ligne][j].numero = 0;
            plateau[ligne][j].nbTete = 0;
        }
        //on place la carte jouée à la première case de la ligne
        plateau[ligne][0].numero = carte.numero;
        plateau[ligne][0].nbTete = carte.nbTete;
        joueur->score += nbTeteLigne;
    }
}

void initPioche(struct carte *pioche) {
    // init de la pioche
    for (int i = 0; i < 104; i++) {
        pioche[i].numero = i + 1;
        pioche[i].nbTete = (rand() % 7) + 1;
    }
}

void initPlateau(struct carte *pioche, struct carte **plateau) {
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
}

void initJoueurs(struct carte *pioche, struct joueur *joueurs, int nbJoueurs) {
    //init des joueurs
    for (int i = 0; i < nbJoueurs; i++) {
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
}