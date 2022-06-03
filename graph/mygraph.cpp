#include "mygraph.h"

//Функция для сортировки Qsort
bool comp(const QPointF a, const QPointF b)
{
    return a.x() > b.x();
}

void my_sort(QVector<QPointF> &arr_points)
{
    for (int i = 0; i < arr_points.size() - 1; i++) {
            for (int j = 0; j < arr_points.size() - i - 1; j++) {
                if (arr_points[j].x() >  arr_points[j + 1].x()) {
                    // меняем элементы местами
                    QPointF temp = arr_points[j];
                    arr_points[j] = arr_points[j + 1];
                    arr_points[j + 1] = temp;
                }
            }
        }
}


void QuickSort(QVector<QPointF> &Arr, int begin, int end)
{
    int mid;
    int i = begin;
    int j = end;
    mid = Arr[(i + j) / 2].x();
    while (i < j)
    {
        while (Arr[i].x() < mid)
            i++;

        while (Arr[j].x() > mid)
            j--;

        if (i <= j)
        {
            QPointF temp = Arr[j];
            Arr[j] = Arr[i];
            Arr[i] = temp;

            i++;
            j--;
        }
    }

    if (begin < j)
        QuickSort(Arr, begin, j);
    if (i < end)
        QuickSort(Arr, i, end);
}

float My_abs(float x)
{
    if (x < 0)
        return (x*(-1));

    return x;
}


MyGraph::MyGraph(QVector<QPointF> vec, QString name, QColor color)
{
    //Получаем точки графика и сортируем по X
    arr_points = vec;

    //Цвет графика
    this->colorGraph = color;
    //Имя графика
    this->nameGraph = name;

    QuickSort(arr_points, 0 , arr_points.size()-1);

    //Находим крайние точки графика, чтобы представить прямоугольник
    for (int i = 0; i < arr_points.size() - 1; i++)
    {
        if (arr_points[i].x() > maxX)
            maxX = arr_points[i].x();
        if (arr_points[i].x() < minX)
            minX = arr_points[i].x();

        if (arr_points[i].y() > maxY)
            maxY = arr_points[i].y();
        if (arr_points[i].y() < minY)
            minY = arr_points[i].y();
    }
}

//Проверка пересечения мышки и графика (Старая версия через вектора -> работает не стабильно)
bool MyGraph::check_collision(float x, float y)
{

    for (int i = 0; i < arr_points.size() - 1; i++)
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
bool MyGraph::check_collision2(float x, float y, QPointF &ans, float zoom)
{
    //Проходим по всем линиям графика
    for (int i = 0; i < arr_points.size() - 1; i++)
    {
       //Левая точка линии
       float x1 = arr_points[i].x();
       float y1 = arr_points[i].y();

       //Правая точка линии
       float x2 = arr_points[i + 1].x();
       float y2 = arr_points[i + 1].y();

       //Угол наклона линии
       float k = (y2 - y1) / (x2 -x1);


       if (My_abs(k) > 1) //Приблизительно горизонтальная линия
       {
           //Проверка диапазона по Y
           bool ans_y = (y <= fmax(y1, y2)) && (y >= fmin(y1, y2));

           //Проверка значения X через уравнение прямой
           float real_x = (( (y - y1) * (x2 - x1) ) / (y2 - y1)) + x1;
           bool ans_x = (My_abs(real_x - x) < 6/zoom);

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
           bool ans_y = (My_abs(real_y - y) < 6/zoom);

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

void MyGraph::set_select(bool value)
{
    selected = value;
}

bool MyGraph::get_select()
{
    return selected;
}
