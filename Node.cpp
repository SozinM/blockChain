#include "Node.h"

Node::Node()
{
    connect(&m_broadSock,SIGNAL(readyRead()),this,SLOT(readBroadcastDatagram()));
}

Node::Node(const QString &ipAddr, quint16 port)
{
    m_ipAddress = QHostAddress(ipAddr);
    m_broadPort = port;
    m_broadSock.bind(m_ipAddress, m_broadPort);
    connect(&m_broadSock,SIGNAL(readyRead()),this,SLOT(readBroadcastDatagram()));
}

void Node::setNetParams(const QString &ipAddr, quint16 port)
{
    m_ipAddress = QHostAddress(ipAddr);
    m_broadPort = port;
    m_broadSock.bind(m_ipAddress, m_broadPort);
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
