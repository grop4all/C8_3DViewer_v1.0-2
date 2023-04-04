#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "./parser.h"
#include "./transformations.h"

START_TEST(create_data_1) {
  data_t *data;
  ck_assert_ptr_nonnull(data = create_data());
  free(data);
  data = NULL;
}
END_TEST

START_TEST(parsline_1) {
  char *filename = "data_null_test";
  data_t *data = NULL;
  ck_assert_int_eq(parsline(filename, data), 0);
}
END_TEST

START_TEST(parsline_2) {
  char *filename = "file_not_found";
  data_t *data = NULL;
  data = create_data();
  ck_assert_int_eq(parsline(filename, data), 0);
  ck_assert_ptr_nonnull(data);
  free(data);
}
END_TEST

START_TEST(init_data_1) {
  data_t *data = NULL;
  ck_assert_int_eq(init_data(data), 0);
}
END_TEST

START_TEST(init_data_2) {
  data_t *data;
  int v, f;
  v = 4;
  f = 4;

  data = create_data();
  data->count_of_facets = f;
  data->count_of_vertexes = v;
  init_data(data);

  ck_assert_ptr_nonnull(data->matrix_3d.matrix);
  for (int i = 0; i < data->matrix_3d.rows; ++i)
    ck_assert_ptr_nonnull(data->matrix_3d.matrix[i]);
  ck_assert_ptr_nonnull(data->polygons);

  for (int i = 0; i < data->matrix_3d.rows; ++i)
    free(data->matrix_3d.matrix[i]);

  free(data->matrix_3d.matrix);
  free(data->polygons[0].vertexes);
  free(data->polygons);
  free(data);
}
END_TEST

START_TEST(init_polygon_1) {
  data_t *data = NULL;
  ck_assert_int_eq(init_polygon(data, "data_null_test", 0), 0);
}
END_TEST

START_TEST(init_polygon_2) {
  data_t *data = malloc(sizeof(data_t *));
  ck_assert_int_eq(init_polygon(data, NULL, 0), 0);
  free(data);
}
END_TEST

START_TEST(init_polygon_3) {
  data_t *data;
  int f, v, index;
  char *line;

  v = 4;
  f = 4;
  index = 1;
  line = "1 2 3";

  data = create_data();
  data->count_of_facets = f;
  data->count_of_vertexes = v;

  init_data(data);
  init_polygon(data, line, index);

  ck_assert_ptr_nonnull(data->polygons[index].vertexes);
  for (int i = 0; i < data->polygons[index].numbers_of_vertexes_in_facets; ++i)
    ck_assert_int_eq(data->polygons[index].vertexes[i], i + 1);

  free(data->polygons[index].vertexes);
  free(data->polygons[0].vertexes);
  free(data->polygons);
  for (int i = 0; i < data->matrix_3d.rows; ++i)
    free(data->matrix_3d.matrix[i]);
  free(data->matrix_3d.matrix);
  free(data);
}
END_TEST

START_TEST(destroy_data_1) {
  data_t *data;
  int f, v;

  data = NULL;
  v = 4;
  f = 4;

  data = create_data();
  data->count_of_facets = f;
  data->count_of_vertexes = v;

  init_data(data);
  destroy_data(&data);
  ck_assert_ptr_null(data);
}
END_TEST

START_TEST(found_min_max_or_1) {
  matrix_t matrix;
  int *ptr_x;
  int *ptr_y;
  int *ptr_z;
  matrix.rows = 4;
  matrix.cols = 3;
  matrix.matrix = (double **)malloc(sizeof(double *) * matrix.rows);

  for (int i = 0; i < matrix.rows; ++i)
    matrix.matrix[i] = (double *)malloc(sizeof(double) * matrix.cols);

  for (int i = 0; i < matrix.rows; ++i)
    for (int j = 0; j < 3; ++j) matrix.matrix[i][j] = .0;

  for (int i = 1; i < matrix.rows; ++i) {
    matrix.matrix[i][0] = i;
    matrix.matrix[i][1] = i;
    matrix.matrix[i][2] = i;
  }

  found_min_max_or(&matrix, &ptr_x, &ptr_y, &ptr_z);
  ck_assert_int_eq(ptr_x[0], 1);
  ck_assert_int_eq(ptr_x[1], 3);
  ck_assert_int_eq(ptr_y[0], 1);
  ck_assert_int_eq(ptr_y[1], 3);
  ck_assert_int_eq(ptr_z[0], 1);
  ck_assert_int_eq(ptr_z[1], 3);

  for (int i = 0; i < matrix.rows; ++i) free(matrix.matrix[i]);
  free(matrix.matrix);
  free(ptr_x);
  free(ptr_y);
  free(ptr_z);
}
END_TEST

