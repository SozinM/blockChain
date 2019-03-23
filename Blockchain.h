#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <QHash>
#include "Block.h"

class Blockchain
{

public:
    explicit Blockchain(Block genericBlock = Block());
    ~Blockchain();    
    int size() const;
    Block genericBlock() const;
    int lastBlockIndex() const;
    Block lastBlock() const;
    Block blockAt(int index) const;
    void append(const Block &block);
    void setDifficulty(int difficulty);
    int difficulty() const;

private:
    QHash<int,Block> m_blockChain;
    int m_difficulty;
    int m_lastIndex;



};

#endif // BLOCKCHAIN_H
