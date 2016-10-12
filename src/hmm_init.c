#include "hmm_init.h"

void init_corpus_analyser(CorpusAnalyser * ca, int states_nb,
						  int observables_nb)
{
	ca->states_nb = states_nb;
	ca->observables_nb = observables_nb;
	ca->size = 0;

	ca->first_occurences = malloc(states_nb * sizeof(int) );
	ca->sentences = 0;
	ca->transition_occurences = malloc(states_nb * sizeof(int*) );
	ca->emission_occurences = malloc(states_nb * sizeof(int*) );
	ca->category_occurences = malloc(states_nb * sizeof(int) );

	ca->transition_occurences[0] = malloc(states_nb *
										states_nb *
										sizeof(int) );
	ca->emission_occurences[0] = malloc(states_nb *
										observables_nb *
										sizeof(int) );

	for(int k = 0; k < states_nb; k++) {
		ca->first_occurences[k] = 0;
		ca->transition_occurences[k] = ca->transition_occurences[0] + k * states_nb;
		ca->emission_occurences[k] = ca->emission_occurences[0] + k * observables_nb;
		ca->category_occurences[k] = 0;
	}

	for(int i = 0; i < states_nb; i++)
		for(int j = 0; j < states_nb; j++)
			ca->transition_occurences[i][j] = 0;

	for(int i = 0; i < states_nb; i++)
		for(int j = 0; j < observables_nb; j++)
			ca->emission_occurences[i][j] = 0;
}

void free_corpus_analyser(CorpusAnalyser * ca) {

	free(ca->transition_occurences[0]);
	free(ca->emission_occurences[0]);

	free(ca->transition_occurences);
	free(ca->emission_occurences);

	free(ca->first_occurences);
	free(ca->category_occurences);
}

void analyse_sentence(CorpusAnalyser * ca, int * words, int * labels, int size)
{
	ca->size = size;

	ca->first_occurences[labels[0] - 1]++;
	ca->emission_occurences[labels[0] - 1][words[0] - 1]++;
	ca->category_occurences[labels[0] - 1]++;

	int previous_label = labels[0] - 1;

	for(int k = 1; k < size; k++) {
		int current_word = words[k] - 1;
		int current_label = labels[k] - 1;

		ca->emission_occurences[current_label][current_word]++;
		ca->transition_occurences[previous_label][current_label]++;
		ca->category_occurences[current_label]++;

		previous_label = current_label;
	}

	ca->sentences++;
}

void analyse_corpus(CorpusAnalyser * ca, char * corpus_file, double corpus_size)
{
	FILE * corpus = fopen(corpus_file, "r");

	int size = 0;

	if( can_speak() ) {
		printf(" - > Extraction des mots étiquetés...\n");
		fflush( stdout );
	}

	int * words;
	int * labels;

	int sentence_count = extract_labels_and_words(corpus,
												  &words, &labels, &size);

	fclose(corpus);

	if( can_speak() ) {
		printf(" - > Terminé.\n - > Comptage des occurences...\n");
		fflush( stdout );
	}

	if( can_shout() ) {
		printf("\t[");
		fflush( stdout );
	}

	int k = 0;
	int space = get_next_space(words, k, size);

	int sentences_to_read = (int) ( sentence_count * corpus_size );
	int sentences_read = 0;

	while(k + 1 < size && sentences_read < sentences_to_read) {
		if( can_shout() ) {
			printf(".");
			fflush( stdout );
		}

		analyse_sentence(ca, words + k, labels + k, space - k);

		k = space + 1;
		space = get_next_space(words, k, size);

		sentences_read++;
	}

	if( can_shout() ) {
		printf("]\n");
		fflush( stdout );
	}

	if( can_speak() ) {
		printf(" - > Terminé.\n");
		fflush( stdout );
	}

	free(words);
	free(labels);
}

void extract_hmm(CorpusAnalyser * ca, Hmm * hmm) {
	double smoothing_value = GLOBAL_PARAMETERS.smoothing_value;

	for(int k = 0; k < ca->states_nb; k++) {
		hmm->PI[k] = log_occurence_probability(
			ca->first_occurences[k] + smoothing_value,
			ca->sentences + ca->states_nb * smoothing_value
			);

		for(int i = 0; i < ca->states_nb; i++) {
			hmm->T[k][i] = log_occurence_probability(
				ca->transition_occurences[k][i] + smoothing_value,
				ca->category_occurences[k] + ca->states_nb * smoothing_value
				);
		}


		for(int i = 0; i < ca->observables_nb; i++)
			hmm->E[k][i] = log_occurence_probability(
				ca->emission_occurences[k][i] + smoothing_value,
				ca->category_occurences[k]
					+ ca->observables_nb * smoothing_value
				);
	}

}

Hmm * init_hmm_by_corpus() {
	int state_nb, word_nb;

	FILE * f = fopen("data/voc_etats.txt", "r");
	state_nb = get_line_count(f);
	fclose(f);

	f = fopen("data/voc_observables.txt", "r");
	word_nb = get_line_count(f);
	fclose(f);

	Hmm * hmm = allocate_hmm(state_nb, word_nb);

	initialize_hmm(hmm);

	return hmm;
}

void compute_corpus(Hmm * hmm, char * corpus_file_name, double corpus_size) {
	CorpusAnalyser ca;

	init_corpus_analyser(&ca, hmm->nbe, hmm->nbo);

	analyse_corpus(&ca, corpus_file_name, corpus_size);

	extract_hmm(&ca, hmm);

	free_corpus_analyser(&ca);
}
