#include "hmm_init.h"

/**
* Cette fonction initialise la structure CorpusAnalyser en fonction des
* paramètres. Toutes les comptes sont mis à 0.
*/
void init_corpus_analyser(CorpusAnalyser * ca, int states_nb,
						  int observables_nb, GlobalData * data)
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

	ca->data = data;
}

/**
* Libère la mémoire utilisée par la structure CorpusAnalyser passée en argument.
*/
void free_corpus_analyser(CorpusAnalyser * ca) {

	free(ca->transition_occurences[0]);
	free(ca->emission_occurences[0]);

	free(ca->transition_occurences);
	free(ca->emission_occurences);

	free(ca->first_occurences);
	free(ca->category_occurences);
}

/**
* Cette fonction compte chaque évènement présent dans la phrase passée en
* paramètre, et ajoute chaque occurence dans la structure CorpusAnalyser passée
* en argument.
*/
void analyse_sentence(CorpusAnalyser * ca, int * words, int * labels, int size)
{
	ca->size = size;

	// On compte tout les évènements en relation avec le premier mot.
	ca->first_occurences[labels[0] - 1]++;
	ca->emission_occurences[labels[0] - 1][words[0] - 1]++;
	ca->category_occurences[labels[0] - 1]++;

	// On prend le premier mot comme "mot précédent"
	int previous_label = labels[0] - 1;

	for(int k = 1; k < size; k++) {
		int current_word = words[k] - 1;
		int current_label = labels[k] - 1;

		// On compte tout les évènements en relations avec le mot actuel et
		// le mot précédent
		ca->emission_occurences[current_label][current_word]++;
		ca->transition_occurences[previous_label][current_label]++;
		ca->category_occurences[current_label]++;

		// Le mot actuel devient le mot précédent
		previous_label = current_label;
	}

	// On augmente de 1 le nombre de phrases lues
	ca->sentences++;
}

/**
* Cette fonction compte l'ensemble des évènements du corpus d'apprentissage
* et enregistre ces statistiques dans la structure CorpusAnalyser passée en
* argument.
*/
void analyse_corpus(CorpusAnalyser * ca)
{
	GlobalData * data = ca->data;

	// Récupération du corpus d'apprentissage
	int size = data->learning_size;
	int * words = data->learning_words;
	int * labels = data->learning_labels;

	if( can_speak(data) ) {
		printf(" - > Comptage des occurences...\n");
		fflush( stdout );
	}

	if( can_shout(data) ) {
		printf("\t[");
		fflush( stdout );
	}

	// On délimite la première phrase, commencant à 0 et finissant au premier
	// espace rencontré
	int k = 0;
	int space = get_next_space(words, k, size);

	// Tant que la phrase considérée contient au moins un mot
	while(k + 1 < size) {
		if( can_shout(data) ) {
			printf(".");
			fflush( stdout );
		}

		// On analyse la phrase actuelle
		analyse_sentence(ca, words + k, labels + k, space - k);

		// On situe la prochaine phrase après la phrase actuelle, et se
		// finissant au prochain espace rencontré
		k = space + 1;
		space = get_next_space(words, k, size);
	}

	if( can_shout(data) ) {
		printf("]\n");
		fflush( stdout );
	}

	if( can_speak(data) ) {
		printf(" - > Terminé.\n");
		fflush( stdout );
	}
}

/**
* Cette fonction utilise les statistiques dans la structure CorpusAnalyser
* passée en argument pour déterminer les scores du hmm passé en paramètre
*/
void extract_hmm(CorpusAnalyser * ca, Hmm * hmm) {
	// Récupération du paramètre de lissage
	double smoothing_value = hmm->data->execution_parameters.smoothing_value;

	// Pour chaque état k,
	for(int k = 0; k < ca->states_nb; k++) {
		// On calcule le score pi_k
		hmm->PI[k] = log_occurence_probability(
			ca->first_occurences[k] + smoothing_value,
			ca->sentences + ca->states_nb * smoothing_value
			);

		// Pour chaque état i,
		for(int i = 0; i < ca->states_nb; i++) {
			// On calcule le score a_ki
			hmm->T[k][i] = log_occurence_probability(
				ca->transition_occurences[k][i] + smoothing_value,
				ca->category_occurences[k] + ca->states_nb * smoothing_value
				);
		}

		// Pour chaque observable i,
		for(int i = 0; i < ca->observables_nb; i++)
			// On calcule le score b_k(i)
			hmm->E[k][i] = log_occurence_probability(
				ca->emission_occurences[k][i] + smoothing_value,
				ca->category_occurences[k]
					+ ca->observables_nb * smoothing_value
				);
	}

}

/**
* Cette fonction initialise un hmm vide de la taille correspondant à nos
* vocabulaires voc_etats.txt et voc_observables.txt.
*/
Hmm * init_hmm_by_corpus(GlobalData * data) {
	int state_nb, word_nb;

	FILE * f = fopen("data/voc_etats.txt", "r");
	state_nb = get_line_count(f);
	fclose(f);

	f = fopen("data/voc_observables.txt", "r");
	word_nb = get_line_count(f);
	fclose(f);

	Hmm * hmm = allocate_hmm(state_nb, word_nb);

	initialize_hmm(hmm, data);

	return hmm;
}

/**
* Cette fonction crée une structure CorpusAnalyser pour compter les évènements
* sur le corpus d'apprentissage pour initialiser les scores du hmm passé
* en argument.
*/
void compute_corpus(Hmm * hmm) {
	CorpusAnalyser ca;

	init_corpus_analyser(&ca, hmm->nbe, hmm->nbo, hmm->data);

	analyse_corpus(&ca);

	extract_hmm(&ca, hmm);

	free_corpus_analyser(&ca);
}
