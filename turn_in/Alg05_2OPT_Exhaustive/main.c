#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "tsp_lib.h"

int main(int argc, char *argv[]){

	// Seed random number generator
	srand ( time(NULL) );
	
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

	// If there is an error closing the input file, exit with error
	if (fclose(input_file)){ 
		fprintf(stderr, "\nERROR: Input file not closed successfully.\n");
		exit(-1);
	}

	int tour_length = 0;
	int best_tour_length = INT_MAX;
	int tour_order[num_pts];
	int best_tour[num_pts];

	// We'll search using 2-OPT starting with every possible nearest-neighbor tour.
	// Nearest-neighbor tours tend to be fairly short on average, so odds are good
	// that one is near a good overall local minimum. We'll start with the nearest
	// neighbor tour that begins at vertex 0, and enumerate all possibilities. The
	// shortest overall will be kept.
	for (int i = 0; i < num_pts; i++) {
		tour_length = tsp_create_nearest_neighbor_tour_seeded(adj_matrix, tour_order,
		 num_pts, i);

		// perform 2-OPT on our nearest-neighbor tour
		tsp_2opt_search(adj_matrix, tour_order, &tour_length, fname_out, num_pts);

		// if it's the best we've seen, save it and write it out
		if (tour_length < best_tour_length) {
			best_tour_length = tour_length;
			memcpy(best_tour, tour_order, sizeof(int) * num_pts);
			tsp_write_tour_to_file(fname_out, best_tour, num_pts, best_tour_length);
		}
	}

	// Start with a simple in order tour
	// tour_length = tsp_create_in_order_tour(adj_matrix, tour_order, num_pts);
	// tour_length = tsp_create_nearest_neighbor_tour(adj_matrix, tour_order, num_pts);

	// char dummyName[] = "dumb_tour.txt.tour";

	// Write simple tour to file
	// tsp_write_tour_to_file(dummyName, tour_order, num_pts, tour_length);

	// tsp_2opt_search(adj_matrix, tour_order, &tour_length, fname_out, num_pts);

	// Write final optimal tour to file
	tsp_write_tour_to_file(fname_out, best_tour, num_pts, best_tour_length);

	// Clean up
	tsp_cleanup(adj_matrix);

	// Successful exit
	exit(0);
}