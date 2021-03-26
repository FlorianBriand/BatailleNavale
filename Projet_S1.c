////////////// PROJET /////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "IA.h"

int *tabTaillesBateaux;

int *grilleIA;
int *grilleADV;
int tailleGrille;

int stylejeux;
int toto;
int attaqueEnCours;
int dirAttaque;
int nbPoints;
int premierPoint;
int dernierPoint;
int positionTir;

void initscore(char *nom_fichier)
{
	FILE *fichier;
	fichier = fopen(nom_fichier, "w");
	if (fichier != NULL)
	{
		fprintf(fichier, "%d\n", 0);
		fprintf(fichier, "%d\n", 0);
		fprintf(fichier, "%d\n", 0);
		fclose(fichier);
	}
}

void modifscore(char *nom_fichier, int retour)
{
	FILE *fichier;
	fichier = fopen(nom_fichier, "r");
	int tableau[2];
	if (fichier != NULL)
	{
		fscanf(fichier, "%d\n", &tableau[0]);
		fscanf(fichier, "%d\n", &tableau[1]);
		fscanf(fichier, "%d\n", &tableau[2]);
		tableau[0] = tableau[0] + 1;
		if (retour == 2)
		{
			tableau[1] = tableau[1] + 1;
		}
		if (retour == 3)
		{
			tableau[1] = tableau[1] + 1;
			tableau[2] = tableau[2] + 1;
		}
		fclose(fichier);
	}
	fichier = fopen(nom_fichier, "w");
	if (fichier != NULL)
	{
		fprintf(fichier, "%d\n", tableau[0]);
		fprintf(fichier, "%d\n", tableau[1]);
		fprintf(fichier, "%d\n", tableau[2]);
		fclose(fichier);
	}
}

void affScore(char *nom_fichier)
{
	FILE *fichier;
	fichier = fopen(nom_fichier, "r");
	int a, b, c;
	if (fichier != NULL)
	{
		fscanf(fichier, "%d\n", &a);
		fscanf(fichier, "%d\n", &b);
		fscanf(fichier, "%d\n", &c);
		fclose(fichier);
	}
	printf("Vous avez tiré %d fois, dont %d fois qui ont touché\nVous avez coulé %d bateaux\n", a, b, c);
}

void aff_tab_dim1(int *tab, int taille_grille)
{
	int i;
	for (i = 0; i < taille_grille; i++)
	{
		printf("| %d ", tab[i]);
	}
	printf("|\n");
}

int nombre_bateau(int taille_grille, int tab_taille_bateau[4])
{
	int coeff_bat, max_pa, max_crois, max_sm, max_torpi;
	int nb_pa, nb_crois, nb_sm, nb_torpi;
	coeff_bat = taille_grille / 10;
	//Max de bateau par catégorie
	max_pa = 1 * coeff_bat;
	max_crois = 2 * coeff_bat;
	max_sm = 3 * coeff_bat;
	max_torpi = 4 * coeff_bat;
	//Demande de PA
	printf("Combien de porte-avions voulez vous ? (1 à max %d) \n", max_pa);
	scanf("%d", &nb_pa);
	while (nb_pa < 1 || nb_pa > max_pa)
	{
		printf("Le nombre de porte-avion n'est pas valide \n");
		scanf("%d", &nb_pa);
	}
	//Demande de Crois
	printf("Combien de croiseurs voulez vous ? (1 à max %d) \n", max_crois);
	scanf("%d", &nb_crois);
	while (nb_crois < 1 || nb_crois > max_crois)
	{
		printf("Le nombre de croiseur n'est pas valide \n");
		scanf("%d", &nb_crois);
	}
	//Demande de sm
	printf("Combien de sous-marins voulez vous ? (1 à max %d) \n", max_sm);
	scanf("%d", &nb_sm);
	while (nb_sm < 1 || nb_sm > max_sm)
	{
		printf("Le nombre de sous-marin n'est pas valide \n");
		scanf("%d", &nb_sm);
	}
	//Demande de torpi
	printf("Combien de torpilleurs voulez vous ? (1 à max %d) \n", max_torpi);
	scanf("%d", &nb_torpi);
	while (nb_torpi < 1 || nb_torpi > max_torpi)
	{
		printf("Le nombre de torpilleur n'est pas valide \n");
		scanf("%d", &nb_torpi);
	}
	// Entrer dans tableau
	tab_taille_bateau[0] = nb_pa;
	tab_taille_bateau[1] = nb_crois;
	tab_taille_bateau[2] = nb_sm;
	tab_taille_bateau[3] = nb_torpi;

	return 0;
}

