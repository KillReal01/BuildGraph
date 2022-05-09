#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QVector>
#include <QPointF>
#include "mygraph.h"
#include "dialog.h"
#include <fstream>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //Размеры активного окна
    float myWidth;
    float myHeight;

    //Глобальное расположение мыши
    QPoint mousePos;

    //Виджеты
    Ui::MainWindow *ui;

    //Пересечиние графика и мышки
    bool collision = false;

    //Отсут от groupBox, выставляется в коде!
    float punding_x_left = 0;
    float punding_x_right = 0;
    float punding_x = 0;


    //Локальные координаты мыши
    float mouseX;
    float mouseY;

    //Точка пересечения графика и мышки
    QPointF ans_point;

    //Массив графиков
    QVector<MyGraph> vecGraph;

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

    //Подпись осей
    QString titleAxisX = "X";
    QString titleAxisY = "Y";

    //название выбранного файла
    QString filename;

    //Размер рамки
    float frame_size = 0;

protected:
    //Нажатие мыши
    virtual void mousePressEvent(QMouseEvent* event);
    //Отрисовка
    void paintEvent(QPaintEvent *event);

private slots:
    void on_horizontalScrollBar_valueChanged(int value);
    void on_horizontalScrollBar_sliderPressed();
    void on_horizontalScrollBar_sliderReleased();
    void on_verticalScrollBar_valueChanged(int value);
    void on_verticalScrollBar_sliderPressed();
    void on_verticalScrollBar_sliderReleased();
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_horizontalSlider_2_sliderMoved(int position);
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    bool readFile(std::string filename,  QVector<QPointF>& points);
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
};
#endif // MAINWINDOW_H
