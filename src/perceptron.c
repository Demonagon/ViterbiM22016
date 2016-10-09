#include "perceptron.h"

char is_prediction_wrong_perceptron(int * real_labels,
									int * approx_labels,
									int size) {
	for(int k = 0; k < size; k++)
		if( real_labels[k] != approx_labels[k] ) return 1;
	return 0;
}

void update_transition(Hmm * hmm, int * real_labels, int * approximated_labels,
					   int size) {
	int previous_real_state = real_labels[0] - 1;
	int previous_approximated_state = approximated_labels[0] - 1;
	for(int k = 1; k < size; k++) {
		int real_state = real_labels[k] - 1;
		int approximated_state = approximated_labels[k] - 1;
		hmm->T[previous_real_state][real_state]--;
		hmm->T[previous_approximated_state][approximated_state]++;
		previous_real_state = real_state;
		previous_approximated_state = approximated_state;
	}
}

void update_emission(Hmm * hmm, int * words,
					 int * real_labels, int * approximated_labels,
					 int size) {
	for(int word_index = 0; word_index < size; word_index++) {
		int word = words[word_index] - 1;
		int real_state = real_labels[word_index] - 1;
		int approximated_state = approximated_labels[word_index] - 1;
		hmm->E[real_state][word]--;
		hmm->E[approximated_state][word]++;
	}
}

void update_initial(Hmm * hmm, int * real_labels, int * approximated_labels) {
	hmm->PI[real_labels[0] - 1]--;
	hmm->PI[approximated_labels[0] - 1]++;
	/*printf("PI[%d] --\n", real_labels[0] - 1);
	printf("PI[%d] ++\n", approximated_labels[0] - 1);*/
}

void update_perceptron(Hmm * hmm, int * words, int * real_labels,
					   int * approximated_labels, int size) {
	update_transition(hmm, real_labels, approximated_labels, size);
	update_emission(hmm, words, real_labels, approximated_labels, size);
	update_initial(hmm, real_labels, approximated_labels);
}

void perceptron_compute_corpus(Hmm * hmm, char * corpus_file) {
	FILE * corpus = fopen(corpus_file, "r");

	//initialize_hmm(hmm);

	int size = 0;

	int * words = extract_words(corpus, &size);

	rewind(corpus);

	int * real_labels = extract_labels(corpus, &size);

	fclose(corpus);

	int approximated_labels[size];

	printf("Calcul du perceptron : \n[");

	fflush( stdout );

	for(int i = 0; i < GLOBAL_PARAMETERS.perceptron_iteration_number; i++) {

		printf(".");

		fflush( stdout );

		int k = 0;
		int space = get_next_space(words, k, size);


		while(k + 1 < size) {
			predict_viterbi(hmm, words + k, approximated_labels + k, space - k);

			/*printf("prediction = {\n");
			for(int l = 0; l < space - k; l++)
				printf("%d %d\n", (words + k)[l], (approximated_labels + k)[l]);
			printf("}\n");*/

			if( is_prediction_wrong_perceptron(
				real_labels + k, approximated_labels + k, space - k) )
				update_perceptron(hmm, words + k, real_labels + k,
									  approximated_labels + k, space - k);

			k = space + 1;
			approximated_labels[space] = 0;
			space = get_next_space(words, k, size);
		}
	}

	printf("]\n");

	fflush(stdout);
}
