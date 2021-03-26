#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern int *tabTaillesBateaux;

extern int *grilleIA;
extern int *grilleADV;
extern int tailleGrille;

extern int stylejeux;

extern int attaqueEnCours;
extern int dirAttaque;
extern int nbPoints;
extern int premierPoint;
extern int dernierPoint;
extern int positionTir;

int validite_case(int ligne, int colonne)
{
    int case_ok;

    if ((ligne >= 0) && (ligne < tailleGrille) && (colonne >= 0) && (colonne < tailleGrille))
    {
        case_ok = 1;
    }
    else
    {
        case_ok = 0;
    }

    return case_ok;
}

int valeur_case(int ligne, int colonne)
{
    int valeur_case;
    valeur_case = 1;

    if ((ligne < 0) || (ligne >= tailleGrille) || (colonne < 0) || (colonne >= tailleGrille))
    {
        valeur_case = 0;
    }
    else if (grilleADV[ligne * tailleGrille + colonne] == 0)
    {
        valeur_case = 0;
    }

    return valeur_case;
}

void ecrire_case(int ligne, int colonne, int valeur)
{
    if ((ligne >= 0) && (ligne < tailleGrille) && (colonne >= 0) && (colonne < tailleGrille))
    {
        grilleIA[ligne * tailleGrille + colonne] = valeur;
    }
}

void remplir_case(int ligne, int colonne, int valeur)
{
    if ((ligne >= 0) && (ligne < tailleGrille) && (colonne >= 0) && (colonne < tailleGrille))
    {
        grilleADV[ligne * tailleGrille + colonne] = valeur;
    }
}

int place_ok(int ligne, int colonne, int orientation, int longueur)
{
    int i;
    int j;
    int cpt_valide;
    int cpt_bord;
    int place;
    place = 0;
    if (orientation == 0)
    {
        cpt_valide = 0;
        for (i = 0; i < longueur; i++)
        {
            cpt_valide += validite_case(ligne, colonne + i);
        }
        cpt_bord = 0;
        for (i = 0; i < 3; i++)
            for (j = 0; j < (longueur + 2); j++)
            {
                cpt_bord += valeur_case((ligne - 1 + i), (colonne - 1 + j));
            }

        if ((cpt_valide == longueur) && (cpt_bord == 0))
        {
            place = 1;
        }
    }
    else
    {
        cpt_valide = 0;
        for (i = 0; i < longueur; i++)
        {
            cpt_valide += validite_case(ligne + i, colonne);
        }
        cpt_bord = 0;
        for (i = 0; i < (longueur + 2); i++)
            for (j = 0; j < 3; j++)
            {
                cpt_bord += valeur_case((ligne - 1 + i), (colonne - 1 + j));
            }
        if ((cpt_valide == longueur) && (cpt_bord == 0))
        {
            place = 1;
        }
    }
    return place;
}

void place_bat(int ligne, int colonne, int orientation, int longueur)
{
    int i;
    if (orientation == 0)
    {
        for (i = 0; i < longueur; i++)
            remplir_case(ligne, colonne + i, 1);
    }
    else
    {
        for (i = 0; i < longueur; i++)
            remplir_case(ligne + i, colonne, 1);
    }
}

void IAInitialisation()
{
    int i;
    int j;
    for (i = 0; i < tailleGrille; i++)
        for (j = 0; j < tailleGrille; j++)
            grilleIA[i * tailleGrille + j] = 0;

    attaqueEnCours = 0;
    dirAttaque = 0;
    nbPoints = 0;
    premierPoint = -1;
    dernierPoint = -1;
}

void IAInitTaillesBateaux(int *tabTailles)
{
    tabTaillesBateaux = tabTailles;
}

