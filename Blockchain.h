<<<<<<< HEAD
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
    QByteArray lastBlockHash() const;
    QByteArray lastBlockPrevHash() const;
    QVariant lastBlockData() const;
    int lastBlockNonce() const;
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
=======
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
    QByteArray lastBlockHash() const;
    Block lastBlock() const;
    Block blockAt(int index) const;
    void append(const Block &block);
    void setDifficulty(int difficulty);
    int difficulty() const;

private:
    QHash<int,Block> blockchain;
    int m_difficulty;
    int m_lastIndex;



};

#endif // BLOCKCHAIN_H
>>>>>>> ff06e58c983b165fec05b628fefdffa6da56a677
