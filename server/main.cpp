#include <QApplication>
#include <QTableView>
#include "studentdatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StudentDatabase db;

    // Открытие базы данных
    if (!db.openDatabase()) {
        return -1;
    }

    // Создание всех таблиц (студенты, предметы, оценки)
    if (!db.createTables()) {
        return -1;
    }

    // Добавление студентов
    db.addStudent("Ivan Ivanov", 101);
    db.addStudent("Petr Petrov", 102);

    // Добавление предметов
    db.addSubject("Math");
    db.addSubject("Physics");

    // Добавление оценок
    db.addGrade(1, 1, 5);  // Иван Иванов, предмет Math, оценка 5
    db.addGrade(1, 2, 4);  // Иван Иванов, предмет Physics, оценка 4
    db.addGrade(2, 1, 3);  // Петр Петров, предмет Math, оценка 3

    // Отображение данных (для студентов, предметов и оценок)
    QTableView viewStudents;
    viewStudents.setModel(db.getStudentsModel());
    viewStudents.setWindowTitle("Students");
    viewStudents.show();

    QTableView viewSubjects;
    viewSubjects.setModel(db.getSubjectsModel());
    viewSubjects.setWindowTitle("Subjects");
    viewSubjects.show();

    QTableView viewGrades;
    viewGrades.setModel(db.getGradesModel());
    viewGrades.setWindowTitle("Grades");
    viewGrades.show();

    return a.exec();
}
