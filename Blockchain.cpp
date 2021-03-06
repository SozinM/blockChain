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

        m_blockChain.insert(genericBlock.index(),genericBlock);
        m_lastIndex = genericBlock.index();
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
