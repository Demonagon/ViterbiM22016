#include "log_sum.h"

double log_occurence_probability(int n, int total) {
	return log(total) - log(n);
}

double log_probability_mult(double a, double b) {
	return a + b;
}

double log_probability_mult3(double a, double b, double c) {
	return a + b + c;
}

void main_test_log_01() {
	int total = 10000;
	int a = 2;
	int b = 20;
	int c = 200;
	int d = 2000;
	int e = total - (a + b + c + d);

	printf("Probabilité brute :\n");
	printf("p(a) = %lf\n", a / (double) total);
	printf("p(b) = %lf\n", b / (double) total);
	printf("p(c) = %lf\n", c / (double) total);
	printf("p(d) = %lf\n", d / (double) total);
	printf("p(e) = %lf\n", e / (double) total);
	printf("p(all) = %lf\n", total / (double) total);
	printf("Probabilité logarithmique :\n");
	printf("p(a) = %lf\n", log_occurence_probability(a, total));
	printf("p(b) = %lf\n", log_occurence_probability(b, total));
	printf("p(c) = %lf\n", log_occurence_probability(c, total));
	printf("p(d) = %lf\n", log_occurence_probability(d, total));
	printf("p(e) = %lf\n", log_occurence_probability(e, total));
	printf("p(total) = %lf\n", log_occurence_probability(total, total));
}

