#include <math.h>
#ifndef PARSER
#define PARSER
#include "./parser.h"
#endif
void move_x(matrix_t *A, double a);
void move_y(matrix_t *A, double a);
void move_z(matrix_t *A, double a);
void rotation_by_ox(matrix_t *A, double angle);
void rotation_by_oy(matrix_t *A, double angle);
void rotation_by_oz(matrix_t *A, double angle);
void scale_model(matrix_t *A, double alp);

void found_min_max_or(matrix_t *A, int **ptr_x, int **ptr_y, int **ptr_z);
void first_centers(matrix_t *A);
void first_init_val_gl(matrix_t *A);
void first_init_val(matrix_t *A);
