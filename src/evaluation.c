#include "evaluation.h"


double compare_labels_results(int * words,
							int * real_labels,
							int * approximated_labels,
							int size) {
	int errors = 0;
	int empty_words = 0;
	for(int k = 0; k < size; k++) {
		if( real_labels[k] != approximated_labels[k] ) {
			if( GLOBAL_PARAMETERS.noise >= VERBOSE )
				printf("at %5d = %5d : X %2d X -> %2d\n", k + 1, words[k],
													approximated_labels[k],
													real_labels[k]);
			errors++;
		}
		else if( ! real_labels[k] && ! approximated_labels[k] )
			empty_words++;
	}

	size -= empty_words;

	double accuracy = 1 - ( errors / (double) size );

	if( GLOBAL_PARAMETERS.noise >= LOUD ) {
		if( ! errors )
			printf("Aucune erreur détectée.\n");
		else
			printf("Taux de précision : %lf ( %d \\ %d )\n",
				accuracy,
				size - errors, size);
	}

	return accuracy;
}
