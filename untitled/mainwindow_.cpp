#include "mainwindow_.h"
#include "ui_mainwindow_.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

MainWindow_::MainWindow_(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow_)
{
    ui->setupUi(this);

    if (db.openDatabase() && db.createTables()) {
        setupUi();
    }
}

MainWindow_::~MainWindow_()
{
    delete ui;
}

void MainWindow_::setupUi()
{
    ui->allDataView->setModel(db.getAllDataModel());
    ui->allDataView->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow_::on_pushButton_2_clicked()
{
    QString name = ui->namestudent->text();
    QString groupText = ui->groupLineEdit->text();
    QString subjectName = ui->nameLineEdit->text();
    QString gradeText = ui->line_grade->text();

    if (name.isEmpty() || subjectName.isEmpty() || gradeText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Имя студента, название предмета и оценка не могут быть пустыми.");
        return;
    }

    bool ok;
    int group = groupText.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Номер группы должен быть числом.");
        return;
    }

    int grade = gradeText.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Оценка должна быть числом.");
        return;
    }

    if (db.addStudent(name, group)) {
        int studentId = db.getStudentsModel()->rowCount();
        if (db.addSubject(subjectName)) {
            int subjectId = db.getSubjectsModel()->rowCount();
            if (db.addGrade(studentId, subjectId, grade)) {
                QMessageBox::information(this, "Успех", "Студент и его оценка успешно добавлены.");
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось добавить оценку.");
            }
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось добавить предмет.");
        }
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить студента.");
    }

    QSqlQueryModel *allDataModel = db.getAllDataModel();
    allDataModel->setQuery("SELECT students.id, students.name, students.group_id, subjects.subject_name, grades.grade "
                           "FROM students "
                           "LEFT JOIN grades ON students.id = grades.student_id "
                           "LEFT JOIN subjects ON grades.subject_id = subjects.id");
    ui->allDataView->setModel(allDataModel);
}


void MainWindow_::on_pushButton_clicked()
{
    QString name = ui->namestudent->text();
    QString groupText = ui->groupLineEdit->text();

    if (name.isEmpty() && groupText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните хотя бы одно поле для поиска.");
        return;
    }

    QString query = "SELECT students.id, students.name, students.group_id, subjects.subject_name, grades.grade "
                    "FROM students "
                    "LEFT JOIN grades ON students.id = grades.student_id "
                    "LEFT JOIN subjects ON grades.subject_id = subjects.id WHERE 1=1";

    if (!name.isEmpty()) {
        query += QString(" AND students.name LIKE '%%1%'").arg(name);
    }
    if (!groupText.isEmpty()) {
        query += QString(" AND students.group_id = %1").arg(groupText.toInt());
    }

    QSqlQueryModel *allDataModel = db.getAllDataModel();
    allDataModel->setQuery(query);

    ui->allDataView->setModel(allDataModel);
}
