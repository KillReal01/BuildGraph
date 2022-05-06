#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <fstream>
#include <map>
#include <time.h>

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

    bool readFile(std::string filename, std::map<double, double>& points);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    Ui::MainWindow *ui;
    QString filename = "";
    QPainterPath path;
};
#endif // MAINWINDOW_H

