#ifndef DIRECTWINDOW_H
#define DIRECTWINDOW_H

#include "qlistwidget.h"
#include <QDialog>
#include <QWidget>
#include <QPainter>

namespace Ui {
class DirectWindow;
}

class DirectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DirectWindow(QWidget *parent = nullptr);
    ~DirectWindow();
    void ShowCont(QString path);
    QString sendTitle();

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::DirectWindow *ui;
    QString currpath = "C:/";

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DIRECTWINDOW_H
