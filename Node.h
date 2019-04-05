#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include "Protocol.h"
#include "Block.h"
#include "Blockchain.h"

const quint16 DEFAULT_BROADCAST_PORT = 13000; //magic num :)
const quint16 DEFAULT_DIRECT_PORT = 13100;
const quint16 DEFAULT_SRV_PORT = 13001;

class Node: public QObject
{
    Q_OBJECT
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
    void anonce(const Block block);//анонс нового блока, привязывается к сигналу createdBlock
    void sendBroadcastDatagram(const QByteArray &datagram);
    void readBroadcastDatagram();
    void readDirectMessage(const QByteArray);
    void onNewConnection();
private:
    void processRequest(const QByteArray &datagram, const QHostAddress &sender);
    void synchronize(QWebSocket *synConnection);

    QUdpSocket m_broadSock; //UDP сокет для приема и отправки широковещательных датаграм
    QWebSocket m_directSock;
    quint16 m_broadPort; // порт узла
    quint16 m_directPort;
    QHostAddress m_ipAddress; // ip адресс узла
    QWebSocketServer m_srv;// когда узел выступает в роли синхронизурующего узла

public:
    Blockchain blockchain;
};


#endif // NODE_H
