#ifndef __VITERBI__
#define __VITERBI__

#include "hmm.h"
#include "hmm_init.h"
#include "log_sum.h"
#include "parser.h"
#include "evaluation.h"
#include "parameters.h"
#include <stdio.h>
#include <math.h>
#include <float.h>

/** Rédigé par Pacôme Perrotin
*
* Ce module présente des fonctions d'implémentation de l'algorithme de Viterbi.
* Ce calcul repose sur les chaînes cachées de markov implémentées en forme
* par le module hmm.
*/

typedef void (*HmmGenerator)(Hmm *, char *);

/**
* La fonction suivante calcule la précision obtenue par l'algorithme de viterbi
* sur les corpus en paramètre. C'est la fonction principale.
*/

double viterbi(char * train_corpus, char * test_corpus, HmmGenerator generator);

/**
* La fonction suivante calcule les étiquettes des mots passés en paramètre
* à partir du hmm passé en paramètre et écrit ces étiquettes dans le paramètre
* labels.
* Cette fonction ne doit être utilisée que pour lire une seule phrase.
*/

void predict_viterbi(Hmm * hmm, int * words, int * labels, int size);

/**
* La fonction suivante extrait les mots du fichier en paramètre, puis calcule
* les étiquettes correspondantes via l'algorithme de Viterbi, puis enfin
* compare les résultats avec les étiquettes inscrites dans le fichier.
*/

double predict_and_compare_viterbi(Hmm * hmm, char * file_name);

/**
* Fonction qui à partir d'un indice et d'un tableau, renvoie le prochain indice
* à 0.
*/

int get_next_space(int * words, int i, int size);

#endif
