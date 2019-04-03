#ifndef PROTOCOL_H
#define PROTOCOL_H
//#include <QTypeInfo>
#include <QByteArray>
#include <QString>
#include <QVariant>
//типы сообщений
const quint8 MSG_TYPE_SIZE = 6;

static QByteArray ANONCE = QVariant("ANONCE").toByteArray(); //анонс нового блока
static QByteArray REQSYN = QVariant("REQSYN").toByteArray(); //запрос на синхронизацию
static QByteArray CANSYN = QVariant("CANSYN").toByteArray(); //ответ - могу синхронизировать
static QByteArray SYNFIN = QVariant("SYNFIN").toByteArray(); //синхронизация завершена
#endif // PROTOCOL_H
