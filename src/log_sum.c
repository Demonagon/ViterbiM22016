#include "log_sum.h"

double log_probability(double a) {
	return - log(a);
}

double get_real_probability(double log)
{
	return exp( - log );
}

double log_occurence_probability(double n, double total) {
	return log(total) - log(n);
}

double log_probability_mult(double a, double b) {
	return a + b;
}

double log_probability_mult3(double a, double b, double c) {
	return a + b + c;
}

void main_test_log_01() {
	double a = 1/1000.0;
	double b = 1/100.0;

	printf("a = %lf\n", a);
	printf("b = %lf\n", b);
	printf("a/b = %lf\n", a/b);
	printf("-log(a) = %lf\n", log_probability(a) );
	printf("-log(b) = %lf\n", log_probability(b) );

	double aoverb = log_occurence_probability(a, b);

	printf("-log(a/b) = %lf\n", aoverb);
	printf("a/b = %lf\n", get_real_probability(aoverb) );

	double atimesb = log_probability_mult(log_probability(a), log_probability(b));

	printf("a * b = %lf\n", a * b);
	printf("-log(a * b) = %lf\n", atimesb);
	printf("a * b = %lf\n", get_real_probability(atimesb));
}

