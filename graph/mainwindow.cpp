#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "directwindow.h"
#include <iostream>

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
    QVector<QPointF> points;
    if (ui->radioButton_rand->isChecked()){
        double x, y;
        int h = ui->widget->geometry().height();
        int w = ui->widget->geometry().width();
        for (int i = 0; i < N; i++){
            x = rand() % w - w/2;
            y = rand() % h - h/2;
            points.push_back(QPointF(x, y));
        }
    }
    if (ui->radioButton_file->isChecked()){
        readFile(filename.toStdString(), points);
    }

    //Цвет
    QColor color = ui->widget_color->palette().window().color();
    vecColor.push_back(color);
    vecGraph.push_back(points);

    update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    int window_x = ui->widget->geometry().x();
    int window_y = ui->widget->geometry().y();

    int window_w = ui->widget->geometry().width();
    int window_h = ui->widget->geometry().height();

    int center_x = window_x + window_w / 2;
    int center_y = window_y  + window_h / 2;

    QPainter painter;
    painter.begin(this);

    painter.setBrush(Qt::white);
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
    for (int j = 0; j < std::size(vecGraph); j++)
    {
        for (int i = 0; i < std::size(vecGraph[j].arr_points) - 1; i++)
        {
            QLineF line(vecGraph[j].arr_points[i].x(), -vecGraph[j].arr_points[i].y(), vecGraph[j].arr_points[i+1].x(), -vecGraph[j].arr_points[i+1].y() );
            painter.setPen(vecColor[j]);
            painter.drawLine(line);
        }
    }
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

bool MainWindow::readFile(std::string filename,  QVector<QPointF>& points){
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
            points.push_back(QPoint(x, -y));//то что выше оси ох < 0
        }
        file.close();
        return true;
    }
}






void MainWindow::on_pushButton_delete_clicked()
{
    vecColor.clear();
    vecGraph.clear();
    update();
}