int verif_bord_eau(int abscisse, int ordonnee, int *tableau_grille, int taille_grille)
{
	int verif;
	verif = 0;

	if ((ordonnee < 0) || (ordonnee >= taille_grille))
	{
		verif = 1;
	}
	else if ((abscisse < 0) || (abscisse >= taille_grille))
	{
		verif = 1;
	}
	else if (tableau_grille[ordonnee * taille_grille + abscisse] == 0)
	{
		verif = 1;
	}

	return verif;
}

int Verif_couloir(int taille_bat, int abscisse, int ordonnee, int orientation, int *tab_grille, int taille_grille)
{
	int verif, verif_case, retour;
	verif = 0;
	retour = 0;
	//vertical
	if (orientation == 1)
	{
		for (int i = ordonnee; i < ordonnee + taille_bat; i++)
		{
			if (i == ordonnee)
			{
				verif_case = verif_bord_eau(abscisse, i - 1, tab_grille, taille_grille);
				verif_case += verif_bord_eau(abscisse + 1, i - 1, tab_grille, taille_grille);
				verif_case += verif_bord_eau(abscisse - 1, i - 1, tab_grille, taille_grille);
				verif_case += verif_bord_eau(abscisse + 1, i, tab_grille, taille_grille);
				verif_case += verif_bord_eau(abscisse - 1, i, tab_grille, taille_grille);
				if (verif_case == 5)
				{
					verif++;
				}
			}
			else if (i == ordonnee + taille_bat - 1)
			{
				verif_case = verif_bord_eau(abscisse, i + 1, tab_grille, taille_grille) + verif_bord_eau(abscisse + 1, i + 1, tab_grille, taille_grille) + verif_bord_eau(abscisse - 1, i + 1, tab_grille, taille_grille) + verif_bord_eau(abscisse + 1, i, tab_grille, taille_grille) + verif_bord_eau(abscisse - 1, i, tab_grille, taille_grille);
				if (verif_case == 5)
				{
					verif++;
				}
			}
			else
			{
				verif_case = verif_bord_eau(abscisse + 1, i, tab_grille, taille_grille) + verif_bord_eau(abscisse - 1, i, tab_grille, taille_grille);
				if (verif_case == 2)
				{
					verif++;
				}
			}
		}
		//printf("verif : %d \n", verif);
	}
	//horizontale
	if (orientation == 0)
	{
		for (int j = abscisse; j < abscisse + taille_bat; j++)
		{
			if (j == abscisse)
			{
				verif_case = verif_bord_eau(j - 1, ordonnee, tab_grille, taille_grille) + verif_bord_eau(j - 1, ordonnee + 1, tab_grille, taille_grille) + verif_bord_eau(j - 1, ordonnee - 1, tab_grille, taille_grille) + verif_bord_eau(j, ordonnee + 1, tab_grille, taille_grille) + verif_bord_eau(j, ordonnee - 1, tab_grille, taille_grille);
				if (verif_case == 5)
				{
					verif++;
				}
			}
			else if (j == abscisse + taille_bat - 1)
			{
				verif_case = verif_bord_eau(j + 1, ordonnee, tab_grille, taille_grille) + verif_bord_eau(j + 1, ordonnee + 1, tab_grille, taille_grille) + verif_bord_eau(j + 1, ordonnee - 1, tab_grille, taille_grille) + verif_bord_eau(j, ordonnee + 1, tab_grille, taille_grille) + verif_bord_eau(j, ordonnee - 1, tab_grille, taille_grille);
				if (verif_case == 5)
				{
					verif++;
				}
			}
			else
			{
				verif_case = verif_bord_eau(j, ordonnee + 1, tab_grille, taille_grille) + verif_bord_eau(j, ordonnee - 1, tab_grille, taille_grille);
				if (verif_case == 2)
				{
					verif++;
				}
			}
		}
	}
	if (verif == taille_bat)
	{
		retour = 1;
	}
	return retour;
}

