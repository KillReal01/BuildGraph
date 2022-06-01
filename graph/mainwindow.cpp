#include "mainwindow.h"
#include "directwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

float floatMod(float a, float b)
{
    double mod;
    // Handling negative values
    if (a < 0)
        mod = -a;
    else
        mod =  a;
    if (b < 0)
        b = -b;

    // Finding mod by repeated subtraction

    while (mod >= b)
        mod = mod - b;

    // Sign of result typically depends
    // on sign of a.
    if (a < 0)
        return -mod;

    return mod;
}

float define_dist_axis(float value)
{
    if (value < -5)
        return 50;
    else if (value < 0)
        return 25;
    else if (value < 5)
        return 12.5;
    else
        return 12.5;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->horizontalSlider->setMaximum(9); //+1 тк 0
    ui->horizontalSlider->setMinimum(-10);
    ui->horizontalSlider->setValue(0);

    //
    setWindowTitle("Построение графиков");
    ui->widget->setStyleSheet(QString("background-color: rgb(0, 0, 0);"));
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    minX -= (100 + frame_size_y);
    maxX += (150 + frame_size_x);

    //Растягиваем скролл
    if (minX  + myWidth / 2 < 0)
        ui->horizontalScrollBar->setMinimum((minX  + myWidth / 2)*scroll_k);
    else
        ui->horizontalScrollBar->setMinimum(0);
    if (maxX - myWidth / 2 > 0)
        ui->horizontalScrollBar->setMaximum((maxX - myWidth / 2)*scroll_k);
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

    //Обновление размера холста
    canvas_max_x = width() + ui->horizontalScrollBar->maximum()/scroll_k * zoom;
    canvas_min_x = -width() + ui->horizontalScrollBar->minimum()/scroll_k * zoom;
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
    minY -= (150 + frame_size_y);
    maxY += (100 + frame_size_y);


    //Растягиваем скролл
    if (minY  + myHeight / 2 < 0)
        ui->verticalScrollBar->setMaximum(-(minY  + myHeight / 2)*scroll_k);
    else
        ui->verticalScrollBar->setMaximum(0);
    if (maxY - myHeight / 2 > 0)
        ui->verticalScrollBar->setMinimum(-(maxY - myHeight / 2)*scroll_k);
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

    //Обновление размера холста
    canvas_max_y = height() + ui->verticalScrollBar->maximum()/scroll_k * zoom;
    canvas_min_y = -height() + ui->verticalScrollBar->minimum()/scroll_k * zoom;
}



