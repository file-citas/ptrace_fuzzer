#include <QApplication>
#include "mainwindow.h"
//#include "fuzzer.h"
//#include "target.h"
#include "fuzzer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Fuzzer* fuzzer = new Fuzzer(argc-1, &argv[1]);
    MainWindow w(fuzzer);
    w.show();

    return a.exec();
}
