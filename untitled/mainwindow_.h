#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include "qtableview.h"
#include "studentdatabase.h"

namespace Ui {
class MainWindow_;
}

class MainWindow_ : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_(QWidget *parent = nullptr);
    ~MainWindow_();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow_ *ui;
    StudentDatabase db;
    void setupUi();
    QSqlQueryModel* allDataModel;
};

#endif // MAINWINDOW_H