void MainWindow::markup_axis_X2(QPainter *painter)
{
    QFont font = painter->font();
    font.setBold(QFont::Bold);
    painter->setFont(font);

    painter->setPen(QPen(Qt::black, 1));

    float dist = define_dist_axis( ui->horizontalSlider->value() );
    float size = size_markup_axis_line;

    //right
    for (float i = 0; i < canvas_max_x; i += dist)
    {
        if ((int)i % 50 == 0)
        {
            painter->drawText(i + scrool_x,
                             size+10 + scrool_y,
                             QString("%1").arg(round(i/zoom*10)/10.0f)
                             );
            painter->setPen(QPen(Qt::black, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::black, 1));
        }

        painter->drawLine(i + scrool_x,
                          -size + scrool_y,
                          i + scrool_x,
                          size + scrool_y);
    }

    //left
    for (float i = 0; i > canvas_min_x; i -= dist)
    {
        if ((int)-i % 50 == 0)
        {
            painter->drawText(i + scrool_x,
                             size+10 + scrool_y,
                             QString("%1").arg(round(i/zoom*10)/10.0f)
                             );
            painter->setPen(QPen(Qt::black, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::black, 1));
        }

        painter->drawLine(i + scrool_x,
                          -size + scrool_y,
                          i + scrool_x,
                          size + scrool_y
                          );
    }
}
void MainWindow::markup_axis_X2_down(QPainter *painter)
{
    QFont font = painter->font();
    font.setBold(QFont::Bold);
    painter->setFont(font);

    painter->setPen(QPen(Qt::black, 1));

    float dist = define_dist_axis( ui->horizontalSlider->value() );
    float size = size_markup_axis_line;

    //right
    for (float i = 0; i < canvas_max_x; i += dist)
    {

        //Выключить отрисовку за рамкой
        if ((i+scrool_x) > myWidth/2*zoom - frame_size_x)
            continue;
        if ((i+scrool_x) < -myWidth/2*zoom + frame_size_x)
            continue;

        if ((int)i % 50 == 0)
        {
            painter->drawText(i + scrool_x,
                             size + 12 + zoom*myHeight/2 - frame_size_y-30,
                             QString("%1").arg(round(i/zoom*10)/10.0f)
                             );
            painter->setPen(QPen(Qt::black, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::black, 1));
        }

        painter->drawLine(i + scrool_x,
                          size + zoom*myHeight/2 - frame_size_y-30,
                          i + scrool_x,
                          -size + zoom*myHeight/2 - frame_size_y-30
                          );

    }

    //left
    for (float i = 0; i > canvas_min_x; i -= dist)
    {
        //Выключить отрисовку за рамкой
        if ((i+scrool_x) < -myWidth/2*zoom + frame_size_x)
            continue;
        if ((i+scrool_x) > myWidth/2*zoom - frame_size_x)
            continue;

        if ((int)-i % 50 == 0)
        {
            painter->drawText(i + scrool_x,
                             size + 12 + zoom*myHeight/2 - frame_size_y-30,
                             QString("%1").arg(round(i/zoom*10)/10.0f)
                             );
            painter->setPen(QPen(Qt::black, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::black, 1));
        }


        painter->drawLine(i + scrool_x,
                          size + zoom*myHeight/2 - frame_size_y-30,
                          i + scrool_x,
                          -size + zoom*myHeight/2 - frame_size_y-30
                          );
    }
}
void MainWindow::markup_axis_X2_up(QPainter *painter)
{
    QFont font = painter->font();
    font.setBold(QFont::Bold);
    painter->setFont(font);

    painter->setPen(QPen(Qt::black, 1));

    float dist = define_dist_axis( ui->horizontalSlider->value() );
    float size = size_markup_axis_line;

    //right
    for (float i = 0; i < canvas_max_x; i += dist)
    {

        //Выключить отрисовку за рамкой
        if ((i+scrool_x) > myWidth/2*zoom - frame_size_x)
            continue;
        if ((i+scrool_x) < -myWidth/2*zoom + frame_size_x)
            continue;

        if ((int)i % 50 == 0)
        {
            painter->drawText(i + scrool_x,
                             -5 -size - zoom*myHeight/2 + frame_size_y,
                             QString("%1").arg(round(i/zoom*10)/10.0f)
                             );
            painter->setPen(QPen(Qt::black, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::black, 1));
        }

        painter->drawLine(i + scrool_x,
                          size - zoom*myHeight/2 + frame_size_y,
                          i + scrool_x,
                          -size - zoom*myHeight/2 + frame_size_y
                          );

    }

    for (float i = 0; i > canvas_min_x; i -= dist)
    {
        //Выключить отрисовку за рамкой
        if ((i+scrool_x) < -myWidth/2*zoom + frame_size_x)
            continue;
        if ((i+scrool_x) > myWidth/2*zoom - frame_size_x)
            continue;

        if ((int)-i % 50 == 0)
        {
            painter->drawText(i + scrool_x,
                             -5-size - zoom*myHeight/2 + frame_size_y,
                             QString("%1").arg(round(i/zoom*10)/10.0f)
                             );
            painter->setPen(QPen(Qt::black, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::black, 1));
        }

        //left
        painter->drawLine(i + scrool_x,
                          size - zoom*myHeight/2 + frame_size_y,
                          i + scrool_x,
                          -size - zoom*myHeight/2 + frame_size_y
                          );
    }
}

