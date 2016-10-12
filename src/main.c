#include "main.h"


int main(int argc, char ** argv) {
	GlobalData data;
	init_data(&data, argc, argv);

	ExecutionParameter execution = data.execution_parameters.execution;

	if( execution == COUNT || execution == EXECUTE_ALL ) {
		double viterbi_accuracy = viterbi(compute_corpus, &data);

		if( can_speak(&data) )
			printf("Précision de viterbi par fréquence relative = %lf\n",
					viterbi_accuracy);
	}
	if( execution == PERCEPTRON || execution == EXECUTE_ALL ) {
		double perceptron_accuracy = viterbi(perceptron_compute_corpus, &data);

		if( can_speak(&data) )
			printf("Précision de viterbi par preceptron = %lf\n",
					perceptron_accuracy);
	}

	free_data(&data);

	return 0;
}
