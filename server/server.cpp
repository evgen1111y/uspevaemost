#include "server.h"
#include <QTcpSocket>
#include <QSqlRecord>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, [=]() {
        QByteArray requestData = socket->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(requestData);
        QJsonObject jsonObj = doc.object();

        QString action = jsonObj["action"].toString();
        QJsonObject responseObj;

        if (action == "register") {
            // Регистрация нового пользователя
            QString username = jsonObj["username"].toString();
            QString password = jsonObj["password"].toString();
            bool result = db.addUser(username, password);
            responseObj["result"] = result ? "success" : "error";
        }
        else if (action == "login") {
            // Авторизация пользователя
            QString username = jsonObj["username"].toString();
            QString password = jsonObj["password"].toString();
            bool authResult = db.authenticateUser(username, password);

            if (authResult) {
                QString token = db.generateToken(username);
                responseObj["result"] = "success";
                responseObj["token"] = token;
            } else {
                responseObj["result"] = "error";
            }
        }
        else if (action == "addStudent") {
            // Проверка токена перед выполнением действия
            QString token = jsonObj["token"].toString();
            if (db.validateToken(token)) {
                QString name = jsonObj["name"].toString();
                int group = jsonObj["group"].toInt();
                bool result = db.addStudent(name, group);
                responseObj["result"] = result ? "success" : "error";
            } else {
                responseObj["result"] = "unauthorized";
            }
        }
        else if (action == "getStudents") {
            QString token = jsonObj["token"].toString();
            if (db.validateToken(token)) {
                QSqlTableModel *model = db.getStudentsModel();
                QJsonArray studentsArray;

                for (int i = 0; i < model->rowCount(); ++i) {
                    QJsonObject studentObj;
                    studentObj["id"] = model->record(i).value("id").toInt();
                    studentObj["name"] = model->record(i).value("name").toString();
                    studentObj["group_id"] = model->record(i).value("group_id").toInt();
                    studentsArray.append(studentObj);
                }
                responseObj["students"] = studentsArray;
            } else {
                responseObj["result"] = "unauthorized";
            }
        }

        QJsonDocument responseDoc(responseObj);
        socket->write(responseDoc.toJson());
        socket->flush();
    });

    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}
