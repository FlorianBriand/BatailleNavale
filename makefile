IA.o : IA.c
	gcc -c -Wall IA.c

Projet_S1.o : Projet_S1.c
	gcc -c -Wall Projet_S1.c
	
Projet_S1 : IA.o Projet_S1.o
	gcc -o Projet_S1 IA.o Projet_S1.o
