#include "main.h"


int main(int argc, char ** argv) {

	printf("Analyse du corpus");

	Hmm * hmm = init_hmm_by_corpus();

	printf(".");

	compute_corpus(hmm, "data/ftb.dev.encoded");

	printf(".");

	print_hmm(hmm, "results/test.hmm");

	printf(".");

	free_hmm(hmm);

	printf(" Terminé.\n");

	return 0;
}
