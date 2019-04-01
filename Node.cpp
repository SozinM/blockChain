#include "Node.h"

Node::Node()
{
    connect(&m_broadSock,SIGNAL(readyRead()),this,SLOT(readBroadcastDatagram()));
    connect(&m_directSock,SIGNAL(readyRead()),this,SLOT(readDirectDatagram()));
}

Node::Node(const QString &ipAddr, quint16 broadcastPort, quint16 directPort)
{
    m_ipAddress = QHostAddress(ipAddr);
    m_broadPort = broadcastPort;
    m_broadSock.bind(m_ipAddress, m_broadPort);
    m_directPort = directPort;
    m_directSock.bind(m_ipAddress,m_directPort);
    connect(&m_broadSock,SIGNAL(readyRead()),this,SLOT(readBroadcastDatagram()));
    connect(&m_directSock,SIGNAL(readyRead()),this,SLOT(readDirectDatagram()));
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
           if (senderPort != m_broadPort)
           {
                processRequest(datagram,senderIp);
           }
        }
     }
}

void Node::requestSynchronization(int chainSize)
{
    QByteArray datagram;
    datagram.append(REQSYN);
    datagram.append(QByteArray::number(chainSize));
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
        }
    }
    //если пришло сообщение, что кто-то готов синхронизировать наш узел
    //то создаем tcp соединение с этим узлом
    else if (msgType.compare(CANSYN) == 0)
    {
        m_directSock.connectToHost(sender, m_directPort);
    }
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
        blockchain.append(block);
    }
}
