#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

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
    void on_pushButton_start_clicked();

    void on_pushButton_browse_clicked();

    void process_ended();

    void readStdOutput();
    void readStdError();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    QProcess *myProcess;
};

#endif // MAINWINDOW_H
