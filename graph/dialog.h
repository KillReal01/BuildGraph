#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QWidget>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    QColor sendColor();

private slots:
    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
