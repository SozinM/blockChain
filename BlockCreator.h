#ifndef BLOCKCREATOR_H
#define BLOCKCREATOR_H

#include "Block.h"

class BlockCreator
{
public:
    BlockCreator();
    BlockCreator(int difficulty);
    void setDifficulty(int difficulty);
    Block createBlock(int index, QByteArray prevHash, QVariant data);
    int hashDifficulty(const QByteArray &hash);
private:
    int m_difficulty;
};

#endif // BLOCKCREATOR_H
