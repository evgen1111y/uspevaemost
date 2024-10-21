#include "mainwindow_.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow_ w;
    w.resize(800, 600);
    w.show();

    return a.exec();
}
