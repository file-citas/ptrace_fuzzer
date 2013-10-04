#include <QApplication>
#include "mainwindow.h"
#include "tracer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Tracer* t = new Tracer(argc-1, &argv[1]);
    MainWindow w(t);
    w.show();

    return a.exec();
}
