#ifndef __DATA__
#define __DATA__

#include "parameter.h"

/** Rédigé par Pacôme Perrotin
* Ce module a pour mission de fournir une structure de donnée capable de
* contenir toutes les informations globale du programme. Ces informations
* concernent les données d'apprentissage et de test, ainsi que les divers
* paramètres passés en ligne de commande.
* Le principal intêret de construire une telle structure est de permettre
* une lecture minimale des fichiers d'apprentissage et de tests.
*/

typedef struct {
	GlobalParameters execution_parameters;
	
	int * learning_words;
	int * learning_labels;
	int   learning_size;
	int * test_words;
	int * test_labels;
	int   test_size;
} GlobalData;

/**
* Initialise la structure derrière le pointeur data.
*/

void init_data(GlobalData * data, int argc, char ** argv);

/**
* S'occupe de la partie de l'initialisation qui concerne l'ouverture des
* fichiers et la lecture des mots et étiquettes présentes dans les fichiers
*/

void extract_data(GlobalData * data, char * learning_file, char * test_file);

/**
* Fonctions dont le rôle accumulé correspond à celui de la fonction du dessus.
*/

void extract_learning_data(Global * data, char * learning_file);
void extract_test_data(Global * data, char * test_file);

/**
* Libère la mémoire occupée par la structure.
*/

void free_data(GlobalData * data);

#endif
