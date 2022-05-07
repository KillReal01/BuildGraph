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

    ui->horizontalSlider->setMaximum(9); //+1 тк 0
    ui->horizontalSlider->setMinimum(-10);
    ui->horizontalSlider->setValue(0);

    //Координаты окна (виджета)
    windowX = ui->widget->geometry().x();
    windowY = ui->widget->geometry().y();

    myWidth = ui->widget->geometry().width();
    myHeight = ui->widget->geometry().height();

    centerX = windowX + myWidth / 2;
    centerY = windowY  + myHeight / 2;

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
    update();
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

void MainWindow::change_horizontalScrollBar()
{
    //Находим крайние X графиков
    float minX = 99999, maxX = -99999;
    for (int i = 0; i < std::size(vecGraph); i++)
    {
        float new_minX = vecGraph[i].get_minX();
        if (new_minX < minX)
            minX = new_minX;

        float new_maxX = vecGraph[i].get_maxX();
        if (new_maxX > maxX)
            maxX = new_maxX;
    }
    minX -= 100;
    maxX += 150;

    //Растягиваем скролл
    if (minX  + myWidth / 2 < 0)
        ui->horizontalScrollBar->setMinimum(minX + myWidth / 2);
    else
        ui->horizontalScrollBar->setMinimum(0);
    if (maxX - myWidth / 2 > 0)
        ui->horizontalScrollBar->setMaximum((maxX - myWidth / 2));
    else
        ui->horizontalScrollBar->setMaximum(0);

    //Прячем или показываем
    if (maxX > myWidth / 2 || minX < myWidth / -2)
        ui->horizontalScrollBar->show();
    else
    {
        scrool_x = 0;
        ui->horizontalScrollBar->hide();
    }

    //qDebug() << ui->horizontalScrollBar->minimum() << "  |  " << ui->horizontalScrollBar->maximum();
}

void MainWindow::change_verticalScrollBar()
{
    //Находим крайние Y графиков
    float minY = 99999, maxY = -99999;
    for (int i = 0; i < std::size(vecGraph); i++)
    {
        float new_minY = vecGraph[i].get_minY();
        if (new_minY < minY)
            minY = new_minY;

        float new_maxY = vecGraph[i].get_maxY();
        if (new_maxY > maxY)
            maxY = new_maxY;
    }
    minY -= 150;
    maxY += 100;

    //Растягиваем скролл
    if (minY  + myHeight / 2 < 0)
        ui->verticalScrollBar->setMaximum(-(minY + myHeight / 2));
    else
        ui->verticalScrollBar->setMaximum(0);
    if (maxY - myHeight / 2 > 0)
        ui->verticalScrollBar->setMinimum(-(maxY - myHeight / 2));
    else
        ui->verticalScrollBar->setMinimum(0);

    //Прячем или показываем
    if (maxY > myHeight / 2 || minY < myHeight / -2)
        ui->verticalScrollBar->show();
    else
    {
        scrool_y = 0;
        ui->verticalScrollBar->hide();
    }

    //qDebug() << ui->verticalScrollBar->minimum() << "  |  " << ui->verticalScrollBar->maximum();
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

    //Заполнение вектора графиков
    QColor color = ui->widget_color->palette().window().color();
    MyGraph graph(points, ui->line_edit_graph_name->text(), color);
    vecGraph.push_back(graph);

    update();
}

