#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_dialvoltage_valueChanged(int value);

    void on_dialampere_valueChanged(int value);

    void isvalue();

    void on_btnopen_clicked();

    void on_btnclose_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
