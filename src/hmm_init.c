#include "hmm_init.h"

void init_corpus_analyser(CorpusAnalyser * ca, int states_nb,
						  int observables_nb)
{
	ca->states_nb = states_nb;
	ca->observables_nb = observables_nb;

	ca->first_occurences = malloc(states_nb * sizeof(int) );
	ca->sentences = 0;
	ca->transition_occurences = malloc(states_nb * sizeof(int*) );
	ca->emission_occurences = malloc(states_nb * sizeof(int*) );
	ca->category_occurences = malloc(states_nb * sizeof(int) );

	ca->emission_occurences[0] = malloc(states_nb *
										observables_nb *
										sizeof(int) );

	for(int k = 0; k < states_nb; k++) {
		ca->first_occurences[k] = 0;
		ca->transition_occurences[k] = malloc(states_nb * sizeof(int));
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

	for(int i = 0; i < ca->states_nb; i++) {
		free(ca->transition_occurences[i]);
	}

	free(ca->transition_occurences);
	free(ca->emission_occurences);

	free(ca->first_occurences);
	free(ca->category_occurences);
}

void analyse_corpus(CorpusAnalyser * ca, FILE * corpus)
{
	int current_word, current_label, previous_label;

	while( ! feof(corpus) ) { // lecture du corpus
		current_word = get_next_labeled_word(corpus, &current_label);
		current_word--; current_label--;

		if( current_word < 0 || current_label < 0 )
			break;

		ca->first_occurences[current_label]++;
		ca->emission_occurences[current_label][current_word]++;
		ca->category_occurences[current_label]++;

		previous_label = current_label;

		while( ! feof(corpus) ) { // lecture d'une phrase
			current_word = get_next_labeled_word(corpus, &current_label);
			current_word--; current_label--;

			if( current_word < 0 || current_label < 0 )
				break;

			ca->emission_occurences[current_label][current_word]++;
			ca->transition_occurences[previous_label][current_label]++;
			ca->category_occurences[current_label]++;

			previous_label = current_label;
		}

		ca->sentences++;
	}
}

void extract_hmm(CorpusAnalyser * ca, Hmm * hmm) {
	for(int k = 0; k < ca->states_nb; k++) {
		hmm->PI[k] = log_occurence_probability(
			ca->first_occurences[k],
			ca->sentences
			);

		for(int i = 0; i < ca->states_nb; i++)
			hmm->T[k][i] = log_occurence_probability(
				ca->transition_occurences[k][i],
				ca->category_occurences[k]
				);

		for(int i = 0; i < ca->observables_nb; i++)
			hmm->E[k][i] = log_occurence_probability(
				ca->emission_occurences[k][i],
				ca->category_occurences[k]
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

void compute_corpus(Hmm * hmm, char * corpus_file_name) {
	FILE * corpus = fopen(corpus_file_name, "r");

	CorpusAnalyser ca;

	init_corpus_analyser(&ca, hmm->nbe, hmm->nbo);

	analyse_corpus(&ca, corpus);

	extract_hmm(&ca, hmm);

	free_corpus_analyser(&ca);

	fclose(corpus);
}