void MainWindow::markup_axis_Y2(QPainter *painter)
{
    QFont font = painter->font();
    font.setBold(QFont::Bold);
    painter->setFont(font);

    painter->setPen(QPen(Qt::black, 1));

    float dist = define_dist_axis( ui->horizontalSlider->value() );
    float size = size_markup_axis_line;

    //right
    for (float i = dist; i < canvas_max_y; i += dist)
    {
        if ((int)i % 50 == 0)
        {
            painter->drawText(size + scrool_x,
                              i + + scrool_y,
                              QString("%1").arg(round(i/zoom*10)/10.0f)
                             );
            painter->setPen(QPen(Qt::black, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::black, 1));
        }

        painter->drawLine(-size + scrool_x,
                          i + scrool_y,
                          size + scrool_x,
                          i + scrool_y
                          );

    }

    //left
    for (float i = -dist; i > canvas_min_y; i -= dist)
    {
        if ((int)-i % 50 == 0)
        {
            painter->drawText(size + scrool_x,
                              i + + scrool_y,
                              QString("%1").arg(round(i/zoom*10)/10.0f)
                             );
            painter->setPen(QPen(Qt::black, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::black, 1));
        }

        painter->drawLine(-size + scrool_x,
                          i + scrool_y,
                          size + scrool_x,
                          i + scrool_y
                          );
    }

}
void MainWindow::markup_axis_Y2_left(QPainter *painter)
{
    QFont font = painter->font();
    font.setBold(QFont::Bold);
    painter->setFont(font);

    painter->setPen(QPen(Qt::black, 1));

    float dist = define_dist_axis( ui->horizontalSlider->value() );
    float size = size_markup_axis_line;

    //down
    for (float i = dist; i < canvas_max_y; i += dist)
    {
        //Выключить отрисовку за рамкой
        if ((i+scrool_y) > myHeight/2*zoom - frame_size_y - 30)
            continue;
        if ((i+scrool_y) < -myHeight/2*zoom + frame_size_y)
            continue;

        if ((int)i % 50 == 0)
        {
            QString str_num = QString("%1").arg(round(i/zoom*10)/10.0f);
            painter->drawText(-5.75*(std::size(str_num))-12-size - zoom*myWidth/2+ frame_size_x,
                              i + scrool_y,
                              QString("%1").arg(round(i/zoom*10)/10.0f)
                             );
            painter->setPen(QPen(Qt::black, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::black, 1));
        }

        painter->drawLine(-size - zoom*myWidth/2+ frame_size_x,
                          i + scrool_y,
                          size - zoom*myWidth/2 + frame_size_x,
                          i + scrool_y
                          );

    }
    painter->setPen(QPen(Qt::black, 1));

    //up
    for (float i = dist; i > canvas_min_y; i -= dist)
    {
        //Выключить отрисовку за рамкой
        if ((i+scrool_y) < -myHeight/2*zoom + frame_size_y)
            continue;
        if ((i+scrool_y) > myHeight/2*zoom - frame_size_y - 30)
            continue;

        if ((int)-i % 50 == 0)
        {
            QString str_num = QString("%1").arg(round(i/zoom*10)/10.0f);
            painter->drawText(-5.75*(std::size(str_num))-12-size - zoom*myWidth/2+ frame_size_x,
                              i + scrool_y,
                              QString("%1").arg(round(i/zoom*10)/10.0f)
                             );
            painter->setPen(QPen(Qt::black, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::black, 1));
        }


        painter->drawLine(-size - zoom*myWidth/2+ frame_size_x,
                          i + scrool_y,
                          size - zoom*myWidth/2 + frame_size_x,
                          i + scrool_y
                          );
    }

}
void MainWindow::markup_axis_Y2_right(QPainter *painter)
{
    QFont font = painter->font();
    font.setBold(QFont::Bold);
    painter->setFont(font);

    painter->setPen(QPen(Qt::black, 1));

    float dist = define_dist_axis( ui->horizontalSlider->value() );
    float size = size_markup_axis_line;

    //down
    for (float i = dist; i < canvas_max_y; i += dist)
    {
        //Выключить отрисовку за рамкой
        if ((i+scrool_y) > myHeight/2*zoom - frame_size_y - 30)
            continue;
        if ((i+scrool_y) < -myHeight/2*zoom + frame_size_y)
            continue;

        if ((int)i % 50 == 0)
        {
            painter->drawText(12-size + zoom*myWidth/2 - frame_size_x,
                              i + scrool_y,
                              QString("%1").arg(round(i/zoom*10)/10.0f)
                             );
            painter->setPen(QPen(Qt::black, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::black, 1));
        }

        painter->drawLine(-size + zoom*myWidth/2 - frame_size_x,
                          i + scrool_y,
                          size + zoom*myWidth/2 - frame_size_x,
                          i + scrool_y
                          );

    }
    painter->setPen(QPen(Qt::black, 1));

    //up
    for (float i = dist; i > canvas_min_y; i -= dist)
    {
        //Выключить отрисовку за рамкой
        if ((i+scrool_y) < -myHeight/2*zoom + frame_size_y)
            continue;
        if ((i+scrool_y) > myHeight/2*zoom - frame_size_y)
            continue;

        if ((int)-i % 50 == 0)
        {
            painter->drawText(12-size + zoom*myWidth/2 - frame_size_x,
                              i + scrool_y,
                              QString("%1").arg(round(i/zoom*10)/10.0f)
                             );
            painter->setPen(QPen(Qt::black, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::black, 1));
        }

        painter->drawLine(-size + zoom*myWidth/2 - frame_size_x,
                          i + scrool_y,
                          size + zoom*myWidth/2 - frame_size_x,
                          i + scrool_y
                          );
    }

}


