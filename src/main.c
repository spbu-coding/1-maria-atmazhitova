#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const int INTERVAL_INPUT_FAILED = -1, INTERVAL_INPUT_SUCCEEDED = 0, RESULTS_OUTPUT_FAILED = 1, RESULTS_OUTPUT_SUCCEEDED = 0, MAX_STRING_LENGTH = 80;

double calculate_by_rectangle_rule(double left_border, double right_border) {
	return sin((left_border + right_border) / 2) * (right_border - left_border);
}

double calculate_by_composite_simpson_rule(double left_border, double right_border) {
	return (sin(left_border) + 4 * sin((left_border + right_border) / 2) + sin(right_border)) * (right_border - left_border) / 6;
}

double calculate_area(double left_border, double right_border, int number_of_rectangles, double (*method)(double, double)) {
	double rectangle_width = (right_border - left_border) / number_of_rectangles;
	double current_left_border = left_border;
	double area = 0;
	for (int i = 0; i < number_of_rectangles; i++) {
		double current_right_border = current_left_border + rectangle_width;
		area += method(current_left_border, current_right_border);
		current_left_border = current_right_border;
	}
	return area;
}

int read_interval(double *left_border, double *right_border) {
	if (printf("Enter interval's left border: ") < 0) {
		fprintf(stderr, "Cannot write to stdout\n");
		return INTERVAL_INPUT_FAILED;
	}
	if (scanf("%lf", left_border) != 1) {
		fprintf(stderr, "Cannot read interval's left border\n");
		return INTERVAL_INPUT_FAILED;
	}
	if (*left_border < 0) {
		fprintf(stderr, "The interval's left border must not be less than 0\n");
		return INTERVAL_INPUT_FAILED;
	}
	if (printf("Enter interval's right border: ") < 0) {
		fprintf(stderr, "Cannot write to stdout\n");
		return INTERVAL_INPUT_FAILED;
	}
	if (scanf("%lf", right_border) != 1) {
		fprintf(stderr, "Cannot read interval's right border\n");
		return INTERVAL_INPUT_FAILED;
	}
	if (*right_border > M_PI) {
		fprintf(stderr, "The interval's right border must not be greater than pi\n");
		return INTERVAL_INPUT_FAILED;
	}
	if (*right_border < *left_border) {
		fprintf(stderr, "The interval's right border must not be less than left\n");
		return INTERVAL_INPUT_FAILED;
	}
	return INTERVAL_INPUT_SUCCEEDED;
}

void free_array_of_strings(char **array, unsigned int elements_to_clear) {
	int true_array_length = sizeof(array) / sizeof(char*);
	if (elements_to_clear > true_array_length)
		elements_to_clear = true_array_length;
	for (unsigned int i = 0; i < elements_to_clear; i++) {
		free(array[i]);
	}
	free(array);
}

char **calculate_areas(double left_border, double right_border, const unsigned int *numbers_of_rectangles, const int number_of_experiments) {
	char **results_of_experiments = (char **)malloc(sizeof(char *) * number_of_experiments);
	if (results_of_experiments == NULL) {
		fprintf(stderr, "Cannot allocate memory for result strings\n");
		return NULL;
	}

	for (int i = 0; i < number_of_experiments; i++) {
		results_of_experiments[i] = (char *)malloc(sizeof(char) * MAX_STRING_LENGTH);
		if (results_of_experiments[i] == NULL) {
			fprintf(stderr, "Cannot allocate memory for result string in %d experiment\n", i + 1);
			return NULL;
		}

		double area_rectangle = calculate_area(left_border, right_border, numbers_of_rectangles[i], calculate_by_rectangle_rule);
		double area_simpson = calculate_area(left_border, right_border, numbers_of_rectangles[i], calculate_by_composite_simpson_rule);
		if (sprintf(results_of_experiments[i], "%d %.5f %.5f", numbers_of_rectangles[i], area_rectangle, area_simpson) <= 0) {
			fprintf(stderr, "Cannot write result of %d experiment to a string\n", i + 1);
			free_array_of_strings(results_of_experiments, i);
			return NULL;
		}
	}

	return results_of_experiments;
}

int print_results(char **results_of_experiments, unsigned int number_of_experiments) {
	for (unsigned int i = 0; i < number_of_experiments; i++) {
		if (printf("%s\n", results_of_experiments[i]) <= 0) {
			fprintf(stderr, "Cannot write %d result to stdout\n", i + 1);
			return RESULTS_OUTPUT_FAILED;
		}
	}
	return RESULTS_OUTPUT_SUCCEEDED;
}

int main() {
	double left_border, right_border;
	if (read_interval(&left_border, &right_border) == INTERVAL_INPUT_FAILED) {
		return 1;
	}
	
	char **results_of_experiments;
	const unsigned int number_of_experiments = 6, numbers_of_rectangles[] = {6, 10, 20, 100, 500, 1000};
	results_of_experiments = calculate_areas(left_border, right_border, numbers_of_rectangles, number_of_experiments);
	if (results_of_experiments == NULL) {
		return 1;
	}

	if (print_results(results_of_experiments, number_of_experiments) == RESULTS_OUTPUT_FAILED) {
		return 1;
	}
	free_array_of_strings(results_of_experiments, number_of_experiments);
	return 0;
}