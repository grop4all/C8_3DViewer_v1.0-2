#include "transformations.h"

void found_min_max_or(matrix_t *A, int **ptr_x, int **ptr_y, int **ptr_z) {
  int *min_max_x = malloc(sizeof(int) * 2);
  int *min_max_y = malloc(sizeof(int) * 2);
  int *min_max_z = malloc(sizeof(int) * 2);

  min_max_x[0] = 1;
  min_max_x[1] = 1;
  min_max_y[0] = 1;
  min_max_y[1] = 1;
  min_max_z[0] = 1;
  min_max_z[1] = 1;

  for (int i = 1; i < A->rows; ++i) {
    if (A->matrix[min_max_x[0]][0] > A->matrix[i][0]) min_max_x[0] = i;
    if (A->matrix[min_max_x[1]][0] < A->matrix[i][0]) min_max_x[1] = i;

    if (A->matrix[min_max_y[0]][1] > A->matrix[i][1]) min_max_y[0] = i;
    if (A->matrix[min_max_y[1]][1] < A->matrix[i][1]) min_max_y[1] = i;

    if (A->matrix[min_max_z[0]][2] > A->matrix[i][2]) min_max_z[0] = i;
    if (A->matrix[min_max_z[1]][2] < A->matrix[i][2]) min_max_z[1] = i;
  }
  *ptr_x = min_max_x;
  *ptr_y = min_max_y;
  *ptr_z = min_max_z;
}

void first_centers(matrix_t *A) {
  int *min_max_x = NULL;
  int *min_max_y = NULL;
  int *min_max_z = NULL;

  found_min_max_or(A, &min_max_x, &min_max_y, &min_max_z);

  double centerx =
      A->matrix[min_max_x[0]][0] +
      (A->matrix[min_max_x[1]][0] - A->matrix[min_max_x[0]][0]) / 2;
  double centery =
      A->matrix[min_max_y[0]][1] +
      (A->matrix[min_max_y[1]][1] - A->matrix[min_max_y[0]][1]) / 2;
  double centerz =
      A->matrix[min_max_z[0]][2] +
      (A->matrix[min_max_z[1]][2] - A->matrix[min_max_z[0]][2]) / 2;

  for (int i = 1; i < A->rows; i++) {
    A->matrix[i][0] -= centerx;
    A->matrix[i][1] -= centery;
    A->matrix[i][2] -= centerz;
  }
  free(min_max_x);
  free(min_max_y);
  free(min_max_z);
}

void first_init_val_gl(matrix_t *A) {
  int *min_max_x = NULL;
  int *min_max_y = NULL;
  int *min_max_z = NULL;
  const double value = 0.5;
  double scal;
  double dmax;

  scal = 0.5;
  found_min_max_or(A, &min_max_x, &min_max_y, &min_max_z);

  double delx, dely, delz;

  delx = A->matrix[min_max_x[1]][0] - A->matrix[min_max_x[0]][0];
  dely = A->matrix[min_max_y[1]][1] - A->matrix[min_max_y[0]][1];
  delz = A->matrix[min_max_z[1]][2] - A->matrix[min_max_z[0]][2];

  dmax = delx;
  if (dely > dmax) dmax = dely;
  if (delx > dmax) dmax = delz;

  scal = (value - (value * (-1))) / dmax;

  for (int i = 1; i < A->rows; ++i)
    for (int j = 0; j < A->cols; ++j) {
      A->matrix[i][j] *= scal;
    }

  free(min_max_x);
  free(min_max_y);
  free(min_max_z);
}

void first_init_val(matrix_t *A) {
  int *min_max_x = NULL;
  int *min_max_y = NULL;
  int *min_max_z = NULL;

  found_min_max_or(A, &min_max_x, &min_max_y, &min_max_z);

  double centerx =
      A->matrix[min_max_x[0]][0] +
      (A->matrix[min_max_x[1]][0] - A->matrix[min_max_x[0]][0]) / 2;
  double centery =
      A->matrix[min_max_y[0]][1] +
      (A->matrix[min_max_y[1]][1] - A->matrix[min_max_y[0]][1]) / 2;
  double centerz =
      A->matrix[min_max_z[0]][2] +
      (A->matrix[min_max_z[1]][2] - A->matrix[min_max_z[0]][2]) / 2;

  for (int i = 1; i < A->rows; i++) {
    A->matrix[i][0] -= centerx;
    A->matrix[i][1] -= centery;
    A->matrix[i][2] -= centerz;
  }

  const double value = 0.3;
  double scal;
  double dmax;

  scal = 0.5;

  double delx, dely, delz;

  delx = A->matrix[min_max_x[1]][0] - A->matrix[min_max_x[0]][0];
  dely = A->matrix[min_max_y[1]][1] - A->matrix[min_max_y[0]][1];
  delz = A->matrix[min_max_z[1]][2] - A->matrix[min_max_z[0]][2];

  dmax = delx;
  if (dely > dmax) dmax = dely;
  if (delx > dmax) dmax = delz;

  scal = (value - (value * (-1))) / dmax;

  for (int i = 1; i < A->rows; ++i)
    for (int j = 0; j < A->cols; ++j) {
      A->matrix[i][j] *= scal;
    }

  free(min_max_x);
  free(min_max_y);
  free(min_max_z);
}

void move_x(matrix_t *A, double a) {
  for (int i = 0; i < A->rows; i++) A->matrix[i][0] += a;
}

void move_y(matrix_t *A, double a) {
  for (int i = 0; i < A->rows; i++) A->matrix[i][1] += a;
}

void move_z(matrix_t *A, double a) {
  for (int i = 0; i < A->rows; i++) A->matrix[i][2] += a;
}

void rotation_by_ox(matrix_t *A, double angle) {
  for (int i = 0; i < A->rows; i++) {
    double temp_y = A->matrix[i][1];
    double temp_z = A->matrix[i][2];
    A->matrix[i][1] = cos(angle) * temp_y + sin(angle) * temp_z;
    A->matrix[i][2] = -sin(angle) * temp_y + cos(angle) * temp_z;
  }
}

void rotation_by_oy(matrix_t *A, double angle) {
  for (int i = 0; i < A->rows; i++) {
    double temp_x = A->matrix[i][0];
    double temp_z = A->matrix[i][2];
    A->matrix[i][0] = cos(angle) * temp_x + sin(angle) * temp_z;
    A->matrix[i][2] = -sin(angle) * temp_x + cos(angle) * temp_z;
  }
}

void rotation_by_oz(matrix_t *A, double angle) {
  for (int i = 0; i < A->rows; i++) {
    double temp_x = A->matrix[i][0];
    double temp_y = A->matrix[i][1];
    A->matrix[i][0] = cos(angle) * temp_x + sin(angle) * temp_y;
    A->matrix[i][1] = -sin(angle) * temp_x + cos(angle) * temp_y;
  }
}

void scale_model(matrix_t *A, double alp) {
  if (alp)
    for (int i = 0; i < A->rows; i++) {
      A->matrix[i][0] *= alp;
      A->matrix[i][1] *= alp;      
      A->matrix[i][2] *= alp;
    }

}