//Событие: Нажатие кнопки мыши
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mouse_before_press_X = this->mapFromGlobal(QCursor::pos()).x();
    mouse_before_press_Y = this->mapFromGlobal(QCursor::pos()).y();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    collision = false;
    repaint();
    mouse_before_press_X = this->mapFromGlobal(QCursor::pos()).x();
    mouse_before_press_Y = this->mapFromGlobal(QCursor::pos()).y();

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    //Глобальные координаты мыши
    mousePos = this->mapFromGlobal(QCursor::pos());

    //Локальные координаты мыши
    mouseX = mousePos.x() / zoom - (myWidth/2 + punding_x_left / zoom + scrool_x / zoom);
    mouseY = (-mousePos.y() / zoom + myHeight/2 + scrool_y / zoom);

    QString mousePosToStr1 = QString("Answer is (x=%1, y=%2)").arg(mouseX).arg(mouseY);

    collision = false;
    //Проверка пересечний мышки с графиком
    for (int i = 0; i < std::size(vecGraph); i++)
    {
        if (vecGraph[i].check_collision2(mouseX, mouseY, ans_point, zoom))
            collision = true;
    }



    //Формируем ответ для statusBar
    QString mousePosToStr = QString("Выбранная точка (x=%1, y=%2)").arg(ans_point.x()).arg(ans_point.y());
    if (collision)
    {
        //Вывод ответа в statusBar
        ui->statusbar->showMessage(mousePosToStr);


        mouse_before_press_X = this->mapFromGlobal(QCursor::pos()).x();
        mouse_before_press_Y = this->mapFromGlobal(QCursor::pos()).y();

        //Рисуем точку
        repaint();
    }
    else if (moveSroll_active)
    {

        float mouse_update_X = mouse_before_press_X - (this->mapFromGlobal(QCursor::pos()).x());
        float mouse_update_Y = mouse_before_press_Y - (this->mapFromGlobal(QCursor::pos()).y());

        mouse_before_press_X = this->mapFromGlobal(QCursor::pos()).x();
        mouse_before_press_Y = this->mapFromGlobal(QCursor::pos()).y();

        float x = ui->horizontalScrollBar->value();
        ui->horizontalScrollBar->setValue(x + scroll_k*mouse_update_X/zoom);

        float y = ui->verticalScrollBar->value();
        ui->verticalScrollBar->setValue(y + scroll_k*mouse_update_Y/zoom);
        repaint();
    }

}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{

    //Глобальные координаты мыши
    mousePos = this->mapFromGlobal(QCursor::pos());

    //Локальные координаты мыши
    mouseX = mousePos.x() / zoom - (myWidth/2 + punding_x_left / zoom + scrool_x / zoom);
    mouseY = (-mousePos.y() / zoom + myHeight/2 + scrool_y / zoom);

    QString mousePosToStr1 = QString("Answer is (x=%1, y=%2)").arg(mouseX).arg(mouseY);

    collision = false;
    int index_graph = -1;
    //Проверка пересечний мышки с графиком
    for (int i = 0; i < std::size(vecGraph); i++)
    {
        if ((vecGraph[i].check_collision2(mouseX, mouseY, ans_point, zoom)) && !vecGraph[i].get_select())
        {
            index_graph = i;
        }
        vecGraph[i].set_select(false);

    }

    for(int i = 0; i < ui->listWidget->count(); i++){
        QListWidgetItem* cur = ui->listWidget->item(i);
        QIcon icon = cur->icon();

        QPixmap *p = new QPixmap;
        *p = icon.pixmap(50,50);
        QImage *img = new QImage;
        *img = p->toImage();
        QRgb b = img->pixel(1,1);
        QColor *c = new QColor;
        c->setRgb(b);
        cur->setSelected(false);
        if (i == index_graph)
            cur->setSelected(true);
    }

    if (index_graph != -1)
    {
        vecGraph[index_graph].set_select(true);
        repaint();
    }


}


