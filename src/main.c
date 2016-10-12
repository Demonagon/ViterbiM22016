#include "main.h"


int main(int argc, char ** argv) {
	read_parameters(argc, argv);

	ExecutionParameter execution = GLOBAL_PARAMETERS.execution;

	if( execution == COUNT || execution == EXECUTE_ALL ) {
		double viterbi_accuracy = viterbi(argv[1], argv[2], compute_corpus);

		if( can_speak() )
			printf("Précision de viterbi par fréquence relative = %lf\n",
					viterbi_accuracy);
	}
	if( execution == PERCEPTRON || execution == EXECUTE_ALL ) {
		double perceptron_accuracy = viterbi(argv[1], argv[2],
					perceptron_compute_corpus);

		if( can_speak() )
			printf("Précision de viterbi par preceptron = %lf\n",
					perceptron_accuracy);
	}

	return 0;
}