int conversion(int i)
{ // permet de convertiseur l index en la taille du bateau
	if (i == 0)
	{
		return 6;
	}
	if (i == 1)
	{
		return 4;
	}
	if (i == 2)
	{
		return 3;
	}
	else
	{
		return 2;
	}
}

void init_grille(int *grille, int taille_grille)
{
	for (int i = 0; i < taille_grille; i++)
	{
		for (int j = 0; j < taille_grille; j++)
		{
			grille[i * taille_grille + j] = 0;
		}
	}
}

void init_grille4(int *grille, int taille_grille)
{
	for (int i = 0; i < taille_grille; i++)
	{
		for (int j = 0; j < taille_grille; j++)
		{
			grille[i * taille_grille + j] = 4;
		}
	}
}

void aff_tab_dim2(int *grille, int taille_grille)
{
	for (int i = 0; i < taille_grille; i++)
	{
		for (int j = 0; j < taille_grille; j++)
		{
			printf("%d ", grille[i * taille_grille + j]);
		}
		printf("\n");
	}
}

void write(char *nom_fichier, int *tableau, int taille_grille)
{
	FILE *fichier;
	fichier = fopen(nom_fichier, "w");
	if (fichier != NULL)
	{
		for (int i = 0; i < taille_grille; i++)
		{
			for (int j = 0; j < taille_grille; j++)
			{
				fprintf(fichier, "%d", tableau[i * taille_grille + j]);
			}
		}
		fclose(fichier);
	}
}

void conversion_ascii(int *tableau, int taille_grille)
{
	for (int i = 0; i < taille_grille; i++)
	{
		for (int j = 0; j < taille_grille; j++)
		{
			if (tableau[i * taille_grille + j] == 48)
			{
				tableau[i * taille_grille + j] = 0;
			}
			if (tableau[i * taille_grille + j] == 49)
			{
				tableau[i * taille_grille + j] = 1;
			}
			if (tableau[i * taille_grille + j] == 50)
			{
				tableau[i * taille_grille + j] = 2;
			}
			if (tableau[i * taille_grille + j] == 51)
			{
				tableau[i * taille_grille + j] = 3;
			}
			if (tableau[i * taille_grille + j] == 52)
			{
				tableau[i * taille_grille + j] = 4;
			}
			if (tableau[i * taille_grille + j] == 53)
			{
				tableau[i * taille_grille + j] = 5;
			}
			if (tableau[i * taille_grille + j] == 54)
			{
				tableau[i * taille_grille + j] = 6;
			}
			if (tableau[i * taille_grille + j] == 55)
			{
				tableau[i * taille_grille + j] = 7;
			}
			if (tableau[i * taille_grille + j] == 56)
			{
				tableau[i * taille_grille + j] = 8;
			}
			if (tableau[i * taille_grille + j] == 57)
			{
				tableau[i * taille_grille + j] = 9;
			}
		}
	}
}

void read(char *nom_fichier, int *tableau, int taille_grille)
{
	FILE *fichier;
	fichier = fopen(nom_fichier, "r");
	int i = 0;
	int caractereActuel = 0;
	if (fichier != NULL)
	{
		while (caractereActuel != EOF)
		{
			caractereActuel = fgetc(fichier);
			tableau[i] = caractereActuel;
			i++;
		}
	}
	fclose(fichier);
	conversion_ascii(tableau, taille_grille);
	//aff_tab_dim2(tableau,taille_grille);
}

