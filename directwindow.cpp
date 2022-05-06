#include "directwindow.h"
#include "ui_directwindow.h"
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

DirectWindow::DirectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DirectWindow)
{
    ui->setupUi(this);
    ShowCont(this->currpath);
}

DirectWindow::~DirectWindow()
{
    delete ui;
}

void DirectWindow::ShowCont(QString path) {
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); i++) {
        QFileInfo fileinfo = list.at(i);
        new QListWidgetItem(fileinfo.fileName(), ui->listWidget);
    }
}

QString DirectWindow::sendTitle()
{
    return this->currpath;
}

void DirectWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->listWidget->clear();

    if (item->text() == ".")
    {
        ShowCont("C:");
        this->currpath = "C:";
    }
    else if (item->text() == "..")
    {
        if (this->currpath.length() != 2)
        {
            int index = this->currpath.lastIndexOf("/");
            this->currpath = this->currpath.remove(index, this->currpath.length() - index);
            ShowCont(this->currpath);
        }
        else ShowCont("C:");
    }
    else if (item->text().right(4) == (QString)".txt")
    {
        this->currpath = this->currpath + "/" + item->text();
        this->close();
        QMessageBox::information(this, "Message", "File read succesful!", QMessageBox::Ok);
    }
    else
    {
        this->currpath = this->currpath + "/" + item->text();
        ShowCont(this->currpath);
    }
    repaint();
}

void DirectWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(20, 20, this->currpath);
    //painter.drawText(200, 20, this->currpath.right(3));
    //painter.drawText(250, 20, QFileInfo(this->currpath).filePath());
}
