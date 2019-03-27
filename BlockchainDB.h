#ifndef BLOCKCHAINDB_H
#define BLOCKCHAINDB_H

#include "Blockchain.h"
#include <QtSql/QSqlDatabase>


class BlockchainDB
{
public:
    explicit BlockchainDB(const QString &dbType, const QString &dbName);
/*возможно нужен не bool*/bool loadBlockchain(Blockchain &blockchain);
    bool saveBlockchain(const Blockchain &blockchain) const;
private:
/*
 * запоминаем сколько записей (то есть блоков) загружено из ДБ
 * чтобы потом не сохранять всю цепочку с начала а только недостающие блоки
 */
    int m_lastLoadedBlockIndex;
    QSqlDatabase m_db;
};

#endif // BLOCKCHAINDB_H