int verifplace(int taille_bateau, int abscisse, int ordonnee, int orientation, int taille_grille)
{
	if (orientation == 1)
	{
		if (ordonnee + taille_bateau > taille_grille)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		if (abscisse + taille_bateau > taille_grille)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

void ajoute_bateau(int taille_bateau, int abscisse, int ordonnee, int orientation, int taille_grille, int *grille)
{
	if (orientation == 1)
	{
		for (int i = 0; i < taille_bateau; i++)
		{
			grille[(ordonnee - 1) * taille_grille + abscisse - 1] = grille[(ordonnee - 1) * taille_grille + abscisse - 1] + 1;
			ordonnee = ordonnee + 1;
		}
	}
	else
	{
		for (int i = 0; i < taille_bateau; i++)
		{
			grille[(ordonnee - 1) * taille_grille + abscisse - 1] = grille[(ordonnee - 1) * taille_grille + abscisse - 1] + 1;
			abscisse = abscisse + 1;
		}
	}
}

int recherche(int *grille1, int taille_grille)
{
	int resultat;
	resultat = 1;
	for (int i = 0; i < taille_grille; i++)
	{
		for (int j = 0; j < taille_grille; j++)
		{
			if (grille1[i * taille_grille + j] == 2)
			{
				resultat = 0;
			}
		}
	}
	return resultat;
}

int verif_entrechoc(int taille_bateau, int abscisse, int ordonnee, int orientation, int taille_grille, int *grille)
{
	int grille1[taille_grille][taille_grille];
	for (int i = 0; i < taille_grille; i++)
	{
		for (int j = 0; j < taille_grille; j++)
		{
			grille1[i][j] = grille[i * taille_grille + j];
		}
	}
	ajoute_bateau(taille_bateau, abscisse, ordonnee, orientation, taille_grille, *grille1);
	int a;
	a = recherche(*grille1, taille_grille);
	return a;
}

void placebateau(int taille_bateau, int *grille, int taille_grille, int numero_joueur)
{
	int place, abscisse, ordonnee, orientation;
	place = 0;
	while (place == 0)
	{
		orientation = 2;
		abscisse = 0;
		ordonnee = 0;
		while ((orientation != 0) && (orientation != 1))
		{
			printf("Entrer l'orientation du bateau, 1 pour vertical et 0 pour horizontal\n");
			scanf("%d", &orientation);
		}
		while ((abscisse < 1) || (abscisse > taille_grille))
		{
			printf("Entrer l'abscisse de début du bateau\n");
			scanf("%d", &abscisse);
		}
		while ((ordonnee < 1) || (ordonnee > taille_grille))
		{
			printf("Entrer l'ordonnée de début du bateau\n");
			scanf("%d", &ordonnee);
		}
		place = verifplace(taille_bateau, abscisse, ordonnee, orientation, taille_grille);
		if (place == 1)
		{
			place = verif_entrechoc(taille_bateau, abscisse, ordonnee, orientation, taille_grille, grille);
			if (place == 1)
			{
				place = Verif_couloir(taille_bateau, abscisse - 1, ordonnee - 1, orientation, grille, taille_grille);
				if (place == 1)
				{
					printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
					ajoute_bateau(taille_bateau, abscisse, ordonnee, orientation, taille_grille, grille);
					aff_tab_dim2(grille, taille_grille);
					printf("\n");
				}
			}
		}
		if (place == 0)
		{
			// clear page
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("Le placement du bateau est mauvais \n");
			aff_tab_dim2(grille, taille_grille);
		}
	}
	char buffer[50];
	sprintf(buffer, "tab_j%d.txt", numero_joueur);
	write(buffer, grille, taille_grille);
}

void remplirgrille(int tab_taille_bateau[4], int *grille, int taille_grille, int numero_joueur)
{
	if (numero_joueur == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < tab_taille_bateau[i]; j++)
			{
				placebateau(conversion(i), grille, taille_grille, numero_joueur);
			}
		}
	}
	else
	{
		if (stylejeux == 0)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < tab_taille_bateau[i]; j++)
				{
					placebateau(conversion(i), grille, taille_grille, numero_joueur);
				}
			}
		}
		else
		{
			//printf("Debut remplissage grille IA \n");
			grilleADV = grille;
			tailleGrille = taille_grille;
			tabTaillesBateaux = tab_taille_bateau;
			IAPlacements();
			printf("\nVoilà la grille de l'IA \n\n");
			char buffer[50];
			sprintf(buffer, "tab_j2.txt");
			write(buffer, grille, taille_grille);
		}
	}
}

int recuperation_taille_grille()
{
	FILE *fichier;
	int taille_grille;
	fichier = fopen("taille_grille.txt", "r");
	fscanf(fichier, "%d", &taille_grille);
	fclose(fichier);
	return taille_grille;
}

int initilisation()
{
	int taille_grille;
	taille_grille = 0;
	while (taille_grille < 10)
	{
		printf("Quelle est la taille de la grille ? (forcément superieurà 10) \n");
		scanf("%d", &taille_grille);
	}
	FILE *fichier;
	fichier = fopen("taille_grille.txt", "w");
	if (fichier != NULL)
	{
		fprintf(fichier, "%d", taille_grille);
	}
	fclose(fichier);
	return taille_grille;
}

