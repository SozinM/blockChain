#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include "Protocol.h"
#include "Block.h"

const quint16 DEFAULT_BROADCAST_PORT = 555; //magic num :)

class Node: public QObject
{
    Q_OBJECT
public:
    Node();
    Node(const QString &ipAddr, quint16 port = DEFAULT_BROADCAST_PORT);
    void setNetParams(const QString &ipAddr, quint16 port = DEFAULT_BROADCAST_PORT);
    void requestSynchronization(int chainSize);//запросить синхронизацию, передаем свою длину цепочки
//хосты получившие запрос проверят длину цепочки. Если у них цепочка длинее - то они отвечают, что могут
//синхронизировать
//Почему слоты приватные? По задумке они должны активироваться по сигналу, поэтому запрещено вызывать
// их самостоятельно
private slots:
    void anonce(const Block &block);//анонс нового блока, привязывается к сигналу createdBlock
    void sendBroadcastDatagram(const QByteArray &datagram);
    void readBroadcastDatagram();
private:
    QUdpSocket m_broadSock; //UDP сокет для приема и отправки широковещательных датаграм
    quint16 m_broadPort; // порт узла
    QHostAddress m_ipAddress; // ip адресс узла
};


#endif // NODE_H
