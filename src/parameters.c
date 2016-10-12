#include "parameters.h"

GlobalParameters GLOBAL_PARAMETERS;

/** LECTURE DES PARAMETRES **/

void read_help_parameter(char ** argv) {
	print_parameters_note(stdout, argv);
	exit(0);
}

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

void read_counting_parameter(GlobalParameters * configuration) {
	if( configuration->execution != PERCEPTRON )
		configuration->execution = COUNT;
	else {
		fprintf(stderr, "Les paramètres P et C ne sont pas compatibles.");
		fprintf(stderr, " Le paramètre C est ignoré.\n");
	}
}

void read_perceptron_parameter(GlobalParameters * configuration) {
	if( configuration->execution != COUNT )
		configuration->execution = PERCEPTRON;
	else {
		fprintf(stderr, "Les paramètres C et P ne sont pas compatibles.");
		fprintf(stderr, " Le paramètre P est ignoré.\n");
	}
}

void read_corpus_size_parameter(GlobalParameters * configuration, double c) {
	if( c < 0 || c > 1 ) {
		fprintf(stderr, "Le paramètre c doit être dans [0, 1].\n");
		exit(-1);
	}

	configuration->corpus_size = c;
}

void read_perceptron_iteration_parameter(GlobalParameters * configuration,
										 int i) {
	if( i < 0 ) {
		fprintf(stderr, "Le paramètre I doit être supérieur à 0.\n");
		exit(-1);
	}

	configuration->perceptron_iteration_number = i;
}

void read_smoothing_parameter(GlobalParameters * configuration,
										 double a) {
	if( a < 0 ) {
		fprintf(stderr, "Le paramètre a doit être supérieur à 0.\n");
		exit(-1);
	}

	configuration->smoothing_value = a;
}

/** LECTURE GLOBALE **/

void read_parameter(GlobalParameters * configuration, char * parameter,
					char ** argv) {
	if( ! parameter ) return;
	if( parameter[0] != '-' ) return;

	int i;
	double c, a;

	if( ! strcmp( parameter, "--help" ) || ! strcmp( parameter, "-h" ) )
		read_help_parameter(argv);
	else if( ! strcmp( parameter, "-s" ) )
		read_silent_parameter(configuration);
	else if ( ! strcmp( parameter, "-v" ) )
		read_verbose_parameter(configuration);
	else if ( ! strcmp( parameter, "-C" ) )
		read_counting_parameter(configuration);
	else if ( ! strcmp( parameter, "-P" ) )
		read_perceptron_parameter(configuration);
	else if( sscanf(parameter, "-c=%lf", &c) == 1 )
		read_corpus_size_parameter(configuration, c);
	else if( sscanf(parameter, "-I=%d", &i) == 1 )
		read_perceptron_iteration_parameter(configuration, i);
	else if( sscanf(parameter, "-a=%lf", &a) == 1 )
		read_smoothing_parameter(configuration, a);
}

void read_parameters(int argc, char ** argv)
{
	if( argc < 3 ) {
		fprintf(stdout, "Pas assez d'arguments pour exécuter le programme.\n");
		print_parameters_note(stdout, argv);
		exit(0);
	}

	argc -= 3;
	argv += 3;

	GlobalParameters configuration = (GlobalParameters) {
		.noise = LOUD,
		.execution = EXECUTE_ALL,
		.corpus_size = 1.,
		.perceptron_iteration_number = 10,
		.smoothing_value = 0
	};

	for(int k = 0; k < argc; k++)
		read_parameter(&configuration, argv[k], argv);

	configure_parameters(configuration);
}

void configure_parameters(GlobalParameters configuration)
{
	GLOBAL_PARAMETERS = configuration;
}

void print_parameters_note(FILE * out, char ** argv)
{
	fprintf(out, "* Notes d'usages :\n");
	fprintf(out, "*  - %s [1] [2] [...]\n", argv[0]);
	fprintf(out, "* Avec :\n\t[1] - un chemin vers le corpus d'entraînement\n");
	fprintf(out, "* \t[2] - un chemin vers le corpus de test\n");
	fprintf(out, "* \t[...] - une liste optionnelle de paramètres parmis :\n");
	fprintf(out, "* \t\t\"-s\" - mode silencieux\n");
	fprintf(out, "* \t\t\"-v\" - mode détaillé\n");
	fprintf(out, "* \t\t\"-C\" - prédire uniquement en comptant les occurences\n");
	fprintf(out, "* \t\t\"-P\" - prédire uniquement grâce au perceptron\n");
	fprintf(out, "* \t\t\"-c=x\" ([0, 1]) - règle le pourcentage du corpus");
	fprintf(out, " à considérer dans les calculs\n");
	fprintf(out, "* \t\t\"-I=x\" (>= 0, entier) - règle le nombre");
	fprintf(out, " d'itérations de l'algorithme de Perceptron\n");
	fprintf(out, "* \t\t\"-a=x\" (>= 0) - règle la valeur");
	fprintf(out, " de lissage des données probabilistes\n");
	fprintf(out, "* \t\t\"-h\" ou \"--help\" - affiche cette aide à la place");
	fprintf(out, " d'exécuter le programme\n");
}

char can_speak() {
	return GLOBAL_PARAMETERS.noise >= LOUD;
}

char can_shout() {
	return GLOBAL_PARAMETERS.noise >= VERBOSE;
}
