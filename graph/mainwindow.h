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

protected:
    void paintEvent(QPaintEvent *) override;

private:
    Ui::MainWindow *ui;

    //Название выбранного файла
    QString filename;

    //Размеры активного окна
    float myWidth;
    float myHeight;

    //Глобальное расположение мыши
    QPoint mousePos;

    //Пересечиние графика и мышки
    bool collision = false;

    //Отсут от groupBox, выставляется в коде!
    float punding_x_left = 0;

    //Локальные координаты мыши
    float mouseX;
    float mouseY;

    //Точка пересечения графика и мышки
    QPointF ans_point;

    //Массив графиков
    QVector<MyGraph> vecGraph;

    //Цвета
    std::vector<QColor> vecColor;
};
#endif // MAINWINDOW_H

