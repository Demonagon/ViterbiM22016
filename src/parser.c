#include "parser.h"

void get_line(FILE * input, char * line)
{
	char c = 0;
	line[0] = 0;

	while( c != '\n' && ! feof(input) ) {
		fscanf(input, "%c", &c);
		sprintf(line, "%s%c", line, c);
	}
}

int get_next_word(FILE * input) {
	static char line[LINE_LENGTH];
	int word;

	get_line(input, line);
	
	if( sscanf(line, "%d", &word) != 1 )
		return 0;

	return word;
}

int get_next_label(FILE * input) {
	static char line[LINE_LENGTH];
	int label;

	get_line(input, line);
	
	if( sscanf(line, "%*d %d", &label) != 1 )
		return 0;

	return label;
}

int get_next_labeled_word(FILE * input, int * label) {
	static char line[LINE_LENGTH];
	int word;

	get_line(input, line);
	
	if( sscanf(line, "%d %d", &word, label) != 2 ) {
		*label = 0;
		return 0;
	}


	return word;
}

int get_line_count(FILE * input) {
	static char line[LINE_LENGTH];
	int line_count = 0;
	while( ! feof(input) ) {
		get_line(input, line);

		if( line[0] ) line_count++;
	}

	return line_count;
}

int get_text_line_count(FILE * input) {
	static char line[LINE_LENGTH];
	int line_count = 0;
	while( ! feof(input) ) {
		fscanf(input, "%[^\n]%*[\n]", line);

		if( line[0] ) line_count++;
	}

	return line_count;
}

int get_sentences_count(FILE * input) {
	static char line[LINE_LENGTH];
	int sentences_count = 0;
	char last_sentence_was_empty = 1;
	while( ! feof(input) ) {
		get_line(input, line);

		if( ! last_sentence_was_empty && ! line[0] ) sentences_count++;

		last_sentence_was_empty = ! line[0];
	}

	return sentences_count;
}

void main_test_parsing(char * file_name) {
	FILE * f = fopen(file_name, "r");
	int word = 0;
	int label = 0;
	int index = 1;

	while( ! feof(f) ) {
		word = get_next_labeled_word(f, &label);
		printf("%5d : %d - %d\n", index, word, label);
		index++;
	}
}

int * extract_words(FILE * corpus, int * size) {
	*size = get_line_count(corpus);

	rewind(corpus);

	int * words = malloc( sizeof(int) * *size );

	int k = 0;

	while( ! feof(corpus) ) {
		words[k] = get_next_word(corpus);
		k++;
	}

	return words;
}

int * extract_labels(FILE * corpus, int * size) {
	*size = get_line_count(corpus);

	rewind(corpus);

	int * labels = malloc( sizeof(int) * *size );

	int k = 0;

	while( ! feof(corpus) ) {
		labels[k] = get_next_label(corpus);
		k++;
	}

	return labels;
}

int extract_labels_and_words(GlobalData * data, FILE * corpus, int ** words,
							int ** labels, int * size, double corpus_size) {
	static char line[LINE_LENGTH];
	int sentences_count = 0;
	char last_sentence_was_empty = 1;

	* size = get_line_count(corpus);

	rewind(corpus);

	* words = malloc( sizeof(int) * *size );
	* labels = malloc( sizeof(int) * *size );

	int k = 0;

	int progress_state = 0;
	int progress_length = 10;

	if( can_speak(data) ) {
		printf("\t");
		fflush( stdout );
	}

	while( ! feof(corpus) ) {

		while( k / (double) *size > progress_state / (double) progress_length
			   && can_speak(data) ) {
			printf("%d%%...   ", progress_state * 10);
			progress_state++;
			fflush( stdout );
		}
	
		get_line(corpus, line);

		if( sscanf(line, "%d %d", (*words) + k, (*labels) + k) != 2 ) {
			(*labels)[k] = 0;
			(*words)[k] = 0;
			if( ! last_sentence_was_empty ) {
				sentences_count++;
				if( k >= ( *size * corpus_size ) ) {
					if( feof(corpus) ) break;
					*size = k + 2;
					(*labels)[k + 1] = 0;
					(*words)[k + 1] = 0;
					break;
				}
			}
			last_sentence_was_empty = 1;
		}
		else
			last_sentence_was_empty = 0;

		k++;
	}

	if( can_speak(data) ) {
		printf("100%%\n");
		fflush( stdout );
	}

	return sentences_count;
}

void main_test_line_count() {
	FILE * f = fopen("data/voc_etats.txt", "r");

	printf("nb états : %d\n", get_line_count(f) );

	f = fopen("data/voc_observables.txt", "r");

	printf("nb de mots : %d\n", get_line_count(f) );
}
