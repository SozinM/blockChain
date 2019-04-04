<<<<<<< HEAD
#include "Blockchain.h"


Blockchain::Blockchain(Block genericBlock)
{
    //Если создаём блокчейн с нуля
    if(genericBlock.index() == -1)
    {
        genericBlock.setIndex(0);
        genericBlock.setNonce(0);
        genericBlock.setData("Hello im first block");
        genericBlock.setPrevHash(QByteArray());//Эквивалент нулю

        append(genericBlock);
    }
    else//Случай когда используем создание не с нулевого блока
    {

    }
}

Blockchain::~Blockchain()
{

}

int Blockchain::size() const
{
    return m_blockChain.size();
}

int Blockchain::lastBlockIndex() const
{
    return m_lastIndex;
}

Block Blockchain::genericBlock() const
{
    int genericBlock = 0;
    return m_blockChain.value(genericBlock);
}

QByteArray Blockchain::lastBlockHash() const
{
    return m_blockChain.value(m_lastIndex).hash();
}

QByteArray Blockchain::lastBlockPrevHash() const
{
    return m_blockChain.value(m_lastIndex).prevHash();
}

QVariant Blockchain::lastBlockData() const
{
    return m_blockChain.value(m_lastIndex).data();
}

int Blockchain::lastBlockNonce() const
{
    return m_blockChain.value(m_lastIndex).nonce();
}

Block Blockchain::lastBlock() const
{
    return m_blockChain.value(m_lastIndex);
}

Block Blockchain::blockAt(int index) const
{
    return m_blockChain.value(index);
}
//Добавляет блок по индексу
void Blockchain::append(const Block &block)
{
    //checkBlock фнукция проверки блока на удовлетворение условиям
    if(!m_blockChain.contains(block.index()))
    {
        //проверка что предыдущий хэш нового блока совпадает с хэшем последнего блока
        if (lastBlockHash() == block.prevHash())
        {
          m_blockChain.insert(block.index(),block);
          m_lastIndex = block.index();
        }

    }

}

void Blockchain::setDifficulty(int difficulty)
{
    m_difficulty = difficulty;
}

int Blockchain::difficulty() const
{
    return m_difficulty;
}
=======
#include "Blockchain.h"


Blockchain::Blockchain(Block genericBlock)
{
    //Если создаём блокчейн с нуля
    if(genericBlock.index() == -1)
    {
        genericBlock.setIndex(0);
        genericBlock.setNonce(0);
        genericBlock.setData("Hello im first block");
        genericBlock.setPrevHash(QByteArray());//Эквивалент нулю

        append(genericBlock);
    }
    else//Случай когда используем создание не с нулевого блока
    {

    }
}

Blockchain::~Blockchain()
{

}

int Blockchain::size() const
{
    return blockchain.size();
}

int Blockchain::lastBlockIndex() const
{
    return m_lastIndex;
}

Block Blockchain::genericBlock() const
{
    int genericBlock = 0;
    return blockchain.value(genericBlock);
}

QByteArray Blockchain::lastBlockHash() const
{
    return blockchain.value(m_lastIndex).hash();
}

Block Blockchain::lastBlock() const
{
    return blockchain.value(m_lastIndex);
}

Block Blockchain::blockAt(int index) const
{
    return blockchain.value(index);
}
//Добавляет блок по индексу
void Blockchain::append(const Block &block)
{
    //checkBlock фнукция проверки блока на удовлетворение условиям
    if(!blockchain.contains(block.index()))
    {
        //проверка что предыдущий хэш нового блока совпадает с хэшем последнего блока
        if (lastBlockHash() == block.prevHash())
        {
          blockchain.insert(block.index(),block);
        }

    }
    m_lastIndex = block.index();
}

void Blockchain::setDifficulty(int difficulty)
{
    m_difficulty = difficulty;
}

int Blockchain::difficulty() const
{
    return m_difficulty;
}
>>>>>>> ff06e58c983b165fec05b628fefdffa6da56a677
