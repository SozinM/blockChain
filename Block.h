#ifndef BLOCK_H
#define BLOCK_H

#include <QByteArray>
#include <QDateTime>
#include <QVariant>

class Block
{
public:
    explicit Block(int index, QByteArray prevHash, QVariant data, int nonce = 0);
    Block();


    int index() const;
    void setIndex(int index);

    int nonce() const;
    void setNonce(int nonce);

    QVariant data() const;
    void setData(const QVariant &data);

    QByteArray prevHash() const;
    void setPrevHash(const QByteArray &prevHash);

    QByteArray hash() const;
    QByteArray hash(int nonce) const;
    QByteArray header() const;
    QString toString() const;

private:
    int m_index;
    int m_nonce;
    QVariant m_data;
    QByteArray m_prevHash;

};

#endif // BLOCK_H
