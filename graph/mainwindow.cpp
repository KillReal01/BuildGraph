#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "directwindow.h"
#include <iostream>

std::map<double, double> points;//массив точек
const int N = 100;//100 случайных точек

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget_color->setStyleSheet(QString("background-color: rgb(0, 0, 0);"));

    //Инициализация области рисования
    srand (time(NULL));
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Слайдер для выбора цвета
void MainWindow::on_horizontalSlider_choose_color_sliderMoved(int pos)
{
    union color {
        int integer;
        unsigned char byte[3];
    };
    color n;
    n.integer = pos;
    int r = n.byte[2], g = n.byte[1], b = n.byte[0];
    ui->widget_color->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b));
}

//Кнопка переименовать оси
void MainWindow::on_pushButton_axis_clicked()
{
    ui->axisX->setText(ui->line_edit_axisX->text());
    ui->axisY->setText(ui->line_edit_axisY->text());
}

//Выбор случайных величин, в качестве данных
void MainWindow::on_radioButton_rand_clicked()
{
    ui->pushButton_select_file->setEnabled(false);
    ui->pushButton_buildgraph->setEnabled(true);
}

//Выбор файла, в качестве данных
void MainWindow::on_radioButton_file_clicked()
{
    ui->pushButton_select_file->setEnabled(true);
    ui->pushButton_buildgraph->setEnabled(true);
}


//Кнопка построить график
void MainWindow::on_pushButton_buildgraph_clicked()
{
    points.clear();//чистим массив точек
    if (ui->radioButton_rand->isChecked()){
        double x, y;
        for (int i = 0; i < N; i++){
            x = rand() % 200 - 100;
            y = rand() % 200 - 100;
            points.insert({x, y});
        }
    }
    if (ui->radioButton_file->isChecked()){
        readFile(filename.toStdString(), points);
    }

    update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    int window_x = ui->widget->geometry().x();
    int window_y = ui->widget->geometry().y();

    int window_w = ui->widget->geometry().width();
    int window_h = ui->widget->geometry().height();

    int center_x = window_x + window_w / 2;
    int center_y = window_y  +window_h / 2;

    QPainter painter;
    painter.begin(this);
    painter.drawRect(window_x, window_y, window_w, window_h);

    painter.translate(center_x, center_y);

    //Оси
    int delta = 10;
    int size = 2;
    painter.drawLine(-window_w/2, 0, window_w/2, 0);
    painter.drawLine(0, window_h/2, 0, -window_h/2);
    for (int i = 0; i < window_w/delta; i++){
        painter.drawLine(-window_w/2 + delta * i, -size, -window_w/2 + delta * i, size);
    }
    for (int i = 0; i < window_h/delta; i++){
        painter.drawLine(-size, -window_h/2 + delta * i, size, -window_h/2 + delta * i);
    }

    //График
    painter.setPen(ui->widget_color->palette().window().color());
    QPainterPath path;
    path.moveTo(points.begin()->first, points.begin()->second);
    for (auto &i : points){
         path.lineTo(i.first, i.second);
    }

    painter.drawPath(path);

    painter.end();

}

//Кнопка выбора файла
void MainWindow::on_pushButton_select_file_clicked()
{
    DirectWindow dialog;
    dialog.setModal(true);
    dialog.exec();
    filename = dialog.sendTitle();
}

bool MainWindow::readFile(std::string filename, std::map<double, double>& points){
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        qDebug() << "file can not be open!";
        return false;
    }
    else {
        qDebug() << "file opened!";
        double x, y;
        while (!file.eof())
        {
            file >> x >> y;
            points.insert({x, -y});//то что выше оси ох < 0
        }
        file.close();
        return true;
    }
}





