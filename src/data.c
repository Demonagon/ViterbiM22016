#include "data.h"
#include "parser.h"

void init_data(GlobalData * data, int argc, char ** argv)
{
	data->learning_words = NULL;
	data->learning_labels = NULL;
	data->test_words = NULL;
	data->test_labels = NULL;
	
	read_parameters(&(data->execution_parameters), argc, argv);

	extract_data(data, argv[1], argv[2]);
}

/**
* S'occupe de la partie de l'initialisation qui concerne l'ouverture des
* fichiers et la lecture des mots et étiquettes présentes dans les fichiers
*/

void extract_data(GlobalData * data, char * learning_file, char * test_file) {
	extract_learning_data(data, learning_file);
	extract_test_data(data, test_file);
}

/**
* Fonctions dont le rôle accumulé correspond à celui de la fonction du dessus.
*/

void extract_learning_data(GlobalData * data, char * learning_file) {
	FILE * corpus = fopen(learning_file, "r");

	if( ! corpus ) {
		fprintf(stderr, "\"%s\" n'est pas un fichier d'apprentissage valide.\n",
				learning_file);
		exit(-1);
	}

	if( can_speak(data) ) {
		printf("Ouverture du corpus d'apprentissage.\n");
		fflush(stdout);
	}

	if( can_speak(data) ) {
		printf(" - > Extraction des mots étiquetés...\n");
		fflush( stdout );
	}

	data->learning_sentences_count = extract_labels_and_words(data, corpus,
		&(data->learning_words), 
		&(data->learning_labels),
		&(data->learning_size),
		data->execution_parameters.corpus_size);

	if( can_speak(data) ) {
		printf(" - > Terminé.\n");
		fflush(stdout);
	}

	fclose(corpus);
}

void extract_test_data(GlobalData * data, char * test_file) {
	FILE * corpus = fopen(test_file, "r");

	if( ! corpus ) {
		fprintf(stderr, "\"%s\" n'est pas un fichier de test valide.\n",
				test_file);
		exit(-1);
	}

	if( can_speak(data) ) {
		printf("Ouverture du corpus de test.\n");
		fflush(stdout);
	}

	if( can_speak(data) ) {
		printf(" - > Extraction des mots étiquetés...\n");
		fflush( stdout );
	}

	data->test_sentences_count = extract_labels_and_words(data, corpus,
		&(data->test_words), 
		&(data->test_labels),
		&(data->test_size),
		1.);

	if( can_speak(data) ) {
		printf(" - > Terminé.\n");
		fflush(stdout);
	}

	fclose(corpus);
}

/**
* Libère la mémoire occupée par la structure.
*/

void free_data(GlobalData * data) {
	free(data->learning_words);
	free(data->learning_labels);
	free(data->test_words);
	free(data->test_labels);
}

char can_speak(GlobalData * data) {
	return data->execution_parameters.noise >= LOUD;
}

char can_shout(GlobalData * data) {
	return data->execution_parameters.noise >= VERBOSE;
}
