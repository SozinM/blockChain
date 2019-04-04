<<<<<<< HEAD
#include "Block.h"
#include <QCryptographicHash>
#include <QString>
#include <QDataStream>

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
    QString preHash = QString ("%1%2%3%4").arg(m_index)
                        .arg(QString::fromStdString(m_prevHash.toStdString()))                      
                        .arg(m_nonce)
                        .arg(m_data.toString());

    return QCryptographicHash::hash(QByteArray::fromStdString(preHash.toStdString()),
                                    QCryptographicHash::Sha256).toHex();
}

QByteArray Block::hash(int nonce) const
{
    QString preHash = QString ("%1%2%3%4").arg(m_index)
                        .arg(QString::fromStdString(m_prevHash.toStdString()))                        
                        .arg(nonce)
                        .arg(m_data.toString());

    return QCryptographicHash::hash(QByteArray::fromStdString(preHash.toStdString()),
                                    QCryptographicHash::Sha256).toHex();
}

QByteArray Block::toByteArray() const
{
    QByteArray blockAsByteArray;
    //может быть можно сделать красивее, через QDataStream например
    char a = ( m_index >> 24 )& 0xFF;
    char b = ( m_index >> 16 )& 0xFF;
    char c = ( m_index >> 8 )& 0xFF;
    char d = m_index & 0xFF;
    blockAsByteArray.append(a);
    blockAsByteArray.append(b);
    blockAsByteArray.append(c);
    blockAsByteArray.append(d);
    blockAsByteArray.append(m_prevHash);
    a = ( m_nonce >> 24 )& 0xFF;
    b = ( m_nonce >> 16 )& 0xFF;
    c = ( m_nonce >> 8 )& 0xFF;
    d = m_nonce & 0xFF;
    blockAsByteArray.append(a);
    blockAsByteArray.append(b);
    blockAsByteArray.append(c);
    blockAsByteArray.append(d);
    blockAsByteArray.append(m_data.toByteArray());

    return  blockAsByteArray;
}

const Block Block::fromByteArray(const QByteArray &byteArray)
{
    Block block;
    int offset = 0;
    QDataStream streamForIndex(byteArray.mid(offset, sizeof(int)));
    int index = 0;
    streamForIndex >> index;
    block.setIndex(index);
    offset += sizeof(int);
    block.setPrevHash(byteArray.mid(offset,
                      QCryptographicHash::hashLength(QCryptographicHash::Sha512)));
    offset += QCryptographicHash::hashLength(QCryptographicHash::Sha512);
    QDataStream streamForNonce(byteArray.mid(offset,sizeof(int)));
    int nonce = 0;
    streamForNonce >> nonce;
    block.setNonce(nonce);
    offset += sizeof(int);
    block.setData(byteArray.mid(offset,byteArray.size()-offset));

    return block;
}
=======
#include "Block.h"
#include <QCryptographicHash>
#include <QString>

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
    QString preHash = QString ("%1%2%3%4").arg(m_index)
                        .arg(QString::fromStdString(m_prevHash.toStdString()))
                        .arg(m_data.toString())
                        .arg(m_nonce);

    return QCryptographicHash::hash(QByteArray::fromStdString(preHash.toStdString()),
                                    QCryptographicHash::Sha256).toHex();
}

QByteArray Block::hash(int nonce) const
{
    QString preHash = QString ("%1%2%3%4").arg(m_index)
                        .arg(QString::fromStdString(m_prevHash.toStdString()))
                        .arg(m_data.toString())
                        .arg(nonce);

    return QCryptographicHash::hash(QByteArray::fromStdString(preHash.toStdString()),
                                    QCryptographicHash::Sha256).toHex();
}

QByteArray Block::toByteArray() const
{
    QByteArray blockAsByteArray;
    blockAsByteArray.setNum(m_index);
    blockAsByteArray.append(m_prevHash);
    blockAsByteArray.append(m_data.toByteArray());
    blockAsByteArray.append(QByteArray::number(m_nonce));

    return  blockAsByteArray;
}

const Block Block::fromByteArray(const QByteArray &byteArray)
{
    Block block;
    int offset = 0;
    block.setIndex(byteArray.mid(offset, sizeof(int)).toInt());
    offset += sizeof(int);
    block.setPrevHash(byteArray.mid(offset,
                      QCryptographicHash::hashLength(QCryptographicHash::Sha256)));
    offset += QCryptographicHash::hashLength(QCryptographicHash::Sha256);
    block.setNonce(byteArray.mid(offset,sizeof(int)).toInt());
    offset += sizeof(int);
    block.setData(byteArray.mid(offset,byteArray.size()-offset));

    return block;
}
>>>>>>> ff06e58c983b165fec05b628fefdffa6da56a677
