cmake_minimum_required(VERSION 3.23)
project(Projet_Systemes_Reseaux C)

set(CMAKE_C_STANDARD 23)

add_executable(gestionJeu src/gestionJeu.c src/gestionJeu.h src/sixQuiPrend.h src/sixQuiPrend.c)

add_executable(robot src/robot.c)

add_executable(joueur src/joueur.c)