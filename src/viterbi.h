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

/**
* Ici est représentée la définition d'un type de fonction génératrice de hmm.
* Cette définition nous permet de remplacer un algorithme de perceptron par
* un algorithme de calcul de probabilités par fréquences relatives simplement
* sans avoir à reprogrammer tout l'algorithme de viterbi.
* Le premier paramètre correspond au hmm à initialiser, le second au nom du
* fichier contenant le corpus d'apprentissage, et enfin le dernier paramètre
* donne le pourcentage du corpus d'apprentissage à considérer.
*/

typedef void (*HmmGenerator)(Hmm *, char *, double);

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
