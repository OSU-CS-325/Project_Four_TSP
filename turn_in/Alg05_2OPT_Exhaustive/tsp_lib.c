#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
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
			adj_matrix[i][j] = tsp_compute_euclidean_distance(x_array[i], y_array[i],
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

int tsp_compute_euclidean_distance(int x1, int y1, int x2, int y2) {
	double dx = x2 - x1;
	double dy = y2 - y1;
	double dist = sqrt(dx*dx + dy*dy);
	return (int) round(dist);
}

int tsp_create_in_order_tour(int **adj_matrix, int *tour, int num_pts) {

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

void tsp_write_tour_to_file(char *fname_out, int *tour, int num_pts, int tour_length) {
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
	int old_tour_length = *tour_length;

	while (improved) {
		improved = false;
		bool exit_early = false;

		for (int i = 1; i < num_pts - 1 && !exit_early; i++) {
			for (int j = i + 1; j < num_pts && !exit_early; j++) {
				old_tour_length = *tour_length;
				tsp_2opt_swap_efficient(adj_matrix, tour, tour_length, num_pts, i, j);
				if (*tour_length < old_tour_length) {
				 	// note that we improved, jump out early
				 	improved = true;
				 	exit_early = true;

				 	// write the improved tour to file
				 	// tsp_write_tour_to_file(fname_out, tour, num_pts, *tour_length);
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

// swap nodeA and nodeB in route order, if it will help the path
// returns updated tour length (or same tour length if no swap)
void tsp_2opt_swap_efficient(int **adj_matrix, int *tour, int *tour_length,
 int num_pts, int nodeA, int nodeB) {
	
	int min_node = MIN(nodeA, nodeB);
	int max_node = MAX(nodeA, nodeB);
	int new_tour_idx = 0;

	int removed_path_length = 0;
	int added_path_length = 0;

	int new_tour[num_pts];

	// The segments that would be removed -- careful to wrap endpoint if necessary
	if (max_node + 1 < num_pts) {
		removed_path_length = adj_matrix[tour[min_node-1]][tour[min_node]] + 
		adj_matrix[tour[max_node]][tour[max_node + 1]];

		added_path_length = adj_matrix[tour[min_node-1]][tour[max_node]] + 
		adj_matrix[tour[min_node]][tour[max_node + 1]];
	} else {
		removed_path_length = adj_matrix[tour[min_node-1]][tour[min_node]] + 
		adj_matrix[tour[max_node]][tour[0]];

		added_path_length = adj_matrix[tour[min_node-1]][tour[max_node]] + 
		adj_matrix[tour[min_node]][tour[0]];
	}

	int path_delta = removed_path_length - added_path_length;

	// Only re-compute the tour if it makes sense to (we found a shorter path)
	if (path_delta > 0) {
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

		// move updated tour into tour array
		memcpy(tour, new_tour, sizeof(int) * num_pts);

		*tour_length -= path_delta;
	}

	return;
}

int tsp_create_nearest_neighbor_tour(int **adj_matrix, int *tour, int num_pts) {
	
	// Determine how many attempts we will give it due to time constraints
	int num_iter = (num_pts <= 2001) ? num_pts : 10;

	// Keep track of best tour in int *tour, current tour in another array
	int cur_tour[num_pts];
	int cur_tour_length = 0;
	int best_tour_length = INT_MAX;
	int starting_city;

	// Create array to track whether a city is included yet or not (1/0)
	// and another for whether or not we've tried starting there before
	int city_in_tour[num_pts];
	int tried_as_starting_city[num_pts];

	int num_in_tour = 0;
	int prev_city = 0;

	int closest_distance = INT_MAX;
	int closest_city = num_pts; // will seg fault if I screw up

	for (int i = 0; i < num_pts; i++) {
		tried_as_starting_city[i] = 0;
	}

	for (int i = 0; i < num_iter; i++) {

		// Reset our city in tour array
		for (int j = 0; j < num_pts; j++) {
			city_in_tour[j] = 0;
		}

		// Reset distances and closest city
		closest_distance = INT_MAX;
		closest_city = num_pts; // will seg fault if I screw up

		// Pick a random city to start with
		starting_city = rand_interval(0, num_pts - 1);
		while (tried_as_starting_city[starting_city]) {
			starting_city = rand_interval(0, num_pts - 1);
		}

		// Mark that we've tried this as a starting city for future reference
		tried_as_starting_city[starting_city] = 1;
		// Mark that it's in the tour
		city_in_tour[starting_city] = 1;	

		cur_tour[0] = starting_city;
		cur_tour_length = 0;
		
		num_in_tour = 1;		

		while (num_in_tour < num_pts) {
			// Start at previous entry in tour, find nearest neighbor
			prev_city = cur_tour[num_in_tour - 1];

			for (int k = 0; k < num_pts; k++) {
				if (!city_in_tour[k] && adj_matrix[prev_city][k] < closest_distance) {
					closest_distance = adj_matrix[prev_city][k];
					closest_city = k;
				}
			}

			// Add closest city to tour
			cur_tour[num_in_tour] = closest_city;
			cur_tour_length += closest_distance;
			city_in_tour[closest_city] = 1;
			num_in_tour += 1;

			// Reset search values
			closest_distance = INT_MAX;
			closest_city = num_pts; // will seg fault if I screw up
		}

		// Add distance back to starting city
		cur_tour_length += adj_matrix[cur_tour[0]][cur_tour[num_pts-1]];

		// See if this is our best tour, if so, keep it
		if (cur_tour_length < best_tour_length) {
			best_tour_length = cur_tour_length;
			memcpy(tour, cur_tour, sizeof(int) * num_pts);
		}
	}	

	return best_tour_length;
}

int tsp_create_nearest_neighbor_tour_seeded(int **adj_matrix, int *tour, int num_pts,
 int starting_city) {
	
	// Start tour length at 0
	int tour_length = 0;

	// Create array to track whether a city is included yet or not (1/0)
	int city_in_tour[num_pts];

	int num_in_tour = 0;
	int prev_city = 0;

	int closest_distance = INT_MAX;
	int closest_city = num_pts;

	// Initialize our city in tour array
	for (int j = 0; j < num_pts; j++) {
		city_in_tour[j] = 0;
	}

	// Mark that starting city is in the tour and start there
	city_in_tour[starting_city] = 1;
	tour[0] = starting_city;	
	num_in_tour = 1;		

	while (num_in_tour < num_pts) {
		// Start at previous entry in tour, find nearest neighbor
		prev_city = tour[num_in_tour - 1];

		for (int k = 0; k < num_pts; k++) {
			if (!city_in_tour[k] && adj_matrix[prev_city][k] < closest_distance) {
				closest_distance = adj_matrix[prev_city][k];
				closest_city = k;
			}
		}

		// Add closest city to tour
		tour[num_in_tour] = closest_city;
		tour_length += closest_distance;
		city_in_tour[closest_city] = 1;
		num_in_tour += 1;

		// Reset search values
		closest_distance = INT_MAX;
		closest_city = num_pts; // will seg fault if I screw up
	}

	// Add distance back to starting city
	tour_length += adj_matrix[tour[0]][tour[num_pts-1]];

	return tour_length;
}

//from http://stackoverflow.com/questions/2509679/how-to-generate-a-random-number-from-within-a-range
unsigned int rand_interval(unsigned int min, unsigned int max) {
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}