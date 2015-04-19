#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

//#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void saveSettings( QString * filename );
    void loadSettings( QString * filename );

private slots:
    void on_pushButton_start_clicked();

    void on_pushButton_browse_clicked();

    void process_ended();

    void readStdOutput();
    void readStdError();

    void on_pushButton_2_clicked();

//    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_updateSerial_clicked();

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

private:

    Ui::MainWindow *ui;

    bool processStarted;
    QProcess *myProcess;
    QList<QSerialPortInfo> *filteredPorts;
};

#endif // MAINWINDOW_H
