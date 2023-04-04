#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <time.h>

extern "C" {
    #include "../parser.h"
    #include "../transformations.h"
}


class MyWidget : public QOpenGLWidget
{
    Q_OBJECT


public:
    explicit MyWidget(QWidget *parent = nullptr);
    void initializeGL();
    void resizeGL(int h, int w);
    void paintGL();
    void initialize_data();

    char *filename = NULL;
    data_t *data = NULL;

    void translate_x(QString line);
    void translate_y(QString line);
    void translate_z(QString line);
    void rotate_x(QString line);
    void rotate_y(QString line);
    void rotate_z(QString line);
    void scale(QString line);
signals:

};

#endif // MYWIDGET_H