void MainWindow::mini_map_draw(QPainter &painter)
{
    float width_RP = ui->groupBox_right->width();
    float height_RP = ui->verticalSpacer_miniMap->geometry().height();
    float spaceXY = 10;


    //Рамка
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(width()/2 - width_RP + spaceXY, height()/2 - height_RP - spaceXY, width_RP - 2*spaceXY, height_RP - 2*spaceXY);

    float centreW = (width()/2 - width_RP/2);
    float centreH = (height()/2 - height_RP/2 - 2*spaceXY);
    painter.translate(centreW, centreH);

    width_RP -= spaceXY*2;
    height_RP -= spaceXY*2;

    //Оси
    painter.setPen(QPen(Qt::black, 1));
    painter.drawLine(-width_RP/2, 0, width_RP/2, 0);
    painter.drawLine(0, -height_RP/2, 0, height_RP/2);


    //Находим крайние X графиков
    float maxX = -9999999;
    for (int i = 0; i < std::size(vecGraph); i++)
    {
        float new_maxX = abs(vecGraph[i].get_minX());
        if (new_maxX > maxX)
            maxX = new_maxX;

        new_maxX = abs(vecGraph[i].get_maxX());
        if (new_maxX > maxX)
            maxX = new_maxX;
    }

    //Находим крайние Y графиков
    float maxY = -99999;
    for (int i = 0; i < std::size(vecGraph); i++)
    {
        float new_maxY = abs(vecGraph[i].get_minY());
        if (new_maxY > maxY)
            maxY = new_maxY;

        new_maxY = abs(vecGraph[i].get_maxY());
        if (new_maxY > maxY)
            maxY = new_maxY;
    }

    float zoomY = height_RP / ((maxY)*2);
    float zoomX = width_RP / ((maxX)*2);

    float zoom_mm = zoomX;
    if (zoomY < zoom_mm)
        zoom_mm = zoomY;


    for (int j = 0; j < std::size(vecGraph); j++)
       {
           for (int i = 0; i < std::size(vecGraph[j].arr_points) - 1; i++)
           {
               QLineF line(
                           (vecGraph[j].arr_points[i].x()) * zoom_mm,
                           (-vecGraph[j].arr_points[i].y()) * zoom_mm,
                           (vecGraph[j].arr_points[i+1].x()) * zoom_mm,
                           (-vecGraph[j].arr_points[i+1].y()) * zoom_mm
                           );

               QPen myPen(vecGraph[j].colorGraph, 1, Qt::SolidLine, Qt::SquareCap);
               painter.setPen(myPen);
               //Рисуем выбранные графики
               if (vecGraph[j].show)
                   painter.drawLine(line);
           }
       }


    painter.setPen(QPen(Qt::red, 1));
    float xx2 = (myWidth/2 - frame_size_x - scrool_x/zoom) * zoom_mm;
    float xx1 = (-myWidth/2 + frame_size_x - scrool_x/zoom) * zoom_mm;
    float yy1 = (myHeight/2 - frame_size_y - scrool_y/zoom) * zoom_mm;
    float yy2 = (-myHeight/2 + frame_size_y - scrool_y/zoom) * zoom_mm;

    if (xx1 < -width_RP/2)
        xx1 = -width_RP/2;
    if (xx2 > width_RP/2)
        xx2 = width_RP/2;


    if (yy2 < -height_RP/2)
        yy2 = -height_RP/2;
    if (yy1 > height_RP/2)
        yy1 = height_RP/2;

    painter.drawRect(xx1, yy2, abs(xx1-xx2), abs(yy1-yy2));
}





