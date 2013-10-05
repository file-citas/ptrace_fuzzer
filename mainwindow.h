#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tag.h"
#include "tracer.h"
#include "tabmodel.h"
#include "codemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(Tracer* t, QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    Tracer* tracer;
    tabmodel* tagview;
    Codemodel* codeview;
};

#endif // MAINWINDOW_H
