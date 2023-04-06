#include "parser.h"

data_t *create_data() {
  data_t *data = NULL;
  data = (data_t *)malloc(sizeof(data_t));
  data->count_of_facets = 0;
  data->count_of_vertexes = 0;
  data->matrix_3d.matrix = NULL;
  data->matrix_3d.cols = 0;
  data->matrix_3d.rows = 0;
  data->polygons = NULL;
  return data;
}

int parsline(char *filename, data_t *data) {
  FILE *file;
  fpos_t pos;
  char *currline;
  size_t len;
  int ans;

  len = 128;
  ans = 1;
  currline = malloc(len);

  if (NULL == (file = fopen(filename, "r"))) return 0;
  if (data == NULL) return 0;

  fgetpos(file, &pos);
  while (-1 != getline(&currline, &len, file)) {
    if ('f' == currline[0] && ' ' == currline[1]) data->count_of_facets += 1;
    if ('v' == currline[0] && ' ' == currline[1]) data->count_of_vertexes += 1;
  }
  fsetpos(file, &pos);
  init_data(data);
  int v = 1, f = 1;
  while (-1 != getline(&currline, &len, file)) {
    if ('v' == currline[0] && ' ' == currline[1]) {
      if (0 != sscanf(currline, "v %lf%lf%lf", &data->matrix_3d.matrix[v][0],
                      &data->matrix_3d.matrix[v][1],
                      &data->matrix_3d.matrix[v][2])) {
        ++v;
      }
    }
    if ('f' == currline[0] && ' ' == currline[1]) {
      init_polygon(data, currline, f);
      ++f;
    }
  }
  free(currline);
  fclose(file);
  return ans;
}

int init_data(data_t *data) {
  // подсчет с 1
  int ans;
  ans = 1;
  if (data == NULL)
    ans = 0;
  else {
    data->matrix_3d.rows = data->count_of_vertexes + 1;
    data->matrix_3d.cols = 3;
    data->matrix_3d.matrix =
        (double **)malloc(sizeof(double *) * data->matrix_3d.rows);

    for (int i = 0; i < data->matrix_3d.rows; ++i)
      data->matrix_3d.matrix[i] =
          (double *)malloc(sizeof(double) * data->matrix_3d.cols);

    for (int i = 0; i < data->matrix_3d.rows; ++i)
      for (int j = 0; j < 3; ++j) data->matrix_3d.matrix[i][j] = .0;

    data->polygons = malloc(sizeof(polygon_t) * (data->count_of_facets + 1));
    data->polygons[0].vertexes = malloc(1);
  }
  return ans;
}

int init_polygon(data_t *data, char *line, int index) {
  int ans;
  ans = 1;
  if (data == NULL)
    ans = 0;
  else if (line == NULL)
    ans = 0;
  else {
    int count = 0;
    char *buff_line = NULL;
    buff_line = malloc(strlen(line) + 1);
    char *tmp = NULL;
    char *delim = " ";

    strcpy(buff_line, line);
    tmp = strtok(buff_line, delim);
    for (; (tmp = strtok(NULL, delim)) != NULL && (atoi(tmp) != 0); ++count)
      ;

    data->polygons[index].numbers_of_vertexes_in_facets = count;
    data->polygons[index].vertexes = malloc(sizeof(int) * count);
    if (data->polygons[index].vertexes == NULL) ans = 0;
    strcpy(buff_line, line);
    tmp = strtok(buff_line, delim);
    tmp = strtok(NULL, delim);
    for (int i = 0; i < data->polygons[index].numbers_of_vertexes_in_facets;
         ++i) {
      int digit = atoi(tmp);
      if (digit != 0) data->polygons[index].vertexes[i] = digit;
      tmp = strtok(NULL, delim);
    }
    free(buff_line);
  }
  return ans;
}

void destroy_data(data_t **datta) {
  data_t *data = *datta;
  for (int i = 0; i <= data->count_of_facets; ++i) {
    free(data->polygons[i].vertexes);
    data->polygons[i].vertexes = NULL;
  }
  free(data->polygons);
  data->polygons = NULL;
  for (int i = 0; i < data->matrix_3d.rows; ++i) {
    free(data->matrix_3d.matrix[i]);
    data->matrix_3d.matrix[i] = NULL;
  }
  free(data->matrix_3d.matrix);
  data->matrix_3d.matrix = NULL;
  free(data);
  *datta = NULL;
}
