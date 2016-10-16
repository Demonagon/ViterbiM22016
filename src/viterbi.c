#include "viterbi.h"

/**
* Cette fonction exécute l'algorithme de viterbi, sachant une méthode de
* génération de score de hmm donnée et les informations générales données.
*/
double viterbi(HmmGenerator generator, GlobalData * data) {
	if( can_speak(data) )
		printf("Entraînement...\n");

	fflush(stdout);

	// On initialise le hmm
	Hmm * hmm = init_hmm_by_corpus();
	hmm->data = data;

	// On génère les scores du hmm
	generator(hmm);

	if( can_speak(data) ) {
		printf("Terminé.\n");
		printf("Test...\n");
	}

	fflush(stdout);

	// On calcule une précision de ces paramètres sur le corpus de test
	double accuracy = predict_and_compare_viterbi(hmm);

	if( can_speak(data) )
		printf("Terminé.\n");

	// On libère le hmm
	free_hmm(hmm);

	// On retourne la précision précédemment calculée
	return accuracy;
}

/**
* Cette fonction correspond à la première partie de l'algorithme de viterbi,
* et initialise les tableaux score et backtrack aux valeurs adéquates.
*/
void viterbi_initialization(Hmm * hmm, int * words, int size, double ** score,
							int ** backtrack) {

	int first_word = words[0] - 1;

	for(int k = 0; k < hmm->nbe; k++) {
		// Le score est initalisé à la valeur pi_k * b_k(O_1)
		score[k][0] = log_probability_mult(hmm->PI[ k ],
										   hmm->E[k][ first_word ]);
		backtrack[k][0] = 0;
	}
}

/**
* Cette fonction renvoie le score minimal pour i la position considérée.
*/
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

/**
* Cette fonction renvoie l'indice du score minimal contenu dans score pour i
* la position considérée.
*/
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

/**
* Cette fonction correspond au morceau principal de l'algorithme de viterbi.
* Ici, le calcul récursif des tableaux score et backtrack est mis en place.
*/
void viterbi_recursion(Hmm * hmm, int * words, int size, double ** score,
					   int ** backtrack) {

	for(int i = 1; i < size; i++) {
		for(int state = 0; state < hmm->nbe; state++) {
			score[state][i] = viterbi_min_k(hmm, i, state, words, score);
			backtrack[state][i] = viterbi_arg_min_k(hmm, i, state, score);
		}
	}
}

/**
* Cette fonction correspond au dernier morceau de l'algorithme de Viterbi,
* c'est à dire la récupération de l'étiquetage optimal contenu dans les tableaux
* score et backtrack.
*/
void viterbi_output(Hmm * hmm, int * labels, int size, double ** score,
					int ** backtrack) {

	int last_label = 0;
	double last_label_score = INFINITY;

	// On récupère la meilleure étiquette pour le dernier mot de la phrase
	for(int k = 0; k < hmm->nbe; k++)
		if( score[k][size - 1] < last_label_score ) {
			last_label = k;
			last_label_score = score[k][size - 1];
		}

	labels[size - 1] = last_label + 1;

	int next_label = last_label;

	// On récupère toutes les étiquettes en partant de la fin de la phrase
	// jusqu'au premier mot
	for(int k = size - 1; k > 0; k--) {
		labels[k - 1] = backtrack[next_label][k] + 1;
		next_label = backtrack[next_label][k];
	}
}

/**
* Cette fonction fait une prédiction de score (aka un étiquetage) sur les mots
* donnés en paramètre sachant le hmm donné en paramètre.
* Les étiquettes ainsi obtenues sont mises dans le paramètre labels.
*/
void predict_viterbi(Hmm * hmm, int * words, int * labels, int size)
{
	// Création des structures de données score et backtrack, nécessaires*
	// à l'algorithme de Viterbi
	double ** score = malloc( sizeof(double * ) * hmm->nbe);
	int ** backtrack = malloc( sizeof(int * ) * hmm->nbe);

	for(int k = 0; k < hmm->nbe; k++) {
		score[k] = malloc(sizeof(double) * size);
		backtrack[k] = malloc(sizeof(int) * size);
	}

	// Initialisation des tableaux
	viterbi_initialization(hmm, words, size, score, backtrack);

	// Calcul des tableaux
	viterbi_recursion(hmm, words, size, score, backtrack);

	// Récupération de l'étiquettage
	viterbi_output(hmm, labels, size, score, backtrack);

	// Libération de l'espace occupé par les tableaux
	for(int k = 0; k < hmm->nbe; k++) {
		free(score[k]);
		free(backtrack[k]);
	}

	free( score );
	free( backtrack );
}

/**
* Cette fonction utilise le hmm donné en paramètre pour calculer un étiquettage
* optimal sur la donnée de test, puis renvoie le score de précision de cet
* étiquettage.
*/
double predict_and_compare_viterbi(Hmm * hmm) {
	GlobalData * data = hmm->data;

	// Récupération du corpus de test sans étiquettes
	int size = data->test_size;
	int * words = data->test_words;

	if( can_speak(data) ) {
		printf(" - > Étiquetage...\n");
		fflush( stdout );
	}

	int approximated_labels[size];

	// Isolation de la première phrase commençant à 0 et terminant au premier
	// espace rencontré
	int k = 0;
	int space = get_next_space(words, k, size);

	// Tant que la phrase considérée possède au moins un mot
	while(k < size) {
		// On calcule les étiquettes de ces phrases que l'on met dans l'espace
		// correspondant dans le tableau des étiquettes
		predict_viterbi(hmm, words + k, approximated_labels + k, space - k);

		// On isole la phrase suivante, commencant juste après la précédente
		// et terminant au espace d'après
		k = space + 1;
		// au passage, on met à 0 l'étiquette de l'espace entre les phrases
		approximated_labels[space] = 0;
		space = get_next_space(words, k, size);
	}

	if( can_speak(data) ) {
		printf(" - > Terminé.\n");
		fflush( stdout );
	}

	// On calcule le score
	double accuracy = compare_labels_results(data, approximated_labels);

	return accuracy;
}

/**
* Cette fonction très utile permet de calculer le prochain espace que l'on
* rencontre après l'indice i dans le tableau de mots words.
*/
int get_next_space(int * words, int i, int size) {
	for(int k = i; k < size; k++)
		if( ! words[k] ) return k;

	return size;
}
