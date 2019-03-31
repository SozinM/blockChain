#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    node("192.168.0.102")
{
    ui->setupUi(this);
    connect(&creator,SIGNAL(createdBlock(const Block)),&node,SLOT(anonce(const Block)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    blockchain.setDifficulty(5);
    creator.setDifficulty(blockchain.difficulty());
    creator.createBlock(blockchain.lastBlockIndex(), blockchain.lastBlockHash(),"data");
}
