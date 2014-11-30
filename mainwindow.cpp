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

void MainWindow::on_recordFrameButton_clicked()
{
    ui->openGLWidget->recordActiveScene(ui->intervalBox->value());
}

void MainWindow::on_saveButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save frames"),"",tr("Frame Collection (*.txt);;All Files (*)"));
    if(fileName.isEmpty())
        return;
    const char *str;;
    QByteArray ba;
    ba = fileName.toLatin1();
    str = ba.data();
    std::ofstream of;
    of.open(str);

    of << ui->openGLWidget->dump();
    of.close();
}

void MainWindow::on_changeFrameButton_clicked()
{
    int nn = ui->changeFrameEdit->text().toInt();
    ui->openGLWidget->changeActiveFrame(nn);
}
