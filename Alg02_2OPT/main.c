#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tsp_lib.h"

int main(int argc, char *argv[]){
	
	// Verify only one input argument was provided, otherwise exit with error
	if (argc != 2) {
		fprintf(stderr, "\nERROR: Expected 1 input parameter, got %d.\n", argc - 1);
		exit(-1);
	}

	// Determine output file name
	char fname_out[100];
	strcpy(fname_out, argv[1]);
	strcat(fname_out, ".tour");

	// Open input file for reading
	FILE *input_file = fopen(argv[1], "r");
	if (!input_file) {
		fprintf(stderr, "\nERROR: Failed to open input file: '%s'\n.", argv[1]);
		exit(-1);
	}

	int **adj_matrix = 0;
	int num_pts = 0;

	adj_matrix = tsp_input_to_adj_matrix(input_file, &num_pts);

	// for (int i = 0; i < num_pts; i++) {
	// 	for (int j = 0; j < num_pts; j++) {
	// 		printf("%d ", adj_matrix[i][j]);
	// 	}
	// 	printf("\n");
	// }

	// If there is an error closing the input file, exit with error
	if (fclose(input_file)){ 
		fprintf(stderr, "\nERROR: Input file not closed successfully.\n");
		exit(-1);
	}

	int tour_length = 0;
	int tour_order[num_pts];

	// Start with a simple in order tour
	tour_length = create_in_order_tour(adj_matrix, tour_order, num_pts);

	// Write simple tour to file
	write_tour_to_file(fname_out, tour_order, num_pts, tour_length);

	// Track the best tour we've found so far
	int best_tour[num_pts];
	memcpy(best_tour, tour_order, num_pts * sizeof(int));
	int best_tour_length = tour_length;

	tsp_2opt_search(adj_matrix, tour_order, tour_length, best_tour, &best_tour_length, 
		fname_out, num_pts);

	// Clean up
	tsp_cleanup(adj_matrix);

	// Successful exit
	exit(0);
}