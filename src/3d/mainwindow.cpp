#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectSetup();

}


void MainWindow::connectSetup() {
    connect(ui->pushButton, &QPushButton::clicked, this,
            &MainWindow::paintfile);
}


void MainWindow::paintfile() {
    QString path = QFileDialog::getOpenFileName(0, "Open File .obj", "/Users/", "*.obj");
    if (path == "") return;
    QByteArray ba = path.toLocal8Bit();
    ui->widget->filename = ba.data();
    ui->widget->initializeGL();
    ui->widget->initialize_data();
    ui->label_V_count->setText(QString::number(ui->widget->data->count_of_vertexes));
    ui->label_F_count->setText(QString::number(ui->widget->data->count_of_facets));
    ui->widget->update();
}

MainWindow::~MainWindow()
{

    delete ui;
}


void MainWindow::on_Button_translate_x_clicked()
{
   ui->widget->translate_x(ui->lineEdit_translate_x->text());
   ui->widget->update();
}

void MainWindow::on_Button_translate_y_clicked()
{
    ui->widget->translate_y(ui->lineEdit_translate_y->text());
    ui->widget->update();
}


void MainWindow::on_Button_translate_z_clicked()
{
    ui->widget->translate_z(ui->lineEdit_translate_z->text());
    ui->widget->update();
}


void MainWindow::on_Button_rotate_x_clicked()
{
    ui->widget->rotate_x(ui->lineEdit_rotate_x->text());
    ui->widget->update();
}


void MainWindow::on_Button_rotate_y_clicked()
{
    ui->widget->rotate_y(ui->lineEdit_rotate_y->text());
    ui->widget->update();
}


void MainWindow::on_Button_rotate_z_clicked()
{
    ui->widget->rotate_z(ui->lineEdit_rotate_z->text());
    ui->widget->update();
}


void MainWindow::on_Button_scale_clicked()
{
    ui->widget->scale(ui->lineEdit_scale_value->text());
    ui->widget->update();
}