int IAPlaceBateaux()
{
    int i;
    int j;
    int k;
    int nbBateaux;
    int bat;
    int orientation;
    int indexCase;
    int placementsPossibles[tailleGrille * tailleGrille];
    int nbPlacementsPossibles;
    nbBateaux = 0;
    for (i = 0; i < 4; i++)
    {
        nbBateaux += tabTaillesBateaux[i];
    }
    int placementOk;
    int tabBateau[nbBateaux];
    bat = 0;
    for (i = 0; i < 4; i++)
    {
        if (i == 0)
            for (j = 0; j < tabTaillesBateaux[i]; j++)
            {
                tabBateau[bat] = 6;
                bat++;
            }
        else if (i == 1)
            for (j = 0; j < tabTaillesBateaux[i]; j++)
            {
                tabBateau[bat] = 4;
                bat++;
            }
        else if (i == 2)
            for (j = 0; j < tabTaillesBateaux[i]; j++)
            {
                tabBateau[bat] = 3;
                bat++;
            }
        else if (i == 3)
            for (j = 0; j < tabTaillesBateaux[i]; j++)
            {
                tabBateau[bat] = 2;
                bat++;
            }
    }

    for (i = 0; i < nbBateaux; i++)
    {
        //printf("IA tab bateaux[%d] = %d\n", i, tabBateau[i]);
    }
    for (i = 0; i < tailleGrille; i++)
        for (j = 0; j < tailleGrille; j++)
            grilleADV[i * tailleGrille + j] = 0;
    placementOk = 1;
    for (k = 0; k < nbBateaux; k++)
    {
        for (i = 0; i < tailleGrille * tailleGrille; i++)
            placementsPossibles[i] = -1;

        nbPlacementsPossibles = 0;

        orientation = (rand() % 2);

        nbPlacementsPossibles = 0;
        for (i = 0; i < tailleGrille; i++)
            for (j = 0; j < tailleGrille; j++)
            {
                if (place_ok(i, j, orientation, tabBateau[k]) == 1)
                {
                    placementsPossibles[nbPlacementsPossibles] = i * tailleGrille + j;
                    nbPlacementsPossibles++;
                }
            }

        if (nbPlacementsPossibles > 0)
        {
            indexCase = placementsPossibles[rand() % nbPlacementsPossibles];
            place_bat(indexCase / tailleGrille, indexCase % tailleGrille, orientation, tabBateau[k]);
        }
        else
        {
            placementOk = 0;
            break;
        }
    }

    return placementOk;
}

void IAPlacements()
{
    int i;

    for (i = 0; i < 50; i++)
    {
        if (IAPlaceBateaux() == 1)
            break;
    }
}

int IADirAttaque(int tabLigne, int tabColonne)
{
    int dirAttaque;

    dirAttaque = -1;

    if ((tabLigne - 1) >= 0)
        if (grilleIA[(tabLigne - 1) * tailleGrille + tabColonne] == 0)
            dirAttaque = 1;

    if ((tabColonne + 1) < tailleGrille)
        if (grilleIA[tabLigne * tailleGrille + (tabColonne + 1)] == 0)
            dirAttaque = 2;

    if ((tabLigne + 1) < tailleGrille)
        if (grilleIA[(tabLigne + 1) * tailleGrille + tabColonne] == 0)
            dirAttaque = 3;

    if ((tabColonne - 1) >= 0)
        if (grilleIA[tabLigne * tailleGrille + (tabColonne - 1)] == 0)
            dirAttaque = 4;

    if (tabLigne == 0)
        if (grilleIA[(tabLigne + 1) * tailleGrille + tabColonne] == 0)
            dirAttaque = 3;

    if (tabColonne == (tailleGrille - 1))
        if (grilleIA[tabLigne * tailleGrille + (tabColonne - 1)] == 0)
            dirAttaque = 4;

    if (tabLigne == (tailleGrille - 1))
        if (grilleIA[(tabLigne - 1) * tailleGrille + tabColonne] == 0)
            dirAttaque = 1;

    if (tabColonne == 0)
        if (grilleIA[tabLigne * tailleGrille + (tabColonne + 1)] == 0)
            dirAttaque = 2;

    return dirAttaque;
}

int IADirAvance(int tabLigne, int tabColonne)
{
    int caseCible;

    caseCible = -1;

    if (dirAttaque == 1)
    {
        if ((tabLigne - 1) >= 0)
            caseCible = (tabLigne - 1) * tailleGrille + tabColonne;
    }
    else if (dirAttaque == 2)
    {
        if ((tabColonne + 1) < tailleGrille)
            caseCible = tabLigne * tailleGrille + (tabColonne + 1);
    }
    else if (dirAttaque == 3)
    {
        if ((tabLigne + 1) < tailleGrille)
            caseCible = (tabLigne + 1) * tailleGrille + tabColonne;
    }
    else if (dirAttaque == 4)
    {
        if ((tabColonne - 1) >= 0)
            caseCible = tabLigne * tailleGrille + (tabColonne - 1);
    }

    return caseCible;
}

void IADirInversion()
{
    int tempIndex;

    if (dirAttaque == 1)
        dirAttaque = 3;
    else if (dirAttaque == 2)
        dirAttaque = 4;
    else if (dirAttaque == 3)
        dirAttaque = 1;
    else if (dirAttaque == 4)
        dirAttaque = 2;

    tempIndex = premierPoint;
    premierPoint = dernierPoint;
    dernierPoint = tempIndex;
}

