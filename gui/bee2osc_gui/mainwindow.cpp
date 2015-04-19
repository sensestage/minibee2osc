#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <unistd.h>

#include <QDebug>

#include <QFileDialog>
#include <QClipboard>

#include <QSettings>

QList<QSerialPortInfo>* filterPorts(QStringList mask) {
//    qDebug() << "searching for ports with mask" << mask;
    QList<QSerialPortInfo> * result = new QList<QSerialPortInfo>;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        foreach (const QString& maskItem, mask) {
            //qDebug() << "looking for:" << maskItem;
            if (info.portName().contains(QRegExp(maskItem))) {
//                qDebug() << "found " << maskItem;
                result->append(info);
                break;
            }
        }
    }
    return result;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myProcess = NULL;
    filteredPorts = NULL;
    processStarted = false;

    on_pushButton_updateSerial_clicked();
    loadSettings( NULL );
}

MainWindow::~MainWindow()
{
    saveSettings( NULL );
    if ( myProcess != NULL){
        myProcess->terminate();
    }
    delete ui;
}

void MainWindow::on_pushButton_start_clicked()
{
//    bool started = false;
//    if ( myProcess != NULL ){
//        int state = myProcess->state();
//        if ( state == QProcess::NotRunning ){
//            started = false;
//        } else {
//            started = true;
//        }
//    }
    if ( !processStarted ){
        QString configfile = ui->lineEdit_configfile->text();
//        QString serialport = ui->comboBox_serial->currentText();
        int selectedPort = ui->comboBox_serial->currentIndex();
        QString serialport = filteredPorts->at( selectedPort ).systemLocation();
        QString targethost = ui->lineEdit_sendOSC->text();
        QString targethostport = ui->lineEdit_sendOSCport->text();
        QString myport = ui->lineEdit_listenOSCport->text();
        int verbosity = ui->spinBox_loglevel->value();

        ui->groupBox_configuration->setVisible( false );
        ui->groupBox_listenosc->setVisible(false );
        ui->groupBox_sendosc->setVisible(false);
        ui->groupBox_serialport->setVisible(false);
        ui->groupBox_verbosity->setVisible(false);
        ui->groupBox->setVisible(false);

        QString program = "minibee2osc";
        QStringList arguments;
        arguments << "-c" << configfile << "-s" << serialport << "-i" << targethost << "-p" << targethostport << "-l" << myport << "-v" << QString::number(verbosity);

        myProcess = new QProcess(this);
        myProcess->start(program, arguments);

        ui->pushButton_start->setText("stop");

        QObject::connect(myProcess, SIGNAL(finished(int , QProcess::ExitStatus )), this, SLOT(process_ended()) );
        QObject::connect(myProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdOutput()) );
        QObject::connect(myProcess, SIGNAL(readyReadStandardError()), this, SLOT(readStdError()) );
        processStarted = true;
    } else {
//        myProcess->write( "quit\n" ); //FIXME: std::cin blocks, so this is not useable
//        usleep( 2000 );
        myProcess->terminate();
//        process_ended();
    }
}

void MainWindow::readStdOutput()
{
    ui->text_output->setTextColor( QColor( "black" ) );
    ui->text_output->append( myProcess->readAllStandardOutput() );

}

void MainWindow::readStdError()
{
    ui->text_output->setTextColor( QColor( "red" ) );

    QString tmp = QObject::trUtf8(myProcess->readAllStandardError());
    tmp.replace("\033[0m","");
    tmp.replace("\033[32m","");
    tmp.replace("\033[33m","");
    tmp.replace("\033[35m","");
    tmp.replace("\033[36m","");
    tmp.replace("\033[90m","");
    tmp.replace("\007","");

    ui->text_output->append( tmp );

}

void MainWindow::process_ended()
{
    processStarted = false;

    QObject::disconnect(myProcess, SIGNAL(finished(int , QProcess::ExitStatus )), this, SLOT(process_ended()) );
    QObject::disconnect(myProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdOutput()) );
    QObject::disconnect(myProcess, SIGNAL(readyReadStandardError()), this, SLOT(readStdError()) );
    ui->pushButton_start->setText("start");
    ui->groupBox_configuration->setVisible( true );
    ui->groupBox_listenosc->setVisible(true );
    ui->groupBox_sendosc->setVisible(true);
    ui->groupBox_serialport->setVisible(true);
    ui->groupBox_verbosity->setVisible(true);
    ui->groupBox->setVisible(true);
}

void MainWindow::on_pushButton_browse_clicked()
{
    QString selfilter = tr("Minibee2osc Configuration Files (*.xml)");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
               tr("Minibee2osc Configuration Files (*.xml);; All files (*.*)"),&selfilter);

       if (!fileName.isEmpty()) {
           ui->lineEdit_configfile->setText( fileName );
       }
}

void MainWindow::on_pushButton_2_clicked()
{
    // clear contents of text window
    ui->text_output->clear();
}

//void MainWindow::on_pushButton_clicked()
//{

////// not working yet...

//////    // Clear first, because the QTextEdit::copy() command doesn't do this
//////    // if nothing is selected.
//////    QApplication::clipboard()->clear (QClipboard::Clipboard);

//////    // copy contents of text window
//////    ui->text_output->copy();

//////    const QString clipSel2 = QApplication::clipboard()->text (QClipboard::Clipboard);
//}