//Событие: Перерисовка (При изменении окна + специальный вызвов repaint)
void MainWindow::paintEvent(QPaintEvent *event)
{
    //Отрисовщик
    QPainter painter(this);

    //Отсутуп от groupBox (левый)
    punding_x_left = ui->groupBox->width();
    punding_x_right = ui->groupBox_right->width();

    punding_x = punding_x_left - punding_x_right;

    //Центр холста
    float centreW = ((width() + punding_x) / 2);
    float centreH = (height() / 2);

    //Размеры холста
    myWidth = (width() - (punding_x_left + punding_x_right)) / zoom;
    myHeight = height() / zoom;

    //Перемещаем точку центра холста
    painter.translate(centreW, centreH);




    //Обновление sroll
    change_horizontalScrollBar();
    change_verticalScrollBar();

    //Оси вышли за рамки экрана
    bool axis_X_up_show = scrool_y < -zoom*myHeight/2 + frame_size_y;
    bool axis_X_down_show = scrool_y > zoom*myHeight/2 - frame_size_y - 30;
    bool axis_Y_left_show = scrool_x < -zoom*myWidth/2 + frame_size_x;
    bool axis_Y_right_show = scrool_x > zoom*myWidth/2 - frame_size_x - 28;


    //Отрисвка осей
    painter.setPen(Qt::black);
    painter.drawLine(
                     (myWidth/-2 * zoom),
                     (0 * zoom + scrool_y),
                     (myWidth/2 * zoom ),
                     (0 * zoom + scrool_y)
                     );

    painter.drawLine(
                    (0 * zoom + scrool_x),
                    (-myHeight * zoom),
                    (0 * zoom + scrool_x),
                    (myHeight * zoom)
                    );

    //Разметка осей
    if (!axis_X_up_show && !axis_X_down_show)
        markup_axis_X2(&painter);
    if (!axis_Y_left_show && !axis_Y_right_show)
        markup_axis_Y2(&painter);

    painter.setPen(Qt::blue);
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

               QPen myPen;
               if (vecGraph[j].get_select())
                   myPen = QPen(Qt::red, 3, Qt::SolidLine, Qt::SquareCap);
               else
                   myPen = QPen(vecGraph[j].colorGraph, 2, Qt::SolidLine, Qt::SquareCap);

               painter.setPen(myPen);
               //Рисуем выбранные графики
               if (vecGraph[j].show)
                   painter.drawLine(line);
           }
       }




    //Отрисовка точки касания
    painter.setPen(Qt::red);
    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
    if (collision)
    {
        //Линии для выделения координат у точки пересечения
        painter.setPen(QPen(Qt::red, 1, Qt::DashLine, Qt::SquareCap));
        painter.drawLine((ans_point.x() * zoom + scrool_x), (-ans_point.y() * zoom + scrool_y), scrool_x, (-ans_point.y() * zoom + scrool_y));
        painter.drawLine((ans_point.x() * zoom + scrool_x), (-ans_point.y() * zoom + scrool_y), (ans_point.x() * zoom + scrool_x), scrool_y);
        //Отрисовка точки касания
        painter.drawEllipse((ans_point.x() * zoom + scrool_x) - 3, (-ans_point.y() * zoom + scrool_y) - 3, 6, 6);
    }


    //Подпись осей
    QPen myPen(Qt::black, 1, Qt::SolidLine, Qt::SquareCap);
    painter.setPen(myPen);

    QFont font = painter.font();
    font.setBold(QFont::Bold);
    painter.setFont(font);

    painter.drawText(QPoint((myWidth*zoom/2) - frame_size_x - (std::size(titleAxisX) * 5.75) - 20, -15 + scrool_y), titleAxisX);
    painter.drawText(QPoint(40 + scrool_x,(-myHeight*zoom/2) + 25 + frame_size_y), titleAxisY);
    //painter.drawText(point, "Y");


    //Создание рамки
    painter.setPen(Qt::white);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    //painter.drawRect(-myWidth/2 - punding_x_left, myHeight/2, width(), myHeight/2);

    painter.drawRect(-zoom*myWidth/2, -zoom*myHeight/2 , width(), frame_size_y);
    painter.drawRect(-zoom*myWidth/2, zoom*myHeight/2 - frame_size_y -30, width(), frame_size_y + 30);

    painter.drawRect(-zoom*myWidth/2, -zoom*myHeight/2, frame_size_x, height());
    painter.drawRect(zoom*myWidth/2 - frame_size_x, -zoom*myHeight/2, width(), height());

    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.drawRect(-zoom*myWidth/2 + frame_size_x, -zoom*myHeight/2 + frame_size_y, zoom*myWidth - frame_size_x*2, zoom*myHeight - frame_size_y*2 - 30);
    //painter.drawRect(0, 0, 100, 100);

    //Разметка осей
    if (axis_X_down_show)
        markup_axis_X2_down(&painter);
    if (axis_X_up_show)
        markup_axis_X2_up(&painter);

    if (axis_Y_left_show)
        markup_axis_Y2_left(&painter);
    if (axis_Y_right_show)
    markup_axis_Y2_right(&painter);
    //markup_axis_Y2(&painter);


    //Мини_Карта
    mini_map_draw(painter);

}