START_TEST(first_init_val_gl_1) {
  matrix_t matrix;
  matrix.rows = 4;
  matrix.cols = 3;
  matrix.matrix = (double **)malloc(sizeof(double *) * matrix.rows);

  for (int i = 0; i < matrix.rows; ++i)
    matrix.matrix[i] = (double *)malloc(sizeof(double) * matrix.cols);

  for (int i = 0; i < matrix.rows; ++i)
    for (int j = 0; j < 3; ++j) matrix.matrix[i][j] = .0;

  for (int i = 1; i < matrix.rows; ++i) {
    matrix.matrix[i][0] = i;
    matrix.matrix[i][1] = i;
    matrix.matrix[i][2] = i;
  }

  first_init_val_gl(&matrix);

  for (int i = 1; i < matrix.rows; ++i)
    for (int j = 0; j < 3; ++j)
      ck_assert_double_eq(matrix.matrix[i][j], i * 0.5);

  for (int i = 0; i < matrix.rows; ++i) free(matrix.matrix[i]);
  free(matrix.matrix);
}
END_TEST

START_TEST(move_x_1) {
  double arg;
  arg = 1.0;
  matrix_t matrix;
  matrix.rows = 2;
  matrix.cols = 3;
  matrix.matrix = (double **)malloc(sizeof(double *) * matrix.rows);

  for (int i = 0; i < matrix.rows; ++i)
    matrix.matrix[i] = (double *)malloc(sizeof(double) * matrix.cols);

  for (int i = 0; i < matrix.rows; ++i)
    for (int j = 0; j < 3; ++j) matrix.matrix[i][j] = .0;

  for (int i = 1; i < matrix.rows; ++i) {
    matrix.matrix[i][0] = i;
    matrix.matrix[i][1] = i;
    matrix.matrix[i][2] = i;
  }
  move_x(&matrix, arg);

  for (int i = 1; i < matrix.rows; ++i)
    ck_assert_double_eq(matrix.matrix[i][0], i + arg);

  for (int i = 0; i < matrix.rows; ++i) free(matrix.matrix[i]);
  free(matrix.matrix);
}
END_TEST

START_TEST(move_y_1) {
  double arg;
  arg = 1.0;
  matrix_t matrix;
  matrix.rows = 2;
  matrix.cols = 3;
  matrix.matrix = (double **)malloc(sizeof(double *) * matrix.rows);

  for (int i = 0; i < matrix.rows; ++i)
    matrix.matrix[i] = (double *)malloc(sizeof(double) * matrix.cols);

  for (int i = 0; i < matrix.rows; ++i)
    for (int j = 0; j < 3; ++j) matrix.matrix[i][j] = .0;

  for (int i = 1; i < matrix.rows; ++i) {
    matrix.matrix[i][0] = i;
    matrix.matrix[i][1] = i;
    matrix.matrix[i][2] = i;
  }
  move_y(&matrix, arg);

  for (int i = 1; i < matrix.rows; ++i)
    ck_assert_double_eq(matrix.matrix[i][1], i + arg);
  for (int i = 0; i < matrix.rows; ++i) free(matrix.matrix[i]);
  free(matrix.matrix);
}
END_TEST

START_TEST(move_z_1) {
  double arg;
  arg = 1.0;
  matrix_t matrix;
  matrix.rows = 2;
  matrix.cols = 3;
  matrix.matrix = (double **)malloc(sizeof(double *) * matrix.rows);

  for (int i = 0; i < matrix.rows; ++i)
    matrix.matrix[i] = (double *)malloc(sizeof(double) * matrix.cols);

  for (int i = 0; i < matrix.rows; ++i)
    for (int j = 0; j < 3; ++j) matrix.matrix[i][j] = .0;

  for (int i = 1; i < matrix.rows; ++i) {
    matrix.matrix[i][0] = i;
    matrix.matrix[i][1] = i;
    matrix.matrix[i][2] = i;
  }
  move_z(&matrix, arg);

  for (int i = 1; i < matrix.rows; ++i)
    ck_assert_double_eq(matrix.matrix[i][2], i + arg);

  for (int i = 0; i < matrix.rows; ++i) free(matrix.matrix[i]);
  free(matrix.matrix);
}
END_TEST

START_TEST(rotation_by_ox_1) {
  double arg;
  arg = M_PI / 2;
  matrix_t matrix;
  matrix.rows = 2;
  matrix.cols = 3;
  matrix.matrix = (double **)malloc(sizeof(double *) * matrix.rows);

  for (int i = 0; i < matrix.rows; ++i)
    matrix.matrix[i] = (double *)malloc(sizeof(double) * matrix.cols);

  for (int i = 0; i < matrix.rows; ++i)
    for (int j = 0; j < 3; ++j) matrix.matrix[i][j] = .0;

  for (int i = 0; i < matrix.rows; ++i) {
    matrix.matrix[i][0] = 1;
    matrix.matrix[i][1] = 1;
    matrix.matrix[i][2] = 1;
  }

  rotation_by_ox(&matrix, arg);

  for (int i = 0; i < matrix.rows; ++i) {
    ck_assert_double_eq(matrix.matrix[i][0], 1);
    ck_assert_double_eq(matrix.matrix[i][1], cos(arg) + sin(arg));
    ck_assert_double_eq(matrix.matrix[i][2], -sin(arg) + cos(arg));
  }

  for (int i = 0; i < matrix.rows; ++i) free(matrix.matrix[i]);
  free(matrix.matrix);
}
END_TEST

