#ifndef __HMM_INIT__
#define __HMM_INIT__

#include "hmm.h"
#include "log_sum.h"
#include "parser.h"
#include "parameters.h"
#include "viterbi.h"

/** Rédigé par Pacôme Perrotin
*
* Ce module utilise les modules hmm, log_sum et parser pour initialiser un hmm
* sous modèle génératif.
*/

/**
* Structure de donnée qui permet d'analyser un corpus dans son ensemble,
* par la simple addition des occurences observées. Ces occurences peuvent être
* additionnées à celles d'un ensemble de corpus, puis extraites sous la forme
* de probabilités logarithmiques dans un hmm.
*/

typedef struct {
	int states_nb;
	int observables_nb;
	int size;

	int * first_occurences;
	int sentences;
	int ** transition_occurences;
	int ** emission_occurences;
	int * category_occurences;

	GlobalData * data;
} CorpusAnalyser;

void init_corpus_analyser(CorpusAnalyser * ca, int states_nb,
						  int observables_nb, GlobalData * data);

void free_corpus_analyser(CorpusAnalyser * ca);

void analyse_sentence(CorpusAnalyser * ca, int * words, int * label, int size);

void analyse_corpus(CorpusAnalyser * ca);

void extract_hmm(CorpusAnalyser * ca, Hmm * hmm);

/**
* Cette fonction vérifie le nombre de lignes des fichiers voc_etats.txt et
* voc_observables.txt pour initialiser un hmm vide de la bonne taille.
*/

Hmm * init_hmm_by_corpus();

/**
* En supposant hmm un hmm initialisé à la bonne taille, cette fonction remplit
* le hmm avec les informations que l'on peut lire à partir du corpus donné
* en paramètre, en ne considérant qu'une fraction de ce fichier définie par
* corpus_size.
*/

void compute_corpus(Hmm * hmm);

#endif
