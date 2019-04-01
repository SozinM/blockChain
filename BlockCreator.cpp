#include "BlockCreator.h"

BlockCreator::BlockCreator()
{

}

BlockCreator::BlockCreator(int difficulty)
{
    m_difficulty = difficulty;
}

void BlockCreator::setDifficulty(int difficulty)
{
    m_difficulty = difficulty;
}

int BlockCreator::hashDifficulty(const QByteArray &hash)
{
    int hashDifficulty = 0;
    foreach (char byte, hash)
    {
        if (byte != '0')
        {
            break;
        }
        else
        {
            ++hashDifficulty;
        }
    }
    return hashDifficulty;
}

Block BlockCreator::createBlock(int index, QByteArray prevHash, QVariant data)
{
    Block candidateBlock(index,prevHash,data);
    int candidateNonce = 0;
    //пытаемся создать блок пока его хэш не будет удовлетворять заданной сложности
    while (hashDifficulty(candidateBlock.hash()) != m_difficulty)
    {
        candidateBlock.setNonce(++candidateNonce);
    }

    emit createdBlock(candidateBlock);
    return candidateBlock;
}