void MainWindow::on_horizontalScrollBar_valueChanged(int value)
{
    scrool_x = -value/scroll_k * zoom;
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
    scrool_y = -value/scroll_k * zoom;
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

//

void MainWindow::on_pushButton_clicked()
{
    DirectWindow dialog;
    dialog.setModal(true);
    dialog.exec();
    filename = dialog.sendTitle();
}

void MainWindow::on_pushButton_3_clicked()
{
    if (ui->lineEdit_2->text() != "") titleAxisX = ui->lineEdit_2->text();
    if (ui->lineEdit_3->text() != "") titleAxisY = ui->lineEdit_3->text();
    repaint();
}


void MainWindow::on_horizontalSlider_2_sliderMoved(int position)
{
    union color {
        int integer;
        unsigned char byte[3];
    };
    color n;
    n.integer = position;
    int r = n.byte[2], g = n.byte[1], b = n.byte[0];
    ui->widget->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b));
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

// кнопка построение графика
void MainWindow::on_pushButton_2_clicked()
{
    QVector<QPointF> temp;

    if (ui->radioButton->isChecked()) {
        double x, y;
        const int N = 100;
        int h = ui->centralwidget->geometry().height();
        int w = ui->centralwidget->geometry().width();
        for (int i = 0; i < N; i++) {
            x = rand() % w - w/2;
            y = rand() % h - h/2;
            temp.push_back(QPointF(x, y));
        }
    }

    if (ui->radioButton_2->isChecked()) {
        if (!readFile(filename.toStdString(), temp)){
            QMessageBox::warning(this, "Ошибка", "Файл не выбран!", QMessageBox::Ok);
            return;
        }
    }


    //цвет
    QColor color = ui->widget->palette().window().color();
    //имя
    QString name = ui->lineEdit->text();

    //Валидность имени
    if (name == "")
        QMessageBox::warning(this, "Ошибка", "Имя графика не введено!", QMessageBox::Ok);
    else{
        //Проверка на существующее имя
        bool repeat = false;
        for (auto &i : vecGraph){
            if (i.nameGraph == name)
                repeat = true;
        }
        if (!repeat){
            //Дополнение легенды
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            QWidget* wgt = new QWidget;
            QLayout* l = new QHBoxLayout;

            //Имя
            QLabel* label = new QLabel;
            label->setText(name);

            //Кнопка
            QPushButton* btn = new QPushButton("");
            btn->setFixedSize(20, 20);
            btn->setFlat(true);
            btn->setAutoFillBackground(true);
            QPalette pal = btn->palette();
            pal.setColor(QPalette::Button, color);
            btn->setPalette(pal);

            //Чекбокс
            QCheckBox *box = new QCheckBox();
            box->setCheckState(Qt::Checked);

            connect(btn, SIGNAL(clicked()), SLOT(onBtnClicked()));
            connect(box, SIGNAL(clicked()), SLOT(onCheckBoxClicked()));

            l->addWidget(box);
            l->addWidget(btn);
            l->addWidget(label);
            l->setAlignment(Qt::AlignLeft);

            wgt->setLayout(l);
            item->setSizeHint(wgt->sizeHint());

            ui->listWidget->setItemWidget(item, wgt);

            MyGraph tempgr(temp, name, color);
            vecGraph.push_back(tempgr);
        }
        else{
            QMessageBox::warning(this, "Ошибка", "Такое имя уже используется!\nВыберите другое название для графика.", QMessageBox::Ok);
        }
    }
    filename = "";
    update();
}