void IAFinal(int ligne, int colonne, int direction)
{
    if (direction == 1)
    {
        ecrire_case(ligne - 1, colonne - 1, 3);
        ecrire_case(ligne - 1, colonne, 3);
        ecrire_case(ligne - 1, colonne + 1, 3);
    }
    else if (direction == 2)
    {
        ecrire_case(ligne + 1, colonne + 1, 3);
        ecrire_case(ligne, colonne + 1, 3);
        ecrire_case(ligne - 1, colonne + 1, 3);
    }
    else if (direction == 3)
    {
        ecrire_case(ligne + 1, colonne + 1, 3);
        ecrire_case(ligne + 1, colonne, 3);
        ecrire_case(ligne + 1, colonne - 1, 3);
    }
    else if (direction == 4)
    {
        ecrire_case(ligne + 1, colonne - 1, 3);
        ecrire_case(ligne, colonne - 1, 3);
        ecrire_case(ligne - 1, colonne - 1, 3);
    }
}

void IALateral(int ligne, int colonne, int direction)
{
    if (direction == 1)
    {
        while (grilleIA[ligne * tailleGrille + colonne] == 2)
        {
            ecrire_case(ligne, colonne + 1, 3);
            ecrire_case(ligne, colonne - 1, 3);
            ligne++;
        }
    }
    else if (direction == 2)
    {
        while (grilleIA[ligne * tailleGrille + colonne] == 2)
        {
            ecrire_case(ligne + 1, colonne, 3);
            ecrire_case(ligne - 1, colonne, 3);
            colonne--;
        }
    }
    else if (direction == 3)
    {
        while (grilleIA[ligne * tailleGrille + colonne] == 2)
        {
            ecrire_case(ligne, colonne + 1, 3);
            ecrire_case(ligne, colonne - 1, 3);
            ligne--;
        }
    }
    else if (direction == 4)
    {
        while (grilleIA[ligne * tailleGrille + colonne] == 2)
        {
            ecrire_case(ligne + 1, colonne, 3);
            ecrire_case(ligne - 1, colonne, 3);
            colonne++;
        }
    }
}

void IATotal(int ligne, int colonne, int direction, int taille_bateau)
{
    int compteur;
    compteur = 1;

    if (direction == 1)
    {
        while (compteur <= taille_bateau)
        {
            if (compteur == 1)
            {
                ecrire_case(ligne, colonne - 1, 3);
                ecrire_case(ligne - 1, colonne - 1, 3);
                ecrire_case(ligne - 1, colonne, 3);
                ecrire_case(ligne - 1, colonne + 1, 3);
                ecrire_case(ligne, colonne + 1, 3);
            }
            else if (compteur == taille_bateau)
            {
                ecrire_case(ligne, colonne + 1, 3);
                ecrire_case(ligne + 1, colonne + 1, 3);
                ecrire_case(ligne + 1, colonne, 3);
                ecrire_case(ligne + 1, colonne - 1, 3);
                ecrire_case(ligne, colonne - 1, 3);
            }
            else
            {
                ecrire_case(ligne, colonne - 1, 3);
                ecrire_case(ligne, colonne + 1, 3);
            }
            ligne++;
            compteur++;
        }
    }
    else if (direction == 2)
    {
        while (compteur <= taille_bateau)
        {
            if (compteur == 1)
            {
                ecrire_case(ligne - 1, colonne, 3);
                ecrire_case(ligne - 1, colonne + 1, 3);
                ecrire_case(ligne, colonne + 1, 3);
                ecrire_case(ligne + 1, colonne + 1, 3);
                ecrire_case(ligne + 1, colonne, 3);
            }
            else if (compteur == taille_bateau)
            {
                ecrire_case(ligne + 1, colonne, 3);
                ecrire_case(ligne + 1, colonne - 1, 3);
                ecrire_case(ligne, colonne - 1, 3);
                ecrire_case(ligne - 1, colonne - 1, 3);
                ecrire_case(ligne - 1, colonne, 3);
            }
            else
            {
                ecrire_case(ligne + 1, colonne, 3);
                ecrire_case(ligne - 1, colonne, 3);
            }
            colonne--;
            compteur++;
        }
    }
    else if (direction == 3)
    {
        while (compteur <= taille_bateau)
        {
            if (compteur == 1)
            {
                ecrire_case(ligne, colonne + 1, 3);
                ecrire_case(ligne + 1, colonne + 1, 3);
                ecrire_case(ligne + 1, colonne, 3);
                ecrire_case(ligne + 1, colonne - 1, 3);
                ecrire_case(ligne, colonne - 1, 3);
            }
            else if (compteur == taille_bateau)
            {
                ecrire_case(ligne, colonne - 1, 3);
                ecrire_case(ligne - 1, colonne - 1, 3);
                ecrire_case(ligne - 1, colonne, 3);
                ecrire_case(ligne - 1, colonne + 1, 3);
                ecrire_case(ligne, colonne + 1, 3);
            }
            else
            {
                ecrire_case(ligne, colonne - 1, 3);
                ecrire_case(ligne, colonne + 1, 3);
            }
            ligne--;
            compteur++;
        }
    }
    else if (direction == 4)
    {
        while (compteur <= taille_bateau)
        {
            if (compteur == 1)
            {
                ecrire_case(ligne + 1, colonne, 3);
                ecrire_case(ligne + 1, colonne - 1, 3);
                ecrire_case(ligne, colonne - 1, 3);
                ecrire_case(ligne - 1, colonne - 1, 3);
                ecrire_case(ligne - 1, colonne, 3);
            }
            else if (compteur == taille_bateau)
            {
                ecrire_case(ligne - 1, colonne, 3);
                ecrire_case(ligne - 1, colonne + 1, 3);
                ecrire_case(ligne, colonne + 1, 3);
                ecrire_case(ligne + 1, colonne + 1, 3);
                ecrire_case(ligne + 1, colonne, 3);
            }
            else
            {
                ecrire_case(ligne + 1, colonne, 3);
                ecrire_case(ligne - 1, colonne, 3);
            }
            colonne++;
            compteur++;
        }
    }
}

