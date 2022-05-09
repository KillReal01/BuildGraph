#include "mainwindow.h"
#include "directwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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
    minX -= 100;
    maxX += 150;

    //Растягиваем скролл
    if (minX  + myWidth / 2 < 0)
        ui->horizontalScrollBar->setMinimum(minX  + myWidth / 2);
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
        ui->verticalScrollBar->setMaximum(-(minY  + myHeight / 2));
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




//Событие: Нажатие кнопки мыши
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //Глобальные координаты мыши
    mousePos = this->mapFromGlobal(QCursor::pos());

    //Локальные координаты мыши
    mouseX = mousePos.x() / zoom - (myWidth/2 + punding_x_left / zoom + scrool_x / zoom);
    mouseY = (-mousePos.y() / zoom + myHeight/2 + scrool_y / zoom);

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
        //Вывод ответа в statusBar
        ui->statusbar->showMessage(mousePosToStr);
        //Рисуем точку
        repaint();
    }


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

            QPen myPen(vecGraph[j].colorGraph, 2, Qt::SolidLine, Qt::SquareCap);
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
        painter.drawEllipse((ans_point.x() * zoom + scrool_x) - 3, (-ans_point.y() * zoom + scrool_y) - 3, 6, 6);
    }


    //Подпись осей
    QPen myPen(Qt::black, 1, Qt::SolidLine, Qt::SquareCap);
    painter.setPen(myPen);

    QFont font = painter.font();
    font.setBold(QFont::Bold);
    painter.setFont(font);

    painter.drawText(QPoint((myWidth*zoom/2) - frame_size - (std::size(titleAxisX) * 5.75) - 40, -5 + scrool_y), titleAxisX);
    painter.drawText(QPoint(5 + scrool_x,(-myHeight*zoom/2) + 25 + frame_size), titleAxisY);
    //painter.drawText(point, "Y");


    //Создание рамки
    painter.setPen(Qt::white);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    //painter.drawRect(-myWidth/2 - punding_x_left, myHeight/2, width(), myHeight/2);

    painter.drawRect(-zoom*myWidth/2, -zoom*myHeight/2 , width(), frame_size);
    painter.drawRect(-zoom*myWidth/2, zoom*myHeight/2 - frame_size -30, width(), frame_size + 30);
    painter.drawRect(-zoom*myWidth/2, -zoom*myHeight/2, frame_size, height());
    painter.drawRect(zoom*myWidth/2 - frame_size - 25, -zoom*myHeight/2, frame_size + 25, height());


    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.drawRect(-zoom*myWidth/2 + frame_size, -zoom*myHeight/2 + frame_size, zoom*myWidth - frame_size*2 - 25, zoom*myHeight - frame_size*2 - 30);
    //painter.drawRect(0, 0, 100, 100);

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
    //qDebug() << value;
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
        readFile(filename.toStdString(), temp);
    }

    for (auto &i : temp) qDebug() << i.x() << " " << i.y();

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
            QListWidgetItem* item = new QListWidgetItem;

            item->setText(name);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
            item->setCheckState(Qt::Checked); // AND initialize check state

            //Цвет графика
            QPixmap pixmap (50, 50);
            pixmap.fill(color);
            QIcon icon;
            icon.addPixmap(pixmap, QIcon::Normal, QIcon::On);
            item->setIcon(icon);

            ui->listWidget->addItem(item);

            MyGraph tempgr(temp, name, color);
            vecGraph.push_back(tempgr);
        }
        else{
            QMessageBox::warning(this, "Ошибка", "Такое имя уже используется!\nВыберите другое название для графика.", QMessageBox::Ok);
        }
    }

    update();
}

void MainWindow::on_pushButton_4_clicked() // кнопка удаление графика
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
}


void MainWindow::on_radioButton_2_clicked()
{
    ui->pushButton->setEnabled(true);
}

//Hide/show график
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if (item->checkState() == Qt::Checked){
        //Активация графика
        for (auto &i : vecGraph){
            if (i.nameGraph == item->text())
                i.show = true;
        }
    }
    if (item->checkState() == Qt::Unchecked){
        //Деактивация графика
        for (auto &i : vecGraph){
            if (i.nameGraph == item->text())
                i.show = false;
        }
    }
    update();
}

//Меняем цвет графика
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    Dialog dialog;
    dialog.setModal(true);
    dialog.exec();
    QColor newColor = dialog.sendColor();

    //Меняем цвет графика
    for (auto &i : vecGraph){
        if (i.nameGraph == item->text()){
            i.colorGraph = newColor;
        }
    }

    //Меняем цвет иконки
    QPixmap pixmap (50, 50);
    pixmap.fill(newColor);
    QIcon icon;
    icon.addPixmap(pixmap, QIcon::Normal, QIcon::On);
    item->setIcon(icon);
}

