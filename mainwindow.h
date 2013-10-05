#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tag.h"
#include "fuzzer.h"
//#include "fuzzer.h"
#include "tabmodel.h"
#include "codemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(Fuzzer* t, QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    Fuzzer* fuzzer;
    tabmodel* tagview;
    Codemodel* codeview;
};

#endif // MAINWINDOW_H
