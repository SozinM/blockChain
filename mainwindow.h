#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Blockchain.h>
#include <BlockCreator.h>
#include <Node.h>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setupUI();
public slots:
    void updateTable();
private slots:
    void pbMine_clicked();

private:
    Ui::MainWindow *ui;
    Blockchain blockchain;
    BlockCreator creator;
    Node node;

    QStandardItemModel *tbvBlockchainModel;
};

#endif // MAINWINDOW_H
