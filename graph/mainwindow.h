#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <fstream>
#include <map>
#include <time.h>
#include <QVector>
#include <QPointF>
#include "mygraph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_horizontalSlider_choose_color_sliderMoved(int position);

    void on_pushButton_axis_clicked();

    void on_radioButton_file_clicked();

    void on_radioButton_rand_clicked();

    void on_pushButton_buildgraph_clicked();

    void on_pushButton_select_file_clicked();

    bool readFile(std::string filename,  QVector<QPointF>& points);

    void on_pushButton_delete_clicked();


    void on_horizontalScrollBar_valueChanged(int value);
    void on_horizontalScrollBar_sliderPressed();
    void on_horizontalScrollBar_sliderReleased();
    void on_verticalScrollBar_valueChanged(int value);
    void on_verticalScrollBar_sliderPressed();
    void on_verticalScrollBar_sliderReleased();
    void on_horizontalSlider_valueChanged(int value);



protected:
    //Отрисовка
    void paintEvent(QPaintEvent *) override;

    //Нажатие мыши
    virtual void mousePressEvent(QMouseEvent*) override;

private:
    Ui::MainWindow *ui;

    //Массив графиков
    QVector<MyGraph> vecGraph;

    //Название выбранного файла
    QString filename;

    //Размеры активного окна
    float myWidth, myHeight;

    //Абсолютный центр окна
    float centerX, centerY;

    //Абсолютное начало окна
    float windowX, windowY;

    //Глобальное расположение мыши
    QPoint mousePos;

    //Пересечиние графика и мышки
    bool collision = false;

    //Отсут от groupBox, выставляется в коде!
    float punding_x_left = 0;

    //Локальные координаты мыши
    float mouseX, mouseY;

    //Точка пересечения графика и мышки
    QPointF ans_point;

    //Скроллы и их активность
    bool scrool_x_active = false;
    bool scrool_y_active = false;
    float scrool_x = 0;
    float scrool_y = 0;

    //Обновление скроллов
    void change_horizontalScrollBar();
    void change_verticalScrollBar();

    //Приближение коэф
    float zoom = 1;
};
#endif // MAINWINDOW_H

