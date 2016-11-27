#ifndef TSP_LIB_H_INCLUDED
#define TSP_LIB_H_INCLUDED

int** tsp_input_to_adj_matrix(FILE *input_file, int *num_pts);

void tsp_cleanup(int **adj_matrix);

int compute_euclidean_distance(int x1, int y1, int x2, int y2);

int create_in_order_tour(int **adj_matrix, int *tour, int num_pts);

void write_tour_to_file(char *fname_out, int *tour, int num_pts, int tour_length);

void tsp_2opt_search(int **adj_matrix, int *tour, int *tour_length, char *fname_out, int num_pts);

int tsp_compute_tour_distance(int **adj_matrix, int *tour, int num_pts);

void tsp_2opt_swap(int *new_tour, int *tour, int num_pts, int nodeA, int nodeB);

void tsp_2opt_swap_efficient(int **adj_matrix, int *tour, int *tour_length,
 int num_pts, int nodeA, int nodeB);

#endif