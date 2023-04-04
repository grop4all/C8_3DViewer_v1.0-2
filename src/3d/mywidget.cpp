#include "mywidget.h"




MyWidget::MyWidget(QWidget *parent)
    : QOpenGLWidget{parent}
{

}


void MyWidget::initializeGL(){
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MyWidget::resizeGL(int h, int w) {

}

void MyWidget::paintGL(){
    if (data == NULL) return;
    for(int i = 1; i <= data->count_of_facets; ++i){
        for(int j = 0; j < data->polygons[i].numbers_of_vertexes_in_facets; ++j) {
            glBegin(GL_LINE_LOOP);
            glVertex3d(data->matrix_3d.matrix[data->polygons[i].vertexes[j]][0],
                    data->matrix_3d.matrix[data->polygons[i].vertexes[j]][1],
                   data->matrix_3d.matrix[data->polygons[i].vertexes[j]][2]);
        }
    glEnd();
    }
}

void  MyWidget::initialize_data() {
    if (data != NULL)
        destroy_data(&data);
    data  = create_data();
    parsline(filename, data);
    first_init_val(&data->matrix_3d);
}


void MyWidget::translate_x(QString line) {
    double arg = line.toDouble();
    move_x(&data->matrix_3d, arg);
}

void MyWidget::translate_y(QString line) {
    double arg = line.toDouble();
    move_y(&data->matrix_3d, arg);
}


void MyWidget::translate_z(QString line) {
    double arg = line.toDouble();
    move_z(&data->matrix_3d, arg);
}

void MyWidget::rotate_x(QString line) {
    double arg = line.toDouble();
    rotation_by_ox(&data->matrix_3d, arg);
}

void MyWidget::rotate_y(QString line) {
    double arg = line.toDouble();
    rotation_by_oy(&data->matrix_3d, arg);
}

void MyWidget::rotate_z(QString line) {
    double arg = line.toDouble();
    rotation_by_oz(&data->matrix_3d, arg);
}


void MyWidget::scale(QString line) {
    double arg = line.toDouble();
    scale_model(&data->matrix_3d,arg);
}
