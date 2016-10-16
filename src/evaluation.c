#include "evaluation.h"

/**
* Cette fonction compare le tableau d'étiquettes donné en paramètre avec
* les vraies étiquettes contenues dans les données de test.
*/
double compare_labels_results(GlobalData * data, int * approximated_labels) {

	// Récupération des données de test
	int * words = data->test_words;
	int * real_labels = data->test_labels;
	int size = data->test_size;

	// On commence à compter le nombre d'erreur
	int errors = 0;
	int empty_words = 0;
	for(int k = 0; k < size; k++) {
		// On rencontre une erreur
		if( real_labels[k] != approximated_labels[k] ) {
			if( can_shout(data) )
				printf("at %5d = %5d : X %2d X -> %2d\n", k + 1, words[k],
													approximated_labels[k],
													real_labels[k]);
			errors++;
		}
		else if( ! real_labels[k] && ! approximated_labels[k] )
			// S'il y a un espace entre deux phrases, on le compte
			empty_words++;
	}

	// Pour s'assurer d'une précision correcte, on enlève les espaces vides
	// avant les phrase au compte de la taille du document
	size -= empty_words;

	// La précision est simplement l'inverse du taux d'erreur.
	double accuracy = 1 - ( errors / (double) size );

	if( can_speak(data) ) {
		if( ! errors )
			printf("Aucune erreur détectée.\n");
		else
			printf("Taux de précision : %lf ( %d \\ %d )\n",
				accuracy,
				size - errors, size);
	}

	return accuracy;
}
