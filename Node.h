#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpServer>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include "Protocol.h"
#include "Block.h"
#include "Blockchain.h"

const quint16 DEFAULT_BROADCAST_PORT = 5550; //magic num :)
const quint16 DEFAULT_DIRECT_PORT = 5560;
const quint16 DEFAULT_SRV_PORT = 5561;

class Node: public QObject
{
    Q_OBJECT
signals:
    void newBlockAdded();
public:
    Node();
    Node(const QString &ipAddr, quint16 broadcastPort = DEFAULT_BROADCAST_PORT,
         quint16 directPort = DEFAULT_DIRECT_PORT);
    void setNetParams(const QString &ipAddr, quint16 broadcastPort = DEFAULT_BROADCAST_PORT,
                      quint16 directPort = DEFAULT_DIRECT_PORT);
    void requestSynchronization();//запросить синхронизацию, передаем свою длину цепочки
//хосты получившие запрос проверят длину цепочки. Если у них цепочка длинее - то они отвечают, что могут
//синхронизировать
//Почему слоты приватные? По задумке они должны активироваться по сигналу, поэтому запрещено вызывать
// их самостоятельно
private slots:
    void anonce(const Block &block);//анонс нового блока, привязывается к сигналу createdBlock
    void sendBroadcastDatagram(const QByteArray &datagram);
    void readBroadcastDatagram();
    void readDirectDatagram();
    void readBinaryMessage(const QByteArray);
    void onNewConnection();
private:
    void processRequest(const QByteArray &datagram, const QHostAddress &sender);
  //  void synchronize(QTcpSocket *synConnection, int startPos);
    void synchronize(QWebSocket *synConnection);
    QUdpSocket m_broadSock; //UDP сокет для приема и отправки широковещательных датаграм
    QTcpSocket m_directSock; //TCP сокета для синхронизации, прямое подключение к узлу
    QWebSocket m_webSock;
    quint16 m_broadPort; // порт узла
    quint16 m_directPort;
    QHostAddress m_ipAddress; // ip адресс узла
    QTcpServer m_server;//когда узел выступает в роли сервера синхронизации
    QWebSocketServer m_webserver;
public:
    Blockchain blockchain;
};


#endif // NODE_H
