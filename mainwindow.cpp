#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    node("192.168.0.102")
{
    ui->setupUi(this);
    //настраиваем таблицу
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->verticalHeader()->setVisible(false);
    QStringList tableLabels;
    tableLabels << "Index" << "Previous HASH" <<  "Nonce" << "Data";
    //
    ui->tableWidget->setHorizontalHeaderLabels(tableLabels);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTable()));
    timer->start(5000);
    connect(&creator,SIGNAL(createdBlock(const Block)),&node,SLOT(anonce(const Block)));
    node.requestSynchronization();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    blockchain.setDifficulty(5);
    creator.setDifficulty(blockchain.difficulty());
    const Block block = creator.createBlock(node.blockchain.lastBlockIndex()+1,
                                            node.blockchain.lastBlockHash(),"data");
    node.blockchain.append(block);
    //updateTable();
}

void MainWindow::updateTable()
{
    int i = ui->tableWidget->rowCount();
    if (node.blockchain.lastBlockIndex() == i)
        return;
    ui->tableWidget->insertRow(i);
    ui->tableWidget->setItem(i,0, new QTableWidgetItem(QString::number(node.blockchain.lastBlockIndex())));
    ui->tableWidget->setItem(i,1, new QTableWidgetItem(QString::fromStdString(node.blockchain.lastBlockPrevHash().toStdString())));
    ui->tableWidget->setItem(i,2, new QTableWidgetItem(QString::number(node.blockchain.lastBlockNonce())));
    ui->tableWidget->setItem(i,3, new QTableWidgetItem(node.blockchain.lastBlockData().toString()));
}
