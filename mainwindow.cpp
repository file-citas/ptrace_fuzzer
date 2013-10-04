#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Tracer* t, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tracer(t)
{
    ui->setupUi(this);

    tm = new tabmodel(t->tags(), this);
    ui->tagtableview->setModel(tm);
    ui->tagtableview->verticalHeader()->hide();
    ui->tagtableview->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->tagtableview->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            tm,
            SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
