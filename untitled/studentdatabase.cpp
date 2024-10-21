#include "studentdatabase.h"
#include "qsqlerror.h"
#include <QDebug>

StudentDatabase::StudentDatabase(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("students.db");
}

StudentDatabase::~StudentDatabase()
{
    closeDatabase();
}

bool StudentDatabase::openDatabase()
{
    if (!db.open()) {
        qDebug() << "Ошибка открытия базы данных: " << db.lastError().text();
        return false;
    }
    return true;
}

void StudentDatabase::closeDatabase()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool StudentDatabase::createTables()
{
    QSqlQuery query;

    QString createStudentsTable = "CREATE TABLE IF NOT EXISTS students ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "name TEXT NOT NULL, "
                                  "group_id INTEGER NOT NULL)";
    if (!query.exec(createStudentsTable)) {
        qDebug() << "Ошибка создания таблицы студентов: " << query.lastError().text();
        return false;
    }

    QString createSubjectsTable = "CREATE TABLE IF NOT EXISTS subjects ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "subject_name TEXT NOT NULL)";
    if (!query.exec(createSubjectsTable)) {
        qDebug() << "Ошибка создания таблицы предметов: " << query.lastError().text();
        return false;
    }

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

QSqlTableModel* StudentDatabase::getStudentsModel()
{
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("students");
    model->select();
    return model;
}

QSqlTableModel* StudentDatabase::getSubjectsModel()
{
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("subjects");
    model->select();
    return model;
}

QSqlTableModel* StudentDatabase::getGradesModel()
{
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("grades");
    model->select();
    return model;
}

QSqlQueryModel* StudentDatabase::getAllDataModel()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);

    QString queryStr = "SELECT students.id, students.name, students.group_id, "
                       "subjects.subject_name, grades.grade "
                       "FROM students "
                       "JOIN grades ON students.id = grades.student_id "
                       "JOIN subjects ON grades.subject_id = subjects.id";

    model->setQuery(queryStr, db);

    if (model->lastError().isValid()) {
        qDebug() << "Ошибка запроса объединённой таблицы:" << model->lastError().text();
    }

    return model;
}
