#include "mygraph.h"

//Функция для сортировки Qsort
bool comp(const QPointF a, const QPointF b)
{
    return a.x() > b.x();
}


MyGraph::MyGraph(QVector<QPointF> vec, QString name, QColor color)
{
    //Получаем точки графика и сортируем по X
    arr_points = vec;

    //Цвет графика
    this->colorGraph = color;

    //Название графика
    this->nameGraph = name;

    std::sort(arr_points.begin(), arr_points.end(), comp);

    //Находим крайние точки графика, чтобы представить прямоугольник
    for (auto point = arr_points.begin(); point < arr_points.end(); point++)
    {
        if (point->x() > maxX)
            maxX = point->x();
        if (point->x() < minX)
            minX = point->x();

        if (point->y() > maxY)
            maxY = point->y();
        if (point->y() < minY)
            minY = point->y();
    }
}

//Проверка пересечения мышки и графика (Старая версия через вектора -> работает не стабильно)
bool MyGraph::check_collision(float x, float y)
{

    for (int i = 0; i < std::size(arr_points) - 1; i++)
    {
        QPointF P1 = arr_points[i];
        QPointF P2 = arr_points[i + 1];
        QPointF M = QPointF(x, y);

        QPointF P1_P2 = QPointF(P2.x() - P1.x(), P2.y() - P1.y());
        QPointF P1_M = QPointF(M.x() - P1.x(), M.y() - P1.y());
        float kos_pr = (P1_P2.x() * P1_M.y()) - (P1_P2.y() * P1_M.x());

        QPointF M_P1 = QPointF(P1.x() - M.x(), P1.y() - M.y());
        QPointF M_P2 = QPointF(P2.x() - M.x(), P2.y() - M.y());
        float scal_pr = (M_P1.x() * M_P2.x()) + (M_P1.y() * M_P2.y());

        if ((-50 < kos_pr && kos_pr < 50) && scal_pr <= 0.3)
            return true;

    }

    return false;
}

//Проверка пересечения мышки и графика (Новая версия через уравнение прямой -> вроде работает круто)
bool MyGraph::check_collision2(float x, float y, QPointF &ans)
{
    //Проходим по всем линиям графика
    for (int i = 0; i < std::size(arr_points) - 1; i++)
    {
       //Левая точка линии
       float x1 = arr_points[i].x();
       float y1 = arr_points[i].y();

       //Правая точка линии
       float x2 = arr_points[i + 1].x();
       float y2 = arr_points[i + 1].y();

       //Угол наклона линии
       float k = (y2 - y1) / (x2 -x1);


       if (abs(k) > 1) //Приблизительно горизонтальная линия
       {
           //Проверка диапазона по Y
           bool ans_y = (y <= fmax(y1, y2)) && (y >= fmin(y1, y2));

           //Проверка значения X через уравнение прямой
           float real_x = (( (y - y1) * (x2 - x1) ) / (y2 - y1)) + x1;
           bool ans_x = (abs(real_x - x) < 10);

           //Пересечение найдено, возращаем точку косания через адрес
           if (ans_x & ans_y)
           {
               ans.setX(real_x);
               ans.setY(y);
               return true;
           }
       }
       else //Приблизительно вертикальная линия
       {
           //Проверка диапазона по X
           bool ans_x = (x >= x2) && (x <= x1);

           //Проверка значения Y через уравнение прямой
           float real_y = (( (x - x1) * (y2 - y1) ) / (x2 - x1)) + y1;
           bool ans_y = (abs(real_y - y) < 10);

           //Пересечение найдено, возращаем точку косания через адрес
           if (ans_x & ans_y)
           {
               ans.setX(x);
               ans.setY(real_y);
               return true;
           }
       }
    }

    //Пересечение НЕ найдено
    return false;
}


// Гетеры для максимальных и минимальных точек графика
float MyGraph::get_maxX()
{
    return maxX;
}

float MyGraph::get_minX()
{
    return minX;
}

float MyGraph::get_maxY()
{
    return maxY;
}

float MyGraph::get_minY()
{
    return minY;
}
