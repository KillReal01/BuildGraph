#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->horizontalSlider->setMaximum(pow(256, 3) - 1);
    ui->widget->setStyleSheet(QString("background-color: rgb(0, 0, 0);"));
    setWindowTitle("Выбор цвета");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_horizontalSlider_sliderMoved(int position)
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

QColor Dialog::sendColor(){
    return ui->widget->palette().window().color();
}

