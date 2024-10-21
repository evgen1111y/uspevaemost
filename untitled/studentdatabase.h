#ifndef STUDENTDATABASE_H
#define STUDENTDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

class StudentDatabase : public QObject
{
    Q_OBJECT

public:
    explicit StudentDatabase(QObject *parent = nullptr);
    ~StudentDatabase();

    bool openDatabase();
    void closeDatabase();
    bool createTables();
    bool addStudent(const QString &name, int group);
    bool addSubject(const QString &subjectName);
    bool addGrade(int studentId, int subjectId, int grade);
    QSqlQueryModel* getAllDataModel();
    QSqlTableModel* getStudentsModel();
    QSqlTableModel* getSubjectsModel();
    QSqlTableModel* getGradesModel();



private:
    QSqlDatabase db;
};

#endif // STUDENTDATABASE_H
