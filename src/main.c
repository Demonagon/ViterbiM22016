#include "main.h"
#include <time.h>

int main(int argc, char ** argv) {
	GlobalData data;
	init_data(&data, argc, argv);

	clock_t time_1, time_2;

	ExecutionParameter execution = data.execution_parameters.execution;

	if( execution == COUNT || execution == EXECUTE_ALL ) {
		time_1 = clock();
		double viterbi_accuracy = viterbi(compute_corpus, &data);
		time_2 = clock();

		if( can_speak(&data) ) {
			printf("Précision de viterbi par fréquence relative = %lf\n",
					viterbi_accuracy);
			printf("Temps utilisé = %lfs\n",
					(time_2 - time_1)/(double) CLOCKS_PER_SEC);
		}
	}
	if( execution == PERCEPTRON || execution == EXECUTE_ALL ) {
		time_1 = clock();
		double perceptron_accuracy = viterbi(perceptron_compute_corpus, &data);
		time_2 = clock();

		if( can_speak(&data) ) {
			printf("Précision de viterbi par preceptron = %lf\n",
					perceptron_accuracy);
			printf("Temps utilisé = %lfs\n",
					(time_2 - time_1)/(double) CLOCKS_PER_SEC);
		}
	}

	free_data(&data);

	return 0;
}
