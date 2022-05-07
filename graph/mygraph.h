#ifndef MYGRAPH_H
#define MYGRAPH_H
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QDebug>
#include <Qt>
#include <QColor>

class MyGraph
{
public:
    // Конструктор класса принимает точки для представления графика
    //MyGraph(QVector<QPointF> vec, Qt::GlobalColor color);
    MyGraph(QVector<QPointF> vec, QString name, QColor color);

    // Массив точек
    QVector<QPointF> arr_points;

    // Проверка пересечения графика с координатами мыши
    bool check_collision(float x, float y);
    bool check_collision2(float x, float y, QPointF &ans);

    // Гетеры для максимальных и минимальных точек графика, чтобы не повредить константы
    float get_maxX();
    float get_minX();
    float get_maxY();
    float get_minY();

    QColor colorGraph;
    QString nameGraph;

private:
    // Максимальные и минимальные точки графика, чтобы вписать в прямоугольник для скролла
    float minX = 0;
    float maxX = 0;
    float maxY = 0;
    float minY = 0;
};

#endif // MYGRAPH_H
