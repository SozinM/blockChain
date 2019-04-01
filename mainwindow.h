#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <../GitHub/blockChain/Blockchain.h>
#include <../GitHub/blockChain/BlockCreator.h>
#include <../GitHub/blockChain/Node.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void updateTable();
private:
    Ui::MainWindow *ui;
    Blockchain blockchain;
    BlockCreator creator;
    Node node;
};

#endif // MAINWINDOW_H
