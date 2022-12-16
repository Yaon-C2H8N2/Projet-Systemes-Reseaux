6QuiPrend-Server: gestionJeu.o sixQuiPrend.o
	gcc -o 6QuiPrend-Server gestionJeu.o sixQuiPrend.o

6QuiPrend-Client: joueur.o
	gcc -o 6QuiPrend-Client joueur.o

gestionJeu.o: src/gestionJeu.c src/gestionJeu.h
	gcc -o gestionJeu.o src/gestionJeu.c src/gestionJeu.h

joueur.o: src/joueur.c
	gcc -o joueur.o src/joueur.c

sixQuiPrend.o: src/sixQuiPrend.c src/sixQuiPrend.h
	gcc -o sixQuiPrend.o src/sixQuiPrend.c src/sixQuiPrend.h

clean:
	rm -rf *.o