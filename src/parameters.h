#ifndef __PARAMETERS__
#define __PARAMETERS__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/** Rédigé par Pacôme Perrotin
*
* Ce module particulier permet un stockage pratique des paramètres donnés
* en ligne de commande du programme.
*/

/**
* Paramètre de bruit :
* default -> LOUD
* "-s" -> QUIET
* "-v" -> VERBOSE
*/

typedef enum {
	QUIET = 0,
	LOUD = 1,
	VERBOSE = 2
} NoiseParameter;

typedef enum {
	EXECUTE_ALL = 0,
	COUNT = 1,
	PERCEPTRON = 2
} ExecutionParameter;

typedef struct {
	NoiseParameter noise;
	ExecutionParameter execution;
	double corpus_size;
	int perceptron_iteration_number;
	double smoothing_value;
} GlobalParameters;

/**
* GLOBAL_PARAMETERS est une variable accessible en tout point du programme qui
* stocke les paramètres en question.
*/

extern GlobalParameters GLOBAL_PARAMETERS;

void read_parameters(int argc, char ** argv);

void configure_parameters(GlobalParameters configuration);

void print_parameters_note(FILE * out, char ** argv);

char can_speak();
char can_shout();

#endif
