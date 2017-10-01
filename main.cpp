#include <QApplication>
#include "mainwindow.h"

Options opts;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    opts.Initialize();
    MainWindow w;
    w.show();

    return a.exec();
}
