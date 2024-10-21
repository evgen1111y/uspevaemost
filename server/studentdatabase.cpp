#include "studentdatabase.h"
#include <QCryptographicHash>
#include <QDebug>
#include <QTime>

// Конструктор и деструктор
StudentDatabase::StudentDatabase(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("students.db");
}

StudentDatabase::~StudentDatabase()
{
    closeDatabase();
}

// Открытие базы данных
bool StudentDatabase::openDatabase()
{
    if (!db.open()) {
        qDebug() << "Ошибка открытия базы данных: " << db.lastError().text();
        return false;
    }
    return true;
}

// Закрытие базы данных
void StudentDatabase::closeDatabase()
{
    if (db.isOpen()) {
        db.close();
    }
}

// Создание всех таблиц (пользователи, студенты, предметы, оценки)
bool StudentDatabase::createTables()
{
    QSqlQuery query;

    // Создание таблицы пользователей
    QString createUsersTable = "CREATE TABLE IF NOT EXISTS users ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "username TEXT NOT NULL UNIQUE, "
                               "password TEXT NOT NULL)";
    if (!query.exec(createUsersTable)) {
        qDebug() << "Ошибка создания таблицы пользователей: " << query.lastError().text();
        return false;
    }

    // Создание таблицы студентов
    QString createStudentsTable = "CREATE TABLE IF NOT EXISTS students ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "name TEXT NOT NULL, "
                                  "group_id INTEGER NOT NULL)";
    if (!query.exec(createStudentsTable)) {
        qDebug() << "Ошибка создания таблицы студентов: " << query.lastError().text();
        return false;
    }

    // Создание таблицы предметов
    QString createSubjectsTable = "CREATE TABLE IF NOT EXISTS subjects ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "subject_name TEXT NOT NULL)";
    if (!query.exec(createSubjectsTable)) {
        qDebug() << "Ошибка создания таблицы предметов: " << query.lastError().text();
        return false;
    }

    // Создание таблицы оценок
    QString createGradesTable = "CREATE TABLE IF NOT EXISTS grades ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "student_id INTEGER NOT NULL, "
                                "subject_id INTEGER NOT NULL, "
                                "grade INTEGER NOT NULL, "
                                "FOREIGN KEY(student_id) REFERENCES students(id), "
                                "FOREIGN KEY(subject_id) REFERENCES subjects(id))";
    if (!query.exec(createGradesTable)) {
        qDebug() << "Ошибка создания таблицы оценок: " << query.lastError().text();
        return false;
    }

    return true;
}

// Добавление студента
bool StudentDatabase::addStudent(const QString &name, int group)
{
    QSqlQuery query;
    query.prepare("INSERT INTO students (name, group_id) VALUES (:name, :group)");
    query.bindValue(":name", name);
    query.bindValue(":group", group);

    if (!query.exec()) {
        qDebug() << "Ошибка добавления студента: " << query.lastError().text();
        return false;
    }
    return true;
}

// Добавление предмета
bool StudentDatabase::addSubject(const QString &subjectName)
{
    QSqlQuery query;
    query.prepare("INSERT INTO subjects (subject_name) VALUES (:subject_name)");
    query.bindValue(":subject_name", subjectName);

    if (!query.exec()) {
        qDebug() << "Ошибка добавления предмета: " << query.lastError().text();
        return false;
    }
    return true;
}

// Добавление оценки по предмету
bool StudentDatabase::addGrade(int studentId, int subjectId, int grade)
{
    QSqlQuery query;
    query.prepare("INSERT INTO grades (student_id, subject_id, grade) "
                  "VALUES (:student_id, :subject_id, :grade)");
    query.bindValue(":student_id", studentId);
    query.bindValue(":subject_id", subjectId);
    query.bindValue(":grade", grade);

    if (!query.exec()) {
        qDebug() << "Ошибка добавления оценки: " << query.lastError().text();
        return false;
    }
    return true;
}

// Получение модели для отображения студентов
QSqlTableModel* StudentDatabase::getStudentsModel()
{
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("students");
    model->select();
    return model;
}

// Получение модели для отображения предметов
QSqlTableModel* StudentDatabase::getSubjectsModel()
{
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("subjects");
    model->select();
    return model;
}

// Получение модели для отображения оценок
QSqlTableModel* StudentDatabase::getGradesModel()
{
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("grades");
    model->select();
    return model;
}

// Добавление нового пользователя
bool StudentDatabase::addUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPassword);

    if (!query.exec()) {
        qDebug() << "Ошибка добавления пользователя: " << query.lastError().text();
        return false;
    }
    return true;
}

// Авторизация пользователя (проверка логина и пароля)
bool StudentDatabase::authenticateUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec() || !query.next()) {
        qDebug() << "Ошибка авторизации пользователя: " << query.lastError().text();
        return false;
    }

    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
    QString storedPassword = query.value(0).toString();

    return (hashedPassword == storedPassword);
}

// Генерация токена
QString StudentDatabase::generateToken(const QString &username)
{
    QByteArray token = QCryptographicHash::hash(username.toUtf8() + QTime::currentTime().toString().toUtf8(), QCryptographicHash::Sha256).toHex();
    return QString(token);
}

// Валидация токена
bool StudentDatabase::validateToken(const QString &token)
{
    return !token.isEmpty();  // В данной реализации токен валиден, если он не пустой
}
