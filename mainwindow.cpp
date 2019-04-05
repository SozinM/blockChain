#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    node("10.10.10.10")
{
    ui->setupUi(this);
    setupUI();

    connect(ui->pbMine,SIGNAL(clicked()),SLOT(pbMine_clicked()));
    connect(&creator,SIGNAL(createdBlock(const Block)),&node,SLOT(anonce(const Block)));
    node.setParent(this);
//    //Сомнительно, что тут должно сразу идти это
    node.requestSynchronization();
// Ну это же по сути запуск программы. Предположение, что как только запускается программа необходимо
// провести синхронизацию
    updateTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    tbvBlockchainModel = new QStandardItemModel;
    tbvBlockchainModel->setColumnCount(5);

    tbvBlockchainModel->setHeaderData(0,Qt::Horizontal,"index",Qt::DisplayRole);
    tbvBlockchainModel->setHeaderData(1,Qt::Horizontal,"data",Qt::DisplayRole);
    tbvBlockchainModel->setHeaderData(2,Qt::Horizontal,"nonce",Qt::DisplayRole);
    tbvBlockchainModel->setHeaderData(3,Qt::Horizontal,"hash",Qt::DisplayRole);
    tbvBlockchainModel->setHeaderData(4,Qt::Horizontal,"previous hash",Qt::DisplayRole);

    ui->tbvBlockchain->setModel(tbvBlockchainModel);
}

void MainWindow::pbMine_clicked()
{
    node.blockchain.setDifficulty(2);
    creator.setDifficulty(node.blockchain.difficulty());
    const Block block = creator.createBlock(node.blockchain.lastBlockIndex()+1,
                                            node.blockchain.lastBlockHash(),"data");
    node.blockchain.append(block);

    QTimer::singleShot(0,this,SLOT(updateTable()));
}

void MainWindow::updateTable()
{
    for(int i = tbvBlockchainModel->rowCount() ; i <= node.blockchain.lastBlockIndex(); i++)
    {
        QList<QStandardItem*> row;
        QStandardItem *index = new QStandardItem(QString::number(node.blockchain.blockAt(i).index()));
        row.append(index);
        QStandardItem *data = new QStandardItem(node.blockchain.blockAt(i).data().toString());
        row.append(data);
        QStandardItem *nonce = new QStandardItem(QString::number(node.blockchain.blockAt(i).nonce()));
        row.append(nonce);
        QStandardItem *hash = new QStandardItem(QVariant(node.blockchain.blockAt(i).hash()).toString());
        row.append(hash);
        QStandardItem *prevHash = new QStandardItem(QVariant(node.blockchain.blockAt(i).prevHash()).toString());
        row.append(prevHash);

        tbvBlockchainModel->insertRow(i,row);
    }
}
