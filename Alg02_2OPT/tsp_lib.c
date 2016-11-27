#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "tsp_lib.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int** tsp_input_to_adj_matrix(FILE *input_file, int *num_pts) {

	// Initial array size -- will double as needed
	int arr_size = 64;

	// x and x location arrays
	int *x_array;
	int *y_array;
	
	// malloc() memory for x and y location arrays based on initial arr_size
	x_array = (int *)malloc(sizeof(int) * arr_size);
	y_array = (int *)malloc(sizeof(int) * arr_size);
	// keep track of how many elements we've read in
	int idx = 0;

	// while we continue to successfully read 3 integers, continue and resize as needed
	while (fscanf(input_file, "%d %d %d", num_pts, &(x_array[idx]), &(y_array[idx])) == 3) {

		// keep track of how many elements we've read in
		idx++;

		// reallocate arrays to double-size if we exceed original size
		if (idx > arr_size - 1) {
			arr_size = arr_size * 2;
			x_array = (int *)realloc(x_array, sizeof(int) * arr_size);
			y_array = (int *)realloc(y_array, sizeof(int) * arr_size);
		}
	}

	// Valid indices = 0, 1, 2, ..., idx
	*num_pts = idx;

	// Create num_pts-by-num_pts 2D array for adjacency matrix
	// num_pts rows of pointers to int*
	int **adj_matrix = (int **)malloc(*num_pts * sizeof(int *));
	for (int i = 0; i < *num_pts; i++) {
		// num_pts columns of pointers to int
		adj_matrix[i] = (int *)malloc(*num_pts * sizeof(int));
	}

	// Fill out adjacency matrix
	// TODO: Optimize based on the fact that it's a symmetric matrix
	for (int i = 0; i < *num_pts; i++) {
		for (int j = 0; j < *num_pts; j++) {
			adj_matrix[i][j] = compute_euclidean_distance(x_array[i], y_array[i],
			 x_array[j], y_array[j]);
		}
	}

	// Clean up arrays we allocated
	free(x_array);
	free(y_array);

	return adj_matrix;
}

void tsp_cleanup(int **adj_matrix) {
	free(adj_matrix);
}

int compute_euclidean_distance(int x1, int y1, int x2, int y2) {
	double dx = x2 - x1;
	double dy = y2 - y1;
	double dist = sqrt(dx*dx + dy*dy);
	return (int) round(dist);
}

int create_in_order_tour(int **adj_matrix, int *tour, int num_pts) {

	// Start with first city, loop through in order
	tour[0] = 0;
	int num_in_tour = 1;

	while (num_in_tour < num_pts) {
		tour[num_in_tour] = num_in_tour;
		num_in_tour += 1;
	}

	int tour_length = tsp_compute_tour_distance(adj_matrix, tour, num_pts);

	return tour_length;
}

void write_tour_to_file(char *fname_out, int *tour, int num_pts, int tour_length) {
	// Open output file for writing
	FILE *output_file = fopen(fname_out, "w");
	if (!output_file) {
		fprintf(stderr, "\nERROR: Failed to open output file: '%s'\n.", fname_out);
		exit(-1);
	}

	fprintf(output_file, "%d\n", tour_length);
	for (int i = 0; i < num_pts; i++) {
		fprintf(output_file, "%d\n", tour[i]);
	}

	// If there is an error closing the output file, exit with error
	if (fclose(output_file)){ 
		fprintf(stderr, "\nERROR: Output file not closed successfully.\n");
		exit(-1);
	}
}

void tsp_2opt_search(int **adj_matrix, int *tour, int *tour_length, char *fname_out, int num_pts) {
	
	bool improved = true;
	int new_tour[num_pts];
	int new_tour_length = 0;

	while (improved) {
		improved = false;
		bool exit_early = false;

		for (int i = 1; i < num_pts - 1 && !exit_early; i++) {
			for (int j = i + 1; j < num_pts && !exit_early; j++) {
				 tsp_2opt_swap(new_tour, tour, num_pts, i, j);
				 new_tour_length = tsp_compute_tour_distance(adj_matrix, new_tour, num_pts);
				 if (new_tour_length < *tour_length) {
				 	// make tour the improved tour
				 	*tour_length = new_tour_length;
				 	memcpy(tour, new_tour, sizeof(int) * num_pts);

				 	// note that we improved, jump out early
				 	improved = true;
				 	exit_early = true;

				 	// write the improved tour to file
				 	write_tour_to_file(fname_out, tour, num_pts, *tour_length);
				 }
			}
		}
	}
}


//compute tour distance
int tsp_compute_tour_distance(int **adj_matrix, int *tour, int num_pts) {
	int tour_length = 0;

	// Add distance from tour[i] to tour[i+1] to tour length
	for (int i = 0; i < num_pts - 1; i++) {
		tour_length += adj_matrix[tour[i]][tour[i+1]];
	}

	// Add final distance from tour[end] to tour[0]
	tour_length += adj_matrix[tour[num_pts - 1]][tour[0]];

	return tour_length;
}

//swap nodeA and nodeB in route order
void tsp_2opt_swap(int *new_tour, int *tour, int num_pts, int nodeA, int nodeB) {
	
	int min_node = MIN(nodeA, nodeB);
	int max_node = MAX(nodeA, nodeB);
	int new_tour_idx = 0;

	// route from 1 to min_node - 1, in order
	for (int i = 0; i < min_node; i++) {
		new_tour[new_tour_idx] = tour[i];
		new_tour_idx++;
	}

	// route from min_node to max_node, in reverse order
	for (int i = max_node; i >= min_node; i--) {
		new_tour[new_tour_idx] = tour[i];
		new_tour_idx++;
	}

	// route from max_node + 1 to end, in order
	for (int i = max_node + 1; i < num_pts; i++) {
		new_tour[new_tour_idx] = tour[i];
		new_tour_idx++;
	}
}