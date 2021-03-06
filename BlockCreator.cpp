#include "BlockCreator.h"

BlockCreator::BlockCreator()
{

}

///Почему 2 метода дублируют друг друга?
BlockCreator::BlockCreator(int difficulty)
{
    m_difficulty = difficulty;
}

void BlockCreator::setDifficulty(int difficulty)
{
    m_difficulty = difficulty;
}
///TODO: Не надо считать какая сложность, надо лишь проверить что она удовлетворяет нашей
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
    srand(time(NULL));

    while (hashDifficulty(candidateBlock.hash()) != m_difficulty)
    {
        candidateBlock.setNonce(rand());
    }

    emit createdBlock(candidateBlock);
    return candidateBlock;
}