//Смена цвета (кнопка)
void MainWindow::onBtnClicked() {
    if( QPushButton* btn = qobject_cast< QPushButton* >(sender())) {
        if( QLabel* e = btn->parent()->findChild< QLabel* >()) {
            Dialog dialog;
            dialog.setModal(true);

            //Меняем цвет, если нажата кнопка "ОК"
            if (dialog.exec () == QDialog :: Accepted){
                QColor newColor = dialog.sendColor();
                //Меняем цвет графика
                for (auto &i : vecGraph){
                    if (i.nameGraph == e->text()){
                        i.colorGraph = newColor;
                    }
                }
                btn->setFlat(true);
                btn->setAutoFillBackground(true);
                QPalette pal = btn->palette();
                pal.setColor(QPalette::Button, newColor);
                btn->setPalette(pal);
            }
            return;
         }
    }
}

//Показать/убрать график
void MainWindow::onCheckBoxClicked(){
    if( QCheckBox* box = qobject_cast< QCheckBox* >(sender())) {
        if( QLabel* e = box->parent()->findChild< QLabel* >()) {
            if (box->checkState() == Qt::Checked){
                //Активация графика
                for (auto &i : vecGraph){
                    if (i.nameGraph == e->text())
                        i.show = true;
                }
            }
            if (box->checkState() == Qt::Unchecked){
                //Деактивация графика
                for (auto &i : vecGraph){
                    if (i.nameGraph == e->text())
                        i.show = false;
                }
            }
            update();
         }
    }
}


//Кнопка удаление графика
void MainWindow::on_pushButton_4_clicked()
{
    if (vecGraph.size() != 0) {
        vecGraph.clear();
        collision = false;
        ui->listWidget->clear();
        update();
    }
    else {
        QMessageBox::warning(this, "Ошибка", "Нет нарисованных графиков!", QMessageBox::Ok);
    }
}


void MainWindow::on_radioButton_clicked() // выбрали рандомный график
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
}


void MainWindow::on_radioButton_2_clicked()//выбрали файл
{
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
}

//Выделение графика
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    for(int i = 0; i < ui->listWidget->count(); i++){

        QListWidgetItem* cur = ui->listWidget->item(i);
        if (cur->isSelected()){
            if (vecGraph[i].get_select())
            {
                vecGraph[i].set_select(false);
                cur->setSelected(false);
            }
            else
                vecGraph[i].set_select(true);
        }
        if (!cur->isSelected()){
            vecGraph[i].set_select(false);
        }
    }
    update();
}


