#include "BlockchainDB.h"
#include "Block.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QDebug>

BlockchainDB::BlockchainDB(const QString &dbType, const QString &dbName)
{
    m_db = QSqlDatabase::addDatabase(dbType);
    m_db.setDatabaseName(dbName);
}

bool BlockchainDB::loadBlockchain(Blockchain &blockchain)
{
    if (!m_db.open())
    {
        return false;
    }
    QSqlQuery query;
    if (!query.exec("SELECT * FROM blockchain"))
    {
        //для тестирования
        qDebug() << query.lastError().text();
        return  false;
    }
    while(query.next())
    {
        const Block block(query.value(0).toInt(),query.value(1).toByteArray(),
                          query.value(2).toByteArray(),query.value(3).toInt());
        blockchain.append(block);
    }

    return true;
}

bool BlockchainDB::saveBlockchain(const Blockchain &blockchain) const
{
    QSqlQuery query;
    Q_ASSERT(query.driver()->hasFeature(QSqlDriver::NamedPlaceholders));
    for (int i = 0; i < blockchain.size(); ++i)
    {
       const Block block = blockchain.blockAt(i);
       query.prepare("INSERT INTO blockchain "
                     "VALUES(:index, :prevHash, :data, :nonce)");
       query.bindValue(":index", block.index());
       query.bindValue(":prevHash", block.prevHash());
       query.bindValue(":data", block.data().toByteArray());
       query.bindValue(":nonce", block.nonce());
       if (!query.exec())
       {
           //Заглушка для тестирования
            qDebug() << query.executedQuery();
            qDebug() << query.lastError().text();
       }
    }
    return true;
}