void IACibleTir(int *ligne, int *colonne)
{
    int i;
    int j;

    int nbAttaquesPossibles;
    int attaquesPossibles[tailleGrille * tailleGrille];

    int cibleIndex;

    int posLigne;
    int posColonne;

    if (attaqueEnCours == 0)
    {
        for (i = 0; i < tailleGrille * tailleGrille; i++)
            attaquesPossibles[i] = -1;

        nbAttaquesPossibles = 0;
        for (i = 0; i < tailleGrille; i++)
            for (j = 0; j < tailleGrille; j++)
            {
                if (grilleIA[i * tailleGrille + j] == 0)
                {
                    attaquesPossibles[nbAttaquesPossibles] = i * tailleGrille + j;
                    nbAttaquesPossibles++;
                }
            }

        srand(time(NULL));
        cibleIndex = attaquesPossibles[rand() % nbAttaquesPossibles];
    }
    else
    {
        posLigne = dernierPoint / tailleGrille;
        posColonne = dernierPoint % tailleGrille;
        cibleIndex = IADirAvance(posLigne, posColonne);
    }

    positionTir = cibleIndex;
    *ligne = cibleIndex / tailleGrille;
    *colonne = cibleIndex % tailleGrille;
}

void IARetourTir(int resultatTir)
{
    int posIndex;

    if (attaqueEnCours == 0)
    {
        if (resultatTir == 1)
        {
            grilleIA[positionTir] = 3;
        }
        else if ((resultatTir == 2) || (resultatTir == 3)) // Tir touche bateau
        {
            grilleIA[positionTir] = 2;
            attaqueEnCours = 1;
            dirAttaque = IADirAttaque(positionTir / tailleGrille, positionTir % tailleGrille);
            nbPoints++;
            premierPoint = positionTir;
            dernierPoint = positionTir;
        }

        positionTir = -1;
    }
    else
    {
        if (resultatTir == 1)
        {
            if (nbPoints == 1)
            {
                grilleIA[positionTir] = 3;

                IAFinal(dernierPoint / tailleGrille, dernierPoint % tailleGrille, dirAttaque);

                dirAttaque = IADirAttaque(dernierPoint / tailleGrille, dernierPoint % tailleGrille);
            }
            else if (nbPoints > 1)
            {
                grilleIA[positionTir] = 3;

                IAFinal(dernierPoint / tailleGrille, dernierPoint % tailleGrille, dirAttaque);

                IADirInversion();
            }
        }
        else if (resultatTir == 2)
        {

            grilleIA[positionTir] = 2;
            nbPoints++;

            dernierPoint = positionTir;

            IALateral(dernierPoint / tailleGrille, dernierPoint % tailleGrille, dirAttaque);

            posIndex = IADirAvance(dernierPoint / tailleGrille, dernierPoint % tailleGrille);

            if (posIndex == -1)
            {
                IAFinal(dernierPoint / tailleGrille, dernierPoint % tailleGrille, dirAttaque);

                IADirInversion();
            }
        }
        else if (resultatTir == 3)
        {

            grilleIA[positionTir] = 2;
            nbPoints++;

            dernierPoint = positionTir;

            IATotal(dernierPoint / tailleGrille, dernierPoint % tailleGrille, dirAttaque, nbPoints);

            attaqueEnCours = 0;
            dirAttaque = 0;
            nbPoints = 0;
            premierPoint = -1;
            dernierPoint = -1;
        }

        positionTir = -1;
    }
}
