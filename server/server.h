#ifndef SERVER_H
#define SERVER_H
#include <QTcpSocket>
#include <QSqlRecord>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpServer>
#include "studentdatabase.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr) : QTcpServer(parent) {}
    void incomingConnection(qintptr socketDescriptor) override;

private:
    StudentDatabase db;
};

#endif // SERVER_H
