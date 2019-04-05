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
    QByteArray indexAsByteArray;
    QDataStream streamForIndex(&indexAsByteArray,QIODevice::WriteOnly);
    streamForIndex << m_index;
    blockAsByteArray.append(indexAsByteArray);
    blockAsByteArray.append(m_prevHash);
    QByteArray nonceAsByteArray;
    QDataStream streamForNonce(&nonceAsByteArray, QIODevice::WriteOnly);
    streamForNonce << m_nonce;
    blockAsByteArray.append(nonceAsByteArray);
    //data нужно добавлять последней поскольку все остальные поля имеют фиксированный размер
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
