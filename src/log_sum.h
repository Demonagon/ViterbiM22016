#ifndef __LOG_SUM__
#define __LOG_SUM__

/** Rédigé par Pacôme Perrotin
*
* Ce module fournit les fonctions nécessaires à
* la manipulation de probabilités faibles voir
* très faibles grâce à leur représentation sous
* forme de logarithme.
* Une forme logarithmique d'une probabilité est
* exprimée sous le type double.
*
*/

#include <stdio.h>
#include <math.h>

/** 
* Fournit -log(n/total). Ce calcul est effectué via
* log(total) - log(n).
* Ici n et total ne sont pas des probabilités sous
* forme logarithmique ; ils doivent être des nombre
* entier correspondant par exemple à des nombres
* d'occurence.
*/
double log_occurence_probability(int n, int total);

/**
* En supposant que a représente une probabilité
* sous forme -log(a') et b représente une probabilité
* sous forme -log(b'), cette fonction calcule le
* produit des probabilités via a + b.
*/
double log_probability_mult(double a, double b);

/* Fonctions de test */

void main_test_log_01();

#endif
