#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "target.h"

MainWindow::MainWindow(Tracer* t, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tracer(t)
{
    ui->setupUi(this);

    tagview = new tabmodel(t->tags(), this);
    ui->tagtableview->setModel(tagview);
    ui->tagtableview->verticalHeader()->hide();
    ui->tagtableview->horizontalHeader()->setStretchLastSection(true);
    ui->tagtableview->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->tagtableview->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            tagview,
            SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

    codeview = new Codemodel(T::arget().getCode(), T::arget().getCi(), this);
    ui->codetableview->setModel(codeview);
    ui->codetableview->horizontalHeader()->setStretchLastSection(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
