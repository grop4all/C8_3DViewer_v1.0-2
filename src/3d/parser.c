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
  
  long int start = clock();
  FILE *file;
  fpos_t pos;
  int tmp;
  char *currline;
  size_t len;
  int ans;
  char *buff_line;


  len = 128;
  ans = 1;
  tmp = 0;
  buff_line = NULL;
  currline = NULL;

  if (NULL == (file = fopen(filename, "r"))) return 0;
  if (data == NULL) return 0;

  fgetpos(file, &pos);
  int puff;
  while (EOF != (tmp = getc(file))) {
    if ('f' == tmp) {
      puff = getc(file);
      if (puff == ' ') data->count_of_facets += 1;
    }
    if ('v' == tmp) {
      puff = getc(file);
      if (puff == ' ') data->count_of_vertexes += 1;
    }
  }

  fsetpos(file, &pos);
  init_data(data);
  int v = 1, f = 1;
  while (!feof(file)) {
    tmp = getc(file);
    if ('v' == tmp) {
      puff = getc(file);
      if (puff == ' ') {
        if (fscanf(file, "%lf%lf%lf", &data->matrix_3d.matrix[v][0],
                   &data->matrix_3d.matrix[v][1],
                   &data->matrix_3d.matrix[v][2]))
          ++v;
      }
    }
    if ('f' == tmp) {
      puff = getc(file);
      if (puff == ' ') {
        if (getline(&currline, &len, file)) {
          buff_line = malloc(len);
          for (int i = 0; currline[i] != '\0'; ++i) buff_line[i] = currline[i];
          init_polygon(data, buff_line, f);
          ++f;
          free(buff_line);
        }
      }
    }
  }
  free(currline);
  fclose(file);
  printf("\n%lf\n",start - clock() * 1.0 / CLOCKS_PER_SEC);
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
    int count = 1;
    char *buff_line = NULL;
    buff_line = malloc(strlen(line) + 1);
    char *tmp = NULL;
    char *delim = " ";

    strcpy(buff_line, line);
    strtok(buff_line, delim);
    for (; strtok(NULL, delim) != NULL; ++count)
      ;

    data->polygons[index].numbers_of_vertexes_in_facets = count;
    data->polygons[index].vertexes = malloc(sizeof(int) * count);
    if (data->polygons[index].vertexes == NULL) ans = 0;
    strcpy(buff_line, line);
    tmp = strtok(buff_line, delim);
    for (int i = 0; i < data->polygons[index].numbers_of_vertexes_in_facets;
         ++i) {
      data->polygons[index].vertexes[i] = atoi(tmp);
      tmp = strtok(NULL, delim);
    }
    free(tmp);
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
