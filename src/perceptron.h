#ifndef __PERCEPTRON__
#define __PERCEPTRON__

#include "hmm.h"
#include "parser.h"
#include "viterbi.h"
#include <stdio.h>

/** Rédigé par Pacôme Perrotin
*
* Ce module fournit les outils permettant de prédire les étiquettes d'une
* séquence de phrases par l'intermédiaire de l'algorithme du perceptron.
*/

void perceptron_compute_corpus(Hmm * hmm, char * corpus_file);

#endif