START_TEST(rotation_by_oy_1) {
  double arg;
  arg = M_PI / 2;
  matrix_t matrix;
  matrix.rows = 2;
  matrix.cols = 3;
  matrix.matrix = (double **)malloc(sizeof(double *) * matrix.rows);

  for (int i = 0; i < matrix.rows; ++i)
    matrix.matrix[i] = (double *)malloc(sizeof(double) * matrix.cols);

  for (int i = 0; i < matrix.rows; ++i)
    for (int j = 0; j < 3; ++j) matrix.matrix[i][j] = .0;

  for (int i = 0; i < matrix.rows; ++i) {
    matrix.matrix[i][0] = 1;
    matrix.matrix[i][1] = 1;
    matrix.matrix[i][2] = 1;
  }

  rotation_by_oy(&matrix, arg);

  for (int i = 0; i < matrix.rows; ++i) {
    ck_assert_double_eq(matrix.matrix[i][0], cos(arg) + sin(arg));
    ck_assert_double_eq(matrix.matrix[i][1], 1);
    ck_assert_double_eq(matrix.matrix[i][2], -sin(arg) + cos(arg));
  }
  for (int i = 0; i < matrix.rows; ++i) free(matrix.matrix[i]);
  free(matrix.matrix);
}
END_TEST

START_TEST(rotation_by_oz_1) {
  double arg;
  arg = M_PI / 2;
  matrix_t matrix;
  matrix.rows = 2;
  matrix.cols = 3;
  matrix.matrix = (double **)malloc(sizeof(double *) * matrix.rows);

  for (int i = 0; i < matrix.rows; ++i)
    matrix.matrix[i] = (double *)malloc(sizeof(double) * matrix.cols);

  for (int i = 0; i < matrix.rows; ++i)
    for (int j = 0; j < 3; ++j) matrix.matrix[i][j] = .0;

  for (int i = 0; i < matrix.rows; ++i) {
    matrix.matrix[i][0] = 1;
    matrix.matrix[i][1] = 1;
    matrix.matrix[i][2] = 1;
  }

  rotation_by_oz(&matrix, arg);

  for (int i = 0; i < matrix.rows; ++i) {
    ck_assert_double_eq(matrix.matrix[i][0], cos(arg) + sin(arg));
    ck_assert_double_eq(matrix.matrix[i][1], -sin(arg) + cos(arg));
    ck_assert_double_eq(matrix.matrix[i][2], 1);
  }
  for (int i = 0; i < matrix.rows; ++i) free(matrix.matrix[i]);
  free(matrix.matrix);
}
END_TEST

Suite *s21_3d() {
  Suite *suite;

  suite = suite_create("s21_3d");
  TCase *tcase_create_data = tcase_create("create_data");
  TCase *tcase_parsline = tcase_create("parline");
  TCase *tcase_init_data = tcase_create("init_data");
  TCase *tcase_init_polygon = tcase_create("init_polygon");
  TCase *tcase_destroy_data = tcase_create("destroy_data");
  TCase *tcase_found_min_max_or = tcase_create("found_min_max_or");
  TCase *tcase_first_init_val_gl = tcase_create("first_init_val_gl");
  TCase *tcase_move = tcase_create("move");
  TCase *tcase_rotation_by = tcase_create("rotation_by");

  suite_add_tcase(suite, tcase_create_data);
  tcase_add_test(tcase_create_data, create_data_1);

  suite_add_tcase(suite, tcase_parsline);
  tcase_add_test(tcase_parsline, parsline_1);
  tcase_add_test(tcase_parsline, parsline_2);

  suite_add_tcase(suite, tcase_init_data);
  tcase_add_test(tcase_init_data, init_data_1);
  tcase_add_test(tcase_init_data, init_data_2);

  suite_add_tcase(suite, tcase_init_polygon);
  tcase_add_test(tcase_init_polygon, init_polygon_1);
  tcase_add_test(tcase_init_polygon, init_polygon_2);
  tcase_add_test(tcase_init_polygon, init_polygon_3);

  suite_add_tcase(suite, tcase_destroy_data);
  tcase_add_test(tcase_destroy_data, destroy_data_1);

  suite_add_tcase(suite, tcase_found_min_max_or);
  tcase_add_test(tcase_found_min_max_or, found_min_max_or_1);

  suite_add_tcase(suite, tcase_first_init_val_gl);
  tcase_add_test(tcase_first_init_val_gl, first_init_val_gl_1);

  suite_add_tcase(suite, tcase_move);
  tcase_add_test(tcase_move, move_x_1);
  tcase_add_test(tcase_move, move_y_1);
  tcase_add_test(tcase_move, move_z_1);

  suite_add_tcase(suite, tcase_rotation_by);
  tcase_add_test(tcase_rotation_by, rotation_by_ox_1);
  tcase_add_test(tcase_rotation_by, rotation_by_oy_1);
  tcase_add_test(tcase_rotation_by, rotation_by_oz_1);

  return suite;
}

int main() {
  Suite *suite = s21_3d();
  SRunner *suite_runner = srunner_create(suite);
  srunner_run_all(suite_runner, CK_VERBOSE);
  srunner_free(suite_runner);
  return 0;
}