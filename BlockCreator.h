#ifndef BLOCKCREATOR_H
#define BLOCKCREATOR_H

#include "Block.h"
#include <QObject>

class BlockCreator: public QObject
{
    Q_OBJECT
public:
    BlockCreator();
    BlockCreator(int difficulty);
    void setDifficulty(int difficulty);
    Block createBlock(int index, QByteArray prevHash, QVariant data);
    int hashDifficulty(const QByteArray &hash);
signals:
    void createdBlock(const Block &block);
private:
    int m_difficulty;
};

#endif // BLOCKCREATOR_H
