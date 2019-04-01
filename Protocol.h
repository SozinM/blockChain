#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <QTypeInfo>

//типы сообщений
const quint8 MSG_TYPE_SIZE = 6;
const char ANONCE[] = "ANONCE"; //анонс нового блока
const char REQSYN[] = "REQSYN"; //запрос на синхронизацию
const char CANSYN[] = "CANSYN"; //ответ - могу синхронизировать
const char SYNFIN[] = "SYNFIN"; //синхронизация завершена
#endif // PROTOCOL_H
