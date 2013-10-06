#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tag.h"
#include "fuzzer.h"
#include "tabmodel.h"
#include "codemodel.h"
#include "vrangemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(Fuzzer* t, QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void startFuzz();

private:
    Ui::MainWindow *ui;
    Fuzzer* fuzzer;
    tabmodel* tagview;
    Codemodel* codeview;
    vrangemodel* vrangeview;
};

#endif // MAINWINDOW_H