void MainWindow::on_pushButton_3_clicked()
{
    // save contents of text window
    QString contents = ui->text_output->toPlainText();
    QString selfilter = tr("Log text files (*.log.txt)");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), QString(),
               tr("Log text files (*.log.txt)"), &selfilter);

       if (!fileName.isEmpty()) {
           QFile file( fileName );
               if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                   return;

               QTextStream out(&file);
               out << contents;
               file.close();
               ui->text_output->setTextColor( QColor( "blue" ) );
               ui->text_output->append( "\nSaved output contents to: " + fileName );
       }

}

void MainWindow::on_pushButton_updateSerial_clicked()
{
       QStringList mask;
       mask << "ttyUSB" << "ACM" << "AMA" << "tty.usb" << "COM";

       if ( filteredPorts != NULL){
           delete filteredPorts;
       }
//       QList<QSerialPortInfo> filteredPorts( filterPorts(mask) );
       filteredPorts = filterPorts(mask);

       ui->comboBox_serial->clear();
       foreach (const QSerialPortInfo &info, *filteredPorts) {
           ui->comboBox_serial->addItem( info.portName() + " - " + info.manufacturer() + " - " +  info.description() );
//           qDebug() << "Name        : " << info.portName();
//           qDebug() << "Description : " << info.description();
//           qDebug() << "Manufacturer: " << info.manufacturer();
//           qDebug() << "ProcducerId : " << info.hasProductIdentifier() << "  " << info.productIdentifier();
//           qDebug() << "BaudRates   : " << info.standardBaudRates();
//           qDebug() << "sysLocation : " << info.systemLocation();
//           qDebug() << "isValid     : " << info.isValid();
//           qDebug() << "isNull      : " << info.isNull();
//           qDebug() << "isBusy      : " << info.isBusy();
       }
}


void MainWindow::on_actionSave_triggered()
{
    QString selfilter = tr("Minibee2osc settings files (*.ini)");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save settings"), QString(),
               tr("Minibee2osc settings files (*.ini)"), &selfilter);

       if (!fileName.isEmpty()) {
        saveSettings( &fileName );
       }
}

void MainWindow::on_actionLoad_triggered()
{

    QString selfilter = tr("Minibee2osc settings files (*.ini)");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load settings"), QString(),
               tr("Minibee2osc settings files (*.ini)"), &selfilter);

       if (!fileName.isEmpty()) {
            loadSettings( &fileName );
       }

}

void MainWindow::saveSettings(QString * filename)
{
    QSettings * settings;
    if ( filename != NULL ){
        settings = new QSettings(*filename, QSettings::IniFormat);
    } else {
        settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SenseStage", "minibee2osc");
        qDebug() << settings->fileName();
    }

    QString configfile = ui->lineEdit_configfile->text();
//        QString serialport = ui->comboBox_serial->currentText();
    int selectedPort = ui->comboBox_serial->currentIndex();
    QString serialport = filteredPorts->at( selectedPort ).systemLocation();
    QString targethost = ui->lineEdit_sendOSC->text();
    QString targethostport = ui->lineEdit_sendOSCport->text();
    QString myport = ui->lineEdit_listenOSCport->text();
    int verbosity = ui->spinBox_loglevel->value();
    bool autostart = ui->checkBox_autostart->isChecked();

    settings->setValue( "configuration/file", configfile );
    settings->setValue( "serial/port", serialport );
    settings->setValue( "osc/targethostip", targethost );
    settings->setValue( "osc/targethostport", targethostport );
    settings->setValue( "osc/listenport", myport );
    settings->setValue( "log/level", verbosity );
    settings->setValue( "autostart", autostart );

    settings->sync();

    if ( filename != NULL ){
        ui->text_output->setTextColor( QColor( "blue" ) );
        ui->text_output->append( "\nSaved settings to: " + *filename );
        ui->actionCurrent_settings->setText( *filename );
    }
}

void MainWindow::loadSettings(QString * filename)
{
    QSettings * settings;
    if ( filename != NULL ){
        settings = new QSettings(*filename, QSettings::IniFormat);
    } else {
        settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SenseStage", "minibee2osc");
        qDebug() << settings->fileName();
    }
    ui->lineEdit_configfile->setText( settings->value( "configuration/file" ).toString() );
//    int noItems = ui->comboBox_serial->count();
//    ui->comboBox_serial->addItem( settings->value( "serial/port").toString() );
    QString portName = settings->value( "serial/port").toString();
    int count = 0; int selected = -1;
    foreach (const QSerialPortInfo &info, *filteredPorts ) {
        if (info.systemLocation().contains(QRegExp(portName))) {
                selected = count;
                break;
        }
        count++;
    }
    if ( selected >= 0 ){
        ui->comboBox_serial->setCurrentIndex( selected );
    }

    ui->lineEdit_sendOSC->setText( settings->value( "osc/targethostip").toString() );
    ui->lineEdit_sendOSCport->setText( settings->value( "osc/targethostport").toString() );
    ui->lineEdit_listenOSCport->setText( settings->value( "osc/listenport" ).toString() );
    ui->spinBox_loglevel->setValue( settings->value( "log/level").toInt() );
    ui->checkBox_autostart->setChecked( settings->value("autostart").toBool() );

    if ( filename != NULL ){
        ui->text_output->setTextColor( QColor( "blue" ) );
        ui->text_output->append( "\nLoaded settings from: " + *filename );
        ui->actionCurrent_settings->setText( *filename );
    }
}
