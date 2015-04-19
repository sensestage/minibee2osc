#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
//    QCoreApplication::setOrganizationName("SenseStage");
//    QCoreApplication::setOrganizationDomain("sensestage.eu");
//    QCoreApplication::setApplicationName("minibee2osc");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
