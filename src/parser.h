#define _GNU_SOURCE
#define PARSER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct facets {
  int *vertexes;
  int numbers_of_vertexes_in_facets;
} polygon_t;

typedef struct Matrix {
  double **matrix;
  int rows;
  int cols;
} matrix_t;

typedef struct data {
  int count_of_vertexes;
  int count_of_facets;
  matrix_t matrix_3d;
  polygon_t *polygons;
} data_t;

int init_data(data_t *data);
int parsline(char *filename, data_t *data);
data_t *create_data();
int init_polygon(data_t *data, char *line, int index);
void destroy_data(data_t **data);
