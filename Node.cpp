#include "Node.h"
#include <QMessageBox>
Node::Node():
     m_webserver("server", QWebSocketServer::NonSecureMode, this)
{
    connect(&m_broadSock,SIGNAL(readyRead()),this,SLOT(readBroadcastDatagram()));
    connect(&m_directSock,SIGNAL(readyRead()),this,SLOT(readDirectDatagram()));
}

Node::Node(const QString &ipAddr, quint16 broadcastPort, quint16 directPort):
    m_webserver("server", QWebSocketServer::NonSecureMode, this)
{
    m_ipAddress = QHostAddress(ipAddr);
    m_broadPort = broadcastPort;
    m_broadSock.bind(m_ipAddress, m_broadPort);
    m_directPort = directPort;
    m_directSock.bind(m_ipAddress,m_directPort);
    connect(&m_broadSock,SIGNAL(readyRead()),this,SLOT(readBroadcastDatagram()));
    connect(&m_directSock,SIGNAL(readyRead()),this,SLOT(readDirectDatagram()));
    connect(&m_webserver, SIGNAL(newConnection()),this,SLOT(onNewConnection()));

}

void Node::setNetParams(const QString &ipAddr, quint16 broadcastPort, quint16 directPort)
{
    m_ipAddress = QHostAddress(ipAddr);
    m_broadPort = broadcastPort;
    m_broadSock.bind(m_ipAddress, m_broadPort);
    m_directPort = directPort;
    m_directSock.bind(m_ipAddress, m_directPort);
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
    datagram.append(REQSYN);
    datagram.append(QByteArray::number(blockchain.size()));
    sendBroadcastDatagram(datagram);
}

void Node::anonce(const Block &block)
{
    QByteArray datagram;
    datagram.append(ANONCE);
    datagram.append(block.toByteArray());
    sendBroadcastDatagram(datagram);
}
void Node::processRequest(const QByteArray &datagram, const QHostAddress &sender)
{
    QByteArray msgType = datagram.mid(0, MSG_TYPE_SIZE);
    //если пришел анонс - то добавляем блок в блокчейн
    if (msgType.compare(ANONCE) == 0)
    {
        const Block block = Block::fromByteArray(datagram.mid(MSG_TYPE_SIZE,datagram.size()));
        blockchain.append(block);
        emit newBlockAdded();
    }
    //если кому-то требуется синхронизация - то проверяем длину своей локлаьной цепочки
    //и если локальная цепочка длинее - то отправляем сообщение о готовности синхронизировать
    else if (msgType.compare(REQSYN) == 0)
    {
        int remoteBlockchainSize = datagram.mid(MSG_TYPE_SIZE, sizeof (int)).toInt();
        if (blockchain.size() > remoteBlockchainSize)
        {
            QByteArray response;
            response.append(CANSYN);
            m_broadSock.writeDatagram(response,sender,m_broadPort);
            m_webserver.listen(m_ipAddress, DEFAULT_SRV_PORT);

           // m_server.listen(m_ipAddress,DEFAULT_SRV_PORT);
          //  bool wait = m_server.waitForNewConnection(30000);
           // QMessageBox box;
           // if (m_server.hasPendingConnections())
          //  {
          //      QTcpSocket *synConnection = m_server.nextPendingConnection();
          //      connect(synConnection, &QAbstractSocket::disconnected,
          //                  synConnection, &QObject::deleteLater);
//                synchronize(synConnection,remoteBlockchainSize);
           // }
          //  else
          //  {
          //      box.setText("Has not pending connections");
          //      box.exec();
          //  }

           // m_server.close();
        }

    }
    //если пришло сообщение, что кто-то готов синхронизировать наш узел
    //то создаем tcp соединение с этим узлом
    else if (msgType.compare(CANSYN) == 0)
    {
        //m_directSock.connectToHost(sender,DEFAULT_SRV_PORT);
        //m_directSock.waitForConnected();
        connect(&m_webSock, SIGNAL(binaryMessageReceived(const QByteArray)),
                this, SLOT(readBinaryMessage(const QByteArray)));
        m_webSock.open(QUrl("ws://192.168.0.104:5561"));

    }
}

void Node::onNewConnection()
{
        if (m_webserver.hasPendingConnections())
        {
            QWebSocket *synConnection = m_webserver.nextPendingConnection();
            if (synConnection == 0)
            {
                QMessageBox box;
                box.setText("null socket");
                box.exec();
            }
            synchronize(synConnection);
        }
        else {
            QMessageBox box;
            box.setText("new connection");
            box.exec();
        }

}

/*void Node::synchronize(QTcpSocket *synConnection, int startPos)
{
    for (int i = startPos; i < blockchain.size(); ++i)
    {
        synConnection->write(blockchain.blockAt(i+1).toByteArray());
        synConnection->waitForBytesWritten();
        synConnection->flush();
    }
    synConnection->write(SYNFIN);
    synConnection->waitForBytesWritten();
}*/

void Node::synchronize(QWebSocket *synConnection)
{

    int size = 0;
    for (int index = 1; index <= blockchain.size(); ++index)
    {
        size = synConnection->sendBinaryMessage(blockchain.blockAt(index).toByteArray());
        if (size == 0)
        {
            QMessageBox box;
            box.setText("doesn't receive");
            box.exec();
        }
    }
    synConnection->sendBinaryMessage(QByteArray(SYNFIN));
}

void Node::readDirectDatagram()
{
    QByteArray datagram = m_directSock.readAll();
    if (datagram.mid(0,MSG_TYPE_SIZE).compare(SYNFIN) == 0)
    {
        m_directSock.disconnectFromHost();
    }
    else
    {
        const Block block = Block::fromByteArray(datagram);
        QMessageBox box;
        box.setText(QString::number(block.index()));
        box.exec();
        blockchain.append(block);
    }
}

void Node::readBinaryMessage(const QByteArray message)
{
    if (message.compare(SYNFIN) == 0)
    {
        m_webSock.close();
    }
    else
    {
        const Block block = Block::fromByteArray(message);
        QMessageBox box;
        box.setText(QString::number(block.index()));
        box.exec();
        blockchain.append(block);
        emit newBlockAdded();
    }

}
