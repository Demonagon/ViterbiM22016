#include "main.h"


int main(int argc, char ** argv) {

	if( argc < 3 ) {
		print_parameters_note(stdout, argv);
		return 0;
	}

	read_parameters(argc - 3, argv + 3);

	/*double viterbi_accuracy = viterbi(argv[1], argv[2], compute_corpus);

	if( GLOBAL_PARAMETERS.noise >= LOUD )
		printf("Précision de viterbi par fréquence relative = %lf\n",
				viterbi_accuracy);*/

	double perceptron_accuracy = viterbi(argv[1], argv[2],
				perceptron_compute_corpus);

	if( GLOBAL_PARAMETERS.noise >= LOUD )
		printf("Précision de viterbi par preceptron = %lf\n",
				perceptron_accuracy);

	return 0;
}