int voisinNord(int *tab, int x, int y, int s)
{
	int i;
	i = x - 1;
	return tab[i * s + y];
}
int voisinNordEst(int *tab, int x, int y, int s)
{
	int i, j;
	i = x - 1;
	j = y + 1;
	return tab[i * s + j];
}
int voisinEst(int *tab, int x, int y, int s)
{
	int j;
	j = y + 1;
	return tab[x * s + j];
}
int voisinSudEst(int *tab, int x, int y, int s)
{
	int i, j;
	i = x + 1;
	j = y + 1;
	return tab[i * s + j];
}
int voisinSud(int *tab, int x, int y, int s)
{
	int i;
	i = x + 1;
	return tab[i * s + y];
}
int voisinSudOuest(int *tab, int x, int y, int s)
{
	int i, j;
	i = x + 1;
	j = y - 1;
	return tab[i * s + j];
}
int voisinOuest(int *tab, int x, int y, int s)
{
	int j;
	j = y - 1;
	return tab[x * s + j];
}
int voisinNordOuest(int *tab, int x, int y, int s)
{
	int i, j;
	i = x - 1;
	j = y - 1;
	return tab[i * s + j];
}
int compteCelluleVoisinage(int *tab, int x, int y, int s)
{
	int i, a;
	i = 0;
	a = voisinNord(tab, x, y, s);
	if (a == 1)
	{
		i = i + 1;
	}
	a = voisinNordEst(tab, x, y, s);
	if (a == 1)
	{
		i = i + 1;
	}
	a = voisinEst(tab, x, y, s);
	if (a == 1)
	{
		i = i + 1;
	}
	a = voisinSudEst(tab, x, y, s);
	if (a == 1)
	{
		i = i + 1;
	}
	a = voisinSud(tab, x, y, s);
	if (a == 1)
	{
		i = i + 1;
	}
	a = voisinSudOuest(tab, x, y, s);
	if (a == 1)
	{
		i = i + 1;
	}
	a = voisinOuest(tab, x, y, s);
	if (a == 1)
	{
		i = i + 1;
	}
	a = voisinNordOuest(tab, x, y, s);
	if (a == 1)
	{
		i = i + 1;
	}
	return i;
}

int resTir(int ligne, int colonne, int *tab, int taille)
{
	int orientation;
	int pos;
	int cptbat, cpttouche;
	cptbat = 0;
	cpttouche = 0;
	if ((colonne - 1) >= 0)
	{
		if (tab[ligne * taille + (colonne - 1)] != 0)
		{
			orientation = 0;
		}
	}
	if ((colonne + 1) < taille)
	{
		if (tab[ligne * taille + (colonne + 1)] != 0)
		{
			orientation = 0;
		}
	}
	if ((ligne - 1) >= 0)
	{
		if (tab[(ligne - 1) * taille + colonne] != 0)
		{
			orientation = 1;
		}
	}
	if ((ligne + 1) < taille)
	{
		if (tab[(ligne + 1) * taille + colonne] != 0)
		{
			orientation = 1;
		}
	}
	if (orientation == 0)
	{
		pos = colonne;
		while (((pos - 1) >= 0) && (tab[ligne * taille + (pos - 1)] != 0))
		{
			pos--;
		}
		while ((pos < taille) && (tab[ligne * taille + pos] != 0))
		{
			cptbat++;
			if (tab[ligne * taille + pos] == 2)
			{
				cpttouche++;
			}
			pos++;
		}
	}
	else
	{
		pos = ligne;
		while (((pos - 1) >= 0) && (tab[(pos - 1) * taille + colonne] != 0))
		{
			pos--;
		}
		while ((pos < taille) && (tab[pos * taille + colonne] != 0))
		{
			cptbat++;
			if (tab[pos * taille + colonne] == 2)
			{
				cpttouche++;
			}
			pos++;
		}
	}

	if (cptbat == cpttouche)
	{
		return 3;
	}
	else
	{
		return 2;
	}
}

