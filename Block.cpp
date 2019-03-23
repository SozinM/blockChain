#include "Block.h"

Block::Block(int index, QByteArray prevHash, QVariant data, int nonce)
{
    m_index = index;
    m_prevHash = prevHash;
    m_data = data;
    m_nonce = nonce;
}

Block::Block()
{
    m_index = -1;
    m_prevHash = QByteArray();
    m_data = -1;
    m_nonce = -1;
}


int Block::index() const
{
    return m_index;
}

void Block::setIndex(int index)
{
    m_index = index;
}

int Block::nonce() const
{
    return m_nonce;
}

void Block::setNonce(int nonce)
{
    m_nonce = nonce;
}

QVariant Block::data() const
{
    return m_data;
}

void Block::setData(const QVariant &data)
{
    m_data = data;
}

QByteArray Block::prevHash() const
{
    return m_prevHash;
}

void Block::setPrevHash(const QByteArray &prevHash)
{
    m_prevHash = prevHash;
}

QByteArray Block::hash() const
{
//    строка для хэширования
//    QString("%1%2%3%4").arg(m_index).arg(m_data).arg(m_prevHash).arg(m_nonce);
}

QByteArray Block::hash(int nonce) const
{
//    строка для хэширования
//    QString("%1%2%3%4").arg(m_index).arg(m_data).arg(m_prevHash).arg(nonce)
}
