#ifndef STUDENTDATABASE_H
#define STUDENTDATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlTableModel>

class StudentDatabase : public QObject
{
    Q_OBJECT

public:
    explicit StudentDatabase(QObject *parent = nullptr);
    ~StudentDatabase();

    bool openDatabase();
    void closeDatabase();
    bool createTables();  // Создание всех необходимых таблиц
    bool addStudent(const QString &name, int group);
    bool addSubject(const QString &subjectName);
    bool addGrade(int studentId, int subjectId, int grade);  // Добавление оценки по предмету
    QSqlTableModel* getStudentsModel();  // Модель для отображения студентов
    QSqlTableModel* getSubjectsModel();  // Модель для отображения предметов
    QSqlTableModel* getGradesModel();    // Модель для отображения оценок

    // Методы для работы с пользователями (авторизация)
    bool addUser(const QString &username, const QString &password);  // Добавление нового пользователя
    bool authenticateUser(const QString &username, const QString &password);  // Авторизация пользователя
    QString generateToken(const QString &username);  // Генерация токена
    bool validateToken(const QString &token);  // Валидация токена

private:
    QSqlDatabase db;
};

#endif // STUDENTDATABASE_H
