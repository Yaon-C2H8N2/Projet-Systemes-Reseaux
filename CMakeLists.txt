cmake_minimum_required(VERSION 3.23)
project(Projet_Systemes_Reseaux C)

set(CMAKE_C_STANDARD 23)

add_executable(gestionJeu gestionJeu.c gestionJeu.h)

add_executable(joueur joueur.c)