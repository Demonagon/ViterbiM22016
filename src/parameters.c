#include "parameters.h"

GlobalParameters GLOBAL_PARAMETERS;

/** LECTURE DES PARAMETRES **/

void read_silent_parameter(GlobalParameters * configuration) {
	if( configuration->noise != VERBOSE )
		configuration->noise = QUIET;
	else {
		fprintf(stderr, "Les paramètres v et s ne sont pas compatibles.");
		fprintf(stderr, " Le paramètre s est ignoré.\n");
	}
}

void read_verbose_parameter(GlobalParameters * configuration) {
	if( configuration->noise != QUIET )
		configuration->noise = VERBOSE;
	else {
		fprintf(stderr, "Les paramètres s et v ne sont pas compatibles.");
		fprintf(stderr, " Le paramètre v est ignoré.\n");
	}
}

void read_perceptron_iteration_parameter(GlobalParameters * configuration,
										 int i) {
	configuration->perceptron_iteration_number = i;
}

/** LECTURE GLOBALE **/

void read_parameter(GlobalParameters * configuration, char * parameter) {
	if( ! parameter ) return;
	if( parameter[0] != '-' ) return;

	if( ! strcmp( parameter, "-s" ) )
		read_silent_parameter(configuration);
	else if ( ! strcmp( parameter, "-v" ) )
		read_verbose_parameter(configuration);

	int i;

	if( sscanf(parameter, "-I=%d", &i) == 1 )
		read_perceptron_iteration_parameter(configuration, i);
}

void read_parameters(int argc, char ** argv)
{
	GlobalParameters configuration = (GlobalParameters) {
		.noise = LOUD,
		.perceptron_iteration_number = 100
	};

	for(int k = 0; k < argc; k++)
		read_parameter(&configuration, argv[k]);

	configure_parameters(configuration);
}

void configure_parameters(GlobalParameters configuration)
{
	GLOBAL_PARAMETERS = configuration;
}

void print_parameters_note(FILE * out, char ** argv)
{
	fprintf(out, "Pas assez d'arguments pour procéder aux tests.\n");
	fprintf(out, "Notes d'usages :\n");
	fprintf(out, " - %s [1] [2] [...]\n", argv[0]);
	fprintf(out, "Avec :\n\t[1] - un chemin vers le corpus d'entraînement\n");
	fprintf(out, "\t[2] - un chemin vers le corpus de test\n");
	fprintf(out, "\t[...] - une liste optionnelle de paramètres parmis :\n");
	fprintf(out, "\t\t\"-s\" - mode silencieux\n");
	fprintf(out, "\t\t\"-v\" - mode détaillé\n");
}
