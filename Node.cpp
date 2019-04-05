#include "Node.h"
#include <QTimer>

Node::Node():
     m_srv("server", QWebSocketServer::NonSecureMode, this)
{
    connect(&m_broadSock,SIGNAL(readyRead()),this,SLOT(readBroadcastDatagram()));
}

Node::Node(const QString &ipAddr, quint16 broadcastPort, quint16 directPort):
    m_srv("server", QWebSocketServer::NonSecureMode, this)
{
    m_ipAddress = QHostAddress(ipAddr);
    m_broadPort = broadcastPort;
    m_broadSock.bind(m_ipAddress, m_broadPort);
    m_directPort = directPort;
    connect(&m_broadSock,SIGNAL(readyRead()),this,SLOT(readBroadcastDatagram()));
    connect(&m_srv, SIGNAL(newConnection()),this,SLOT(onNewConnection()));

}

void Node::setNetParams(const QString &ipAddr, quint16 broadcastPort, quint16 directPort)
{
    m_ipAddress = QHostAddress(ipAddr);
    m_broadPort = broadcastPort;
    m_broadSock.bind(m_ipAddress, m_broadPort);
    m_directPort = directPort;
}

void Node::sendBroadcastDatagram(const QByteArray &datagram)
{
    m_broadSock.writeDatagram(datagram,m_ipAddress.Broadcast, m_broadPort);
}

void Node::readBroadcastDatagram()
{
    while (m_broadSock.hasPendingDatagrams())
    {
        QHostAddress senderIp;
        quint16 senderPort;
        QByteArray datagram;
        datagram.resize(m_broadSock.pendingDatagramSize());
        if (m_broadSock.readDatagram(datagram.data(), datagram.size(),
                                     &senderIp, &senderPort) != -1)
        {
           if (senderIp != m_ipAddress)
           {
                processRequest(datagram,senderIp);
           }
        }
     }
}

void Node::requestSynchronization()
{
    QByteArray datagram;
    datagram.append(REQSYN.data());
    datagram.append(QByteArray::number(blockchain.size()));
    sendBroadcastDatagram(datagram);
}

void Node::anonce(const Block block)
{
    QByteArray datagram;
    datagram.append(ANONCE.data());
    datagram.append(block.toByteArray());
    sendBroadcastDatagram(datagram);
}

void Node::processRequest(const QByteArray &datagram, const QHostAddress &sender)
{
    QByteArray msgType = datagram.mid(0, MSG_TYPE_SIZE);
    //если пришел анонс - то добавляем блок в блокчейн
    if (msgType == ANONCE)
    {
        const Block block = Block::fromByteArray(datagram.mid(MSG_TYPE_SIZE,datagram.size()));
        blockchain.append(block);
        QTimer::singleShot(0,this->parent(),SLOT(updateTable()));
    }
    //если кому-то требуется синхронизация - то проверяем длину своей локлаьной цепочки
    //и если локальная цепочка длинее - то отправляем сообщение о готовности синхронизировать
    else if (msgType == REQSYN)
    {
        int remoteBlockchainSize = datagram.mid(MSG_TYPE_SIZE, sizeof (int)).toInt();
        if (blockchain.size() > remoteBlockchainSize)
        {
            QByteArray response;
            response.append(CANSYN.data());
            m_broadSock.writeDatagram(response,sender,m_broadPort);
            //начинаем слушать прямое подключение для синхронизации
            m_srv.listen(m_ipAddress, DEFAULT_SRV_PORT);
        }

    }
    //если пришло сообщение, что кто-то готов синхронизировать наш узел
    //то создаем tcp соединение с этим узлом
    else if (msgType == CANSYN)
    {
        connect(&m_directSock, SIGNAL(binaryMessageReceived(const QByteArray)),
                this, SLOT(readDirectMessage(const QByteArray)));
        QString strUrl = "ws://"+sender.toString()+":"+QString::number(DEFAULT_SRV_PORT);
        m_directSock.open(QUrl(strUrl));

    }
}

void Node::onNewConnection()
{
       QWebSocket *synConnection = m_srv.nextPendingConnection();
       synchronize(synConnection);
}

void Node::synchronize(QWebSocket *synConnection)
{
    for (int blockIndex = 1; blockIndex <= blockchain.size(); ++blockIndex)
    {
        synConnection->sendBinaryMessage(blockchain.blockAt(blockIndex).toByteArray());
    }
    synConnection->sendBinaryMessage(SYNFIN);
}


void Node::readDirectMessage(const QByteArray message)
{
    if (message == SYNFIN)
    {
        m_directSock.close();
    }
    else
    {
        const Block block = Block::fromByteArray(message);
        blockchain.append(block);
        QTimer::singleShot(0,this->parent(),SLOT(updateTable()));
    }
}