int tire(int *tab_att, int *tab_aff, int *tmp, int s, int numj)
{
	int x, y, retour;
	if ((stylejeux == 1) && (numj == 2))
	{
		printf("\n");
		aff_tab_dim2(tab_att, s);
		printf("IA joue\n");
		grilleIA = tab_att;
		IACibleTir(&x, &y);
		printf("Abscisse : %d\n", y + 1);
		printf("Ordonné : %d\n", x + 1);
	}
	else
	{
		printf("\n");
		aff_tab_dim2(tab_att, s);
		printf("\nOù voulez vous tirer : \n");
		printf("Abscisse du tir : \n");
		scanf("%d", &y);
		y = y - 1;
		printf("Ordonné du tir : \n");
		scanf("%d", &x);
		x = x - 1;
	}
	//if (tab_aff[x * s + y]==tmp[x * s + y]){
	//	printf("\nVous avez déja tiré à cette endroit \n\n");
	//	tire(tab_att,tab_aff,tmp,s, numj);}
	if (tab_aff[x * s + y] == 0)
	{
		tab_att[x * s + y] = 3;
		tmp[x * s + y] = tab_aff[x * s + y];
		printf("\nA l'eau\n");
		retour = 1;
		if (stylejeux == 1)
		{
			printf("\nVoilà le tableau tir de l'IA \n");
			printf("0 = De l'eau, 3 = Tir dans l'eau, 2 = bateau touché \n\n");
			aff_tab_dim2(tab_att, s);
			printf("\n");
		}
		else
		{
			printf("Voilà votre tableau tir\n");
			printf("0 = De l'eau, 3 = Tir dans l'eau, 2 = bateau touché \n\n");
			aff_tab_dim2(tab_att, s);
			printf("\n");
		}
	}
	if (tab_aff[x * s + y] == 1)
	{
		tab_aff[x * s + y] = 2;
		tab_att[x * s + y] = 2;
		tmp[x * s + y] = tab_aff[x * s + y];
		retour = resTir(x, y, tab_aff, s);
		printf("\n");
		if (retour == 2)
		{
			printf("Touché\n");
		}
		if (retour == 3)
		{
			printf("Touché Coulé\n");
			if (stylejeux == 1)
			{
				printf("Voilà le tableau tir de l'IA \n");
				printf("0 = De l'eau, 3 = Tir dans l'eau, 2 = bateau touché \n\n");
				aff_tab_dim2(tab_att, s);
				printf("\n");
			}
			else
			{
				printf("Voilà votre tableau tir\n");
				printf("0 = De l'eau, 3 = Tir dans l'eau, 2 = bateau touché \n\n");
				aff_tab_dim2(tab_att, s);
				printf("\n");
			}
		}
	}
	char buffer[50];
	sprintf(buffer, "scorej%d.txt", numj);
	modifscore(buffer, retour);
	return retour;
}

int nombre1(int *tab_aff, int s)
{
	int nb1;
	nb1 = 0;
	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < s; j++)
		{
			if (tab_aff[i * s + j] == 1)
			{
				nb1 = nb1 + 1;
			}
		}
	}
	return nb1;
}

