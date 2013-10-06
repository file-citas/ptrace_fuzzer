#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "target.h"
#include <QDebug>

MainWindow::MainWindow(Fuzzer* t, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fuzzer(t)
{
    ui->setupUi(this);

    tagview = new tabmodel(t->tracer()->tags(), this);
    ui->tagtableview->setModel(tagview);
    ui->tagtableview->verticalHeader()->hide();
    ui->tagtableview->horizontalHeader()->setStretchLastSection(true);
    ui->tagtableview->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tagtableview->setContextMenuPolicy(Qt::CustomContextMenu);
    //ui->tagtableview->setContextMenuPolicy(Qt::ActionsContextMenu);
    //QAction* setVRange = new QAction(tr("Set Value Range"), this);
    //ui->tagtableview->addAction(setVRange);
    connect(ui->tagtableview,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            tagview,
            SLOT(on_actionRightClick_triggered(const QPoint&)));

    connect(tagview->setVRange,
            SIGNAL(triggered()),
            tagview,
            SLOT(on_setVRange_triggered()));

    connect(ui->tagtableview->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            tagview,
            SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

    codeview = new Codemodel(T::arget().getCode(), T::arget().getCi(), this);
    ui->codetableview->setModel(codeview);
    ui->codetableview->horizontalHeader()->setStretchLastSection(true);
    ui->codetableview->verticalHeader()->hide();

    connect(ui->codetableview->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            codeview,
            SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

    connect(codeview,
            SIGNAL(selectionChanged(const addr_t& , const addr_t& )),
            tagview,
            SLOT(CodeSelectionChangedSlot(const addr_t& , const addr_t& )));

    vrangeview = new vrangemodel();
    ui->vrangeTableView->setModel(vrangeview);
    ui->vrangeTableView->verticalHeader()->hide();

    connect(tagview,
            SIGNAL(addVRange_signal(const Tag*,Val,Val)),
            vrangeview,
            SLOT(addVRange_slot(const Tag*,Val,Val)));
}

MainWindow::~MainWindow()
{
    delete tagview;
    delete codeview;
    delete vrangeview;
    delete ui;
}

void MainWindow::startFuzz()
{
    addr_t from = codeview->getSelRipFrom();
    addr_t to = codeview->getSelRipTo();
    std::vector<const VRange*> vrange;
    vrangeview->getVrange(vrange);
    fuzzer->fuzz(from, to, vrange);
}
