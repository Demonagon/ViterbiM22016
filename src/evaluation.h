#ifndef __EVALUATION__
#define __EVALUATION__

#include "parameters.h"
#include <stdio.h>

/** Rédigé par Pacôme Perrotin
*
* Ce module fournit les outils permettant d'évaluer l'étiquettage d'un
* algorithme.
*/

/**
* La fonction suivante compare les deux tableaux d'étiquettes en paramètres
* pour estimer et afficher l'erreur de calcul.
*/

double compare_labels_results(int * words,
							  int * real_labels,
							  int * approximated_labels,
							  int size);

#endif