void jeux(int *tab_att_j1, int *tab_aff_j1, int *tmp1, int *tab_att_j2, int *tab_aff_j2, int *tmp2, int s, int num_j)
{
	int resultat_tir;
	if (num_j == 1)
	{
		int num;
		printf("\nJOUEUR 1 c'est à vous :\n");
		affScore("scorej1.txt");
		resultat_tir = tire(tab_att_j1, tab_aff_j2, tmp1, s, num_j);
		while (resultat_tir == 2)
		{
			printf("C'est encore à vous de jouer\n");
			resultat_tir = tire(tab_att_j1, tab_aff_j2, tmp1, s, num_j);
		}
		printf("Rappel de votre tableau bateau\n");
		printf("1 : bateau, 2 : point du bateau touché\n\n");
		aff_tab_dim2(tab_aff_j1, s);
		printf("\n");
		printf("Fin de votre tour\n");
		write("tab_att_j1", tab_att_j1, s);
		write("tab_j2.txt", tab_aff_j2, s);
		write("tmp1", tmp1, s);

		if (nombre1(tab_aff_j2, s) == 0)
		{
			FILE *fichier;
			fichier = fopen("parametre.txt", "w");
			if (fichier != NULL)
			{
				fprintf(fichier, "%d", 0); //sauvegarde
				fprintf(fichier, "%d", 1);
				fprintf(fichier, "%d", 8);
				fprintf(fichier, "%d", 8);
				fclose(fichier);
			}
			printf("Félicitation vous avez gagné joueur 1 !! \n");
			exit(1);
		}

		printf("Entrer 1 si vous voulez continuer ou 0 pour sauvergarder : ");
		scanf("%d", &num);
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		if (num == 0)
		{
			FILE *fichier;
			fichier = fopen("parametre.txt", "w");
			if (fichier != NULL)
			{
				fprintf(fichier, "%d", 1); //sauvegarde
				fprintf(fichier, "%d", 2);
				fprintf(fichier, "%d", 8);
				fprintf(fichier, "%d", 8);
				fclose(fichier);
				exit(1);
			}
		}
		jeux(tab_att_j1, tab_aff_j1, tmp1, tab_att_j2, tab_aff_j2, tmp2, s, 2);
	}
	else
	{
		printf("\nJOUEUR 2 c'est à vous : \n");
		affScore("scorej2.txt");
		resultat_tir = tire(tab_att_j2, tab_aff_j1, tmp1, s, num_j);
		if (stylejeux == 1)
		{
			IARetourTir(resultat_tir);
		}
		while (resultat_tir == 2)
		{
			printf("C'est encore à vous de jouer\n");
			resultat_tir = tire(tab_att_j2, tab_aff_j1, tmp1, s, num_j);
			if (stylejeux == 1)
			{
				IARetourTir(resultat_tir);
			}
		}
		if (stylejeux == 1)
		{
			printf("Rappel du tableau bateau de l'IA\n");
			printf("1 : bateau, 2 : point du bateau touché\n\n");
			aff_tab_dim2(tab_aff_j2, s);
			printf("\n");
		}
		else
		{
			printf("Rappel de votre tableau bateau\n");
			printf("1 : bateau, 2 : point du bateau touché\n\n");
			aff_tab_dim2(tab_aff_j2, s);
			printf("\n");
		}
		write("tab_att_j2", tab_att_j2, s);
		write("tab_j1.txt", tab_aff_j1, s);
		write("tmp2", tmp2, s);

		if (nombre1(tab_aff_j1, s) == 0)
		{
			FILE *fichier;
			fichier = fopen("parametre.txt", "w");
			if (fichier != NULL)
			{
				fprintf(fichier, "%d", 0); //sauvegarde
				fprintf(fichier, "%d", 2);
				fprintf(fichier, "%d", 8);
				fprintf(fichier, "%d", 8);
				fclose(fichier);
			}
			printf("Félicitation vous avez gagné joueur 2 !! \n");
			exit(1);
		}

		printf("Entrer 1 si vous voulez continuer ou 0 pour sauvergarder : ");
		int num;
		scanf("%d", &num);
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		if (num == 0)
		{
			FILE *fichier;
			fichier = fopen("parametre.txt", "w");
			if (fichier != NULL)
			{
				fprintf(fichier, "%d", 1); //sauvegarde
				fprintf(fichier, "%d", 1); //dernier joueur ayant joué
				fprintf(fichier, "%d", 8);
				fprintf(fichier, "%d", 8);
				fclose(fichier);
				exit(1);
			}
		}
		jeux(tab_att_j1, tab_aff_j1, tmp1, tab_att_j2, tab_aff_j2, tmp2, s, 1);
	}
}

void init_tous_tab(int *tab_aff_j1, int *tab_att_j1, int *tab_aff_j2, int *tab_att_j2, int *tmp1, int *tmp2, int taille_grille)
{
	read("tab_j1.txt", tab_aff_j1, taille_grille);
	read("tab_j2.txt", tab_aff_j2, taille_grille);
	init_grille(tab_att_j1, taille_grille);
	init_grille(tab_att_j2, taille_grille);
	init_grille4(tmp1, taille_grille);
	init_grille4(tmp2, taille_grille);
	write("tab_j1.txt", tab_aff_j1, taille_grille);
	write("tab_j2.txt", tab_aff_j2, taille_grille);
	write("tab_att_j1", tab_att_j1, taille_grille);
	write("tab_att_j2", tab_att_j2, taille_grille);
	write("tmp1", tmp1, taille_grille);
	write("tmp2", tmp2, taille_grille);
}

