#include "viterbi.h"

double viterbi(HmmGenerator generator, GlobalData * data) {
	if( can_speak(data) )
		printf("Entraînement...\n");

	fflush(stdout);

	Hmm * hmm = init_hmm_by_corpus();
	hmm->data = data;

	generator(hmm);

	if( can_speak(data) ) {
		printf("Terminé.\n");
		printf("Test...\n");
	}

	fflush(stdout);

	double accuracy = predict_and_compare_viterbi(hmm);

	if( can_speak(data) )
		printf("Terminé.\n");

	free_hmm(hmm);

	return accuracy;
}

void viterbi_initialization(Hmm * hmm, int * words, int size, double ** score,
							int ** backtrack) {

	int first_word = words[0] - 1;

	for(int k = 0; k < hmm->nbe; k++) {
		score[k][0] = log_probability_mult(hmm->PI[ k ],
										   hmm->E[k][ first_word ]);
		//printf("score[%d][0] = %lf\n", k, score[k][0]);
		backtrack[k][0] = 0;
	}
}

double viterbi_min_k(Hmm * hmm, int i, int state, int * words, double ** score) {
	double min_k_score = INFINITY;

	for(int k = 0; k < hmm->nbe; k++) {
		double k_score = log_probability_mult3(
				score[k][i - 1],
				hmm->T[k][state],
				hmm->E[state][ words[i] - 1 ] );
		if( k_score < min_k_score )
			min_k_score = k_score;
	}

	return min_k_score;
}

int viterbi_arg_min_k(Hmm * hmm, int i, int state, double ** score) {
	int min_k = 0;
	double min_k_score = INFINITY;

	for(int k = 0; k < hmm->nbe; k++) {
		double k_score = log_probability_mult(
				score[k][i - 1],
				hmm->T[k][state]);
		if( k_score < min_k_score ) {
			min_k = k;
			min_k_score = k_score;
		}
	}

	return min_k;
}

void viterbi_recursion(Hmm * hmm, int * words, int size, double ** score,
					   int ** backtrack) {

	for(int i = 1; i < size; i++) {
		for(int state = 0; state < hmm->nbe; state++) {
			score[state][i] = viterbi_min_k(hmm, i, state, words, score);
			backtrack[state][i] = viterbi_arg_min_k(hmm, i, state, score);
		}
	}
}

void viterbi_output(Hmm * hmm, int * labels, int size, double ** score,
					int ** backtrack) {

	int last_label = 0;
	double last_label_score = INFINITY;

	for(int k = 0; k < hmm->nbe; k++)
		if( score[k][size - 1] < last_label_score ) {
			last_label = k;
			last_label_score = score[k][size - 1];
		}

	labels[size - 1] = last_label + 1;

	int next_label = last_label;

	for(int k = size - 1; k > 0; k--) {
		labels[k - 1] = backtrack[next_label][k] + 1;
		next_label = backtrack[next_label][k];
	}
}

void predict_viterbi(Hmm * hmm, int * words, int * labels, int size)
{
	double ** score = malloc( sizeof(double * ) * hmm->nbe);
	int ** backtrack = malloc( sizeof(int * ) * hmm->nbe);

	for(int k = 0; k < hmm->nbe; k++) {
		score[k] = malloc(sizeof(double) * size);
		backtrack[k] = malloc(sizeof(int) * size);
	}

	viterbi_initialization(hmm, words, size, score, backtrack);

	viterbi_recursion(hmm, words, size, score, backtrack);

	viterbi_output(hmm, labels, size, score, backtrack);

	for(int k = 0; k < hmm->nbe; k++) {
		free(score[k]);
		free(backtrack[k]);
	}

	free( score );
	free( backtrack );
}

double predict_and_compare_viterbi(Hmm * hmm) {
	GlobalData * data = hmm->data;

	int size = data->test_size;
	int * words = data->test_words;

	if( can_speak(data) ) {
		printf(" - > Étiquetage...\n");
		fflush( stdout );
	}

	int approximated_labels[size];

	int k = 0;
	int space = get_next_space(words, k, size);

	while(k < size) {
		predict_viterbi(hmm, words + k, approximated_labels + k, space - k);
		k = space + 1;
		approximated_labels[space] = 0;
		space = get_next_space(words, k, size);
	}

	if( can_speak(data) ) {
		printf(" - > Terminé.\n");
		fflush( stdout );
	}

	double accuracy = compare_labels_results(data, approximated_labels);

	return accuracy;
}

int get_next_space(int * words, int i, int size) {
	for(int k = i; k < size; k++)
		if( ! words[k] ) return k;

	return size;
}
