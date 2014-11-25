#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Keyframe"),"",tr("Keyframe (*.txt);;All Files (*)"));
    if(fileName.isEmpty())
        return;
    const char *str;;
    QByteArray ba;
    ba = fileName.toLatin1();
    str = ba.data();
    ui->openGLWidget->loadAnglesFromFile(str);
}