int main()
{
	int tab_parametre[4];
	int taille_grille, a;
	int tab_taille_bateau[4];
	srand(time(NULL));
	read("parametre.txt", tab_parametre, 4);
	stylejeux = 3;
	while ((stylejeux != 1) && (stylejeux != 0))
	{
		printf("Quelle style de jeux voulez vous ? (0 humain vs humain et 1 humain vs IA)\n");
		scanf("%d", &stylejeux);
	}
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	if (tab_parametre[0] == 1)
	{
		printf("Vous avez une partie en cours.\nVoulez vous la continuer ?\n1 pour oui\n0 pour non\n");
		scanf("%d", &a);
		if (a == 0)
		{
			tab_parametre[0] = 0;
			FILE *fichier;
			fichier = fopen("parametre.txt", "w");
			if (fichier != NULL)
			{
				fprintf(fichier, "%d", 0);
				fprintf(fichier, "%d", 1);
				fprintf(fichier, "%d", 8);
				fprintf(fichier, "%d", 8);
				fclose(fichier);
			}
		}
	}
	if (tab_parametre[0] == 0)
	{
		initscore("scorej1.txt");
		initscore("scorej2.txt");
		taille_grille = initilisation();
		tailleGrille = taille_grille;
		grilleIA = malloc(taille_grille * taille_grille * sizeof(int));
		IAInitialisation();
		nombre_bateau(taille_grille, tab_taille_bateau);
		int grillej1[taille_grille][taille_grille];
		init_grille(*grillej1, taille_grille);
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		printf("Voilà votre tableau joueur 1\n");
		aff_tab_dim2(*grillej1, taille_grille);
		remplirgrille(tab_taille_bateau, *grillej1, taille_grille, 1);
		printf("Entree pour passer au joueur 2\n");
		toto = getchar();
		toto = getchar();
		int grillej2[taille_grille][taille_grille];
		init_grille(*grillej2, taille_grille);
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		printf("Voilà votre tableau joueur 2\n");
		aff_tab_dim2(*grillej2, taille_grille);
		remplirgrille(tab_taille_bateau, *grillej2, taille_grille, 2);
		if (stylejeux == 1)
		{
			aff_tab_dim2(*grillej2, taille_grille);
		}
		printf("Les placements des bateaux sont finis. Entrée pour jouer");
		if (stylejeux == 1)
		{
			toto = getchar();
		}
		else
		{
			toto = getchar();
			toto = getchar();
		}
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		int tab_aff_j1[taille_grille][taille_grille];
		int tab_att_j1[taille_grille][taille_grille];
		int tab_aff_j2[taille_grille][taille_grille];
		int tab_att_j2[taille_grille][taille_grille];
		int tmp1[taille_grille][taille_grille];
		int tmp2[taille_grille][taille_grille];
		init_tous_tab(*tab_aff_j1, *tab_att_j1, *tab_aff_j2, *tab_att_j2, *tmp1, *tmp2, taille_grille);
		printf("Lancement phase de jeu\n");
		jeux(*tab_att_j1, *tab_aff_j1, *tmp1, *tab_att_j2, *tab_aff_j2, *tmp2, taille_grille, tab_parametre[1]);
		free(grilleIA);
	}
	else
	{
		taille_grille = recuperation_taille_grille();
		tailleGrille = taille_grille;
		grilleIA = malloc(taille_grille * taille_grille * sizeof(int));
		IAInitialisation();
		int tab_aff_j1[taille_grille][taille_grille];
		int tab_att_j1[taille_grille][taille_grille];
		int tab_aff_j2[taille_grille][taille_grille];
		int tab_att_j2[taille_grille][taille_grille];
		int tmp1[taille_grille][taille_grille];
		int tmp2[taille_grille][taille_grille];
		read("tab_j1.txt", *tab_aff_j1, taille_grille);
		read("tab_j2.txt", *tab_aff_j2, taille_grille);
		read("tab_att_j1", *tab_att_j1, taille_grille);
		read("tab_att_j2", *tab_att_j2, taille_grille);
		read("tmp2", *tmp2, taille_grille);
		read("tmp1", *tmp1, taille_grille);
		jeux(*tab_att_j1, *tab_aff_j1, *tmp1, *tab_att_j2, *tab_aff_j2, *tmp2, taille_grille, tab_parametre[1]);
		free(grilleIA);
	}
	return 0;
}
