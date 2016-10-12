#ifndef __EVALUATION__
#define __EVALUATION__

#include "data.h"
#include <stdio.h>

/** Rédigé par Pacôme Perrotin
*
* Ce module fournit les outils permettant d'évaluer l'étiquettage d'un
* algorithme.
*/

/**
* La fonction suivante compare le tableau d'étiquettes en paramètres avec
* celui du fichier de test pour donner la précision du résultat.
*/

double compare_labels_results(GlobalData * data, int * approximated_labels);

#endif