//Событие: Нажатие кнопки мыши
void MainWindow::mousePressEvent(QMouseEvent *)
{
    //Глобальные координаты мыши
    mousePos = this->mapFromGlobal(QCursor::pos());

    //Локальные координаты мыши
    mouseX = mousePos.x() / zoom - (myWidth/2 + windowX / zoom + scrool_x / zoom);
    mouseY = (-mousePos.y() / zoom + myHeight/2 + windowY / zoom + scrool_y / zoom);

    //Проверка пересечний мышки с графиком
    for (int i = 0; i < std::size(vecGraph); i++)
    {
        if (vecGraph[i].check_collision2(mouseX, mouseY, ans_point))
            collision = true;
    }

    //Формируем ответ для statusBar
    QString mousePosToStr = QString("Выбранная точка (x=%1, y=%2)").arg(ans_point.x()).arg(ans_point.y());
    if (collision)
    {
        ui->statusbar->showMessage(mousePosToStr);
        repaint();
    }

    ui->statusbar->showMessage(QString::number(mouseX) + " " + QString::number(mouseY));
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);

    //Контур
    painter.setPen(Qt::black);
    painter.setBrush(Qt::white);
    painter.drawRect(centerX - myWidth/2, centerY - myHeight/2, myWidth, myHeight);

    painter.translate(centerX, centerY);

    //Обновление sroll
    change_horizontalScrollBar();
    change_verticalScrollBar();

    //Оси
    painter.drawLine(-myWidth/2, scrool_y, myWidth/2, scrool_y);
    painter.drawLine(scrool_x, myHeight/2, scrool_x, -myHeight/2);

    //деления
    /*
    int delta = 10;
    int size = 2;
    for (int i = 0; i < myWidth/delta; i++){
        painter.drawLine(-myWidth/2 * zoom + delta * i, scrool_y - size, -myWidth/2 * zoom + delta * i, scrool_y + size);
    }
    for (int i = 0; i < myHeight/delta; i++){
        painter.drawLine(scrool_x - size, -myHeight/2 * zoom + delta * i, scrool_x + size, -myHeight/2 * zoom + delta * i);
    }
    */

    //Отрисовка графиков
    for (int j = 0; j < std::size(vecGraph); j++)
    {
        for (int i = 0; i < std::size(vecGraph[j].arr_points) - 1; i++)
        {
            QLineF line(
                        (vecGraph[j].arr_points[i].x() * zoom+ scrool_x),
                        (-vecGraph[j].arr_points[i].y() * zoom + scrool_y),
                        (vecGraph[j].arr_points[i+1].x() * zoom + scrool_x),
                        (-vecGraph[j].arr_points[i+1].y() * zoom + scrool_y)
                        );

            QPen myPen(vecGraph[j].colorGraph, 2, Qt::SolidLine, Qt::SquareCap);
            painter.setPen(myPen);
            painter.drawLine(line);
        }
    }

    //Отрисовка точки касания
    painter.setPen(Qt::red);
    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
    if (collision)
    {
        painter.drawEllipse((ans_point.x() * zoom + scrool_x) - 3, (-ans_point.y() * zoom + scrool_y) - 3, 6, 6);
    }

    //Подпись осей
    QPen myPen(Qt::black, 5, Qt::SolidLine, Qt::SquareCap);
    painter.setPen(myPen);

    QFont font = painter.font();
    font.setBold(QFont::Bold);
    painter.setFont(font);

    painter.drawText(QPoint(myWidth/2 - ui->line_edit_axisX->text().size() * 5.75 - 25, -5 + scrool_y), ui->line_edit_axisX->text());
    painter.drawText(QPoint(5 + scrool_x, -myHeight / 2 + 25), ui->line_edit_axisY->text());

    painter.end();
}

void MainWindow::on_horizontalScrollBar_valueChanged(int value)
{
    qDebug() << value;
    scrool_x = -value * zoom;
    if (scrool_x_active)
        repaint();
}


void MainWindow::on_horizontalScrollBar_sliderPressed()
{
    scrool_x_active = true;
}


void MainWindow::on_horizontalScrollBar_sliderReleased()
{
    scrool_x_active = false;
}


void MainWindow::on_verticalScrollBar_valueChanged(int value)
{
    scrool_y = -value * zoom; //&вниз?
    if (scrool_y_active)
        repaint();
}


void MainWindow::on_verticalScrollBar_sliderPressed()
{
    scrool_y_active = true;
}


void MainWindow::on_verticalScrollBar_sliderReleased()
{
    scrool_y_active = false;
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    qDebug() << value;
    if (value >= 0)
    {
        zoom = value + 1;
    }
    else
    {
        zoom = 1.0F/(-value + 1);
    }

    repaint();
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
    vecGraph.clear();
    collision = false;
    update();
}

