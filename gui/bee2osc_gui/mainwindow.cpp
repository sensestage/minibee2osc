#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myProcess = NULL;
}

MainWindow::~MainWindow()
{
    myProcess->terminate();
    delete ui;
}

void MainWindow::on_pushButton_start_clicked()
{
    bool started = false;
    if ( myProcess != NULL ){
        if ( myProcess->state() == QProcess::NotRunning ){
            started = false;
        } else {
            started = true;
        }
    }
    if ( !started ){
        QString configfile = ui->lineEdit_configfile->text();
        QString serialport = ui->comboBox_serial->currentText();
        QString targethost = ui->lineEdit_sendOSC->text();
        QString targethostport = ui->lineEdit_sendOSCport->text();
        QString myport = ui->lineEdit_listenOSCport->text();
        int verbosity = ui->spinBox_loglevel->value();

        ui->groupBox_configuration->setVisible( false );
        ui->groupBox_listenosc->setVisible(false );
        ui->groupBox_sendosc->setVisible(false);
        ui->groupBox_serialport->setVisible(false);
        ui->groupBox_verbosity->setVisible(false);

        QString program = "/home/nescivi/git/ssdn/minibee2osc/bin/minibee2osc";
        QStringList arguments;
        arguments << "-c" << configfile << "-s" << serialport << "-i" << targethost << "-p" << targethostport << "-l" << myport << "-v" << QString::number(verbosity);

        myProcess = new QProcess(this);
        myProcess->start(program, arguments);

        ui->pushButton_start->setText("stop");

        QObject::connect(myProcess, SIGNAL(finished(int , QProcess::ExitStatus )), this, SLOT(process_ended()) );
        QObject::connect(myProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdOutput()) );
        QObject::connect(myProcess, SIGNAL(readyReadStandardError()), this, SLOT(readStdError()) );
    } else {
        QObject::disconnect(myProcess, SIGNAL(finished(int , QProcess::ExitStatus )), this, SLOT(process_ended()) );
        QObject::disconnect(myProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdOutput()) );
        QObject::disconnect(myProcess, SIGNAL(readyReadStandardError()), this, SLOT(readStdError()) );
        ui->pushButton_start->setText("start");
        ui->groupBox_configuration->setVisible( true );
        ui->groupBox_listenosc->setVisible(true );
        ui->groupBox_sendosc->setVisible(true);
        ui->groupBox_serialport->setVisible(true);
        ui->groupBox_verbosity->setVisible(true);
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
    ui->text_output->append( myProcess->readAllStandardOutput() );

}

void MainWindow::process_ended()
{
    QObject::disconnect(myProcess, SIGNAL(finished(int , QProcess::ExitStatus )), this, SLOT(process_ended()) );
    QObject::disconnect(myProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdOutput()) );
    QObject::disconnect(myProcess, SIGNAL(readyReadStandardError()), this, SLOT(readStdError()) );
    ui->pushButton_start->setText("start");
    ui->groupBox_configuration->setVisible( true );
    ui->groupBox_listenosc->setVisible(true );
    ui->groupBox_sendosc->setVisible(true);
    ui->groupBox_serialport->setVisible(true);
    ui->groupBox_verbosity->setVisible(true);

}

void MainWindow::on_pushButton_browse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
               tr("Minibee2osc Configuration Files (*.xml);"));

       if (!fileName.isEmpty()) {
           ui->lineEdit_configfile->setText( fileName );
       }
}

void MainWindow::on_pushButton_2_clicked()
{
    // clear contents of text window
    ui->text_output->clear();
}

void MainWindow::on_pushButton_clicked()
{

// not working yet...

//    // Clear first, because the QTextEdit::copy() command doesn't do this
//    // if nothing is selected.
//    QApplication::clipboard()->clear (QClipboard::Clipboard);

//    // copy contents of text window
//    ui->text_output->copy();

//    const QString clipSel2 = QApplication::clipboard()->text (QClipboard::Clipboard);
}

void MainWindow::on_pushButton_3_clicked()
{
    // save contents of text window
    QString contents = ui->text_output->toPlainText();

}
