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

    // Create tableview for Tags
    tagmodel = new TagModel(t->tracer()->tags(), this);
    ui->tagtableview->setModel(tagmodel);
    ui->tagtableview->verticalHeader()->hide();
    ui->tagtableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tagtableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->tagtableview,
            SIGNAL(doubleClicked(QModelIndex)),
            tagmodel,
            SLOT(doubleClicked_slot(QModelIndex)));
    connect(ui->tagtableview->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            tagmodel,
            SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

    // Create tableview for Code
    codemodel = new CodeModel(T::arget().getCode(), T::arget().getCi(), this);
    ui->codetableview->setModel(codemodel);
    ui->codetableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->codetableview->verticalHeader()->hide();
    connect(ui->codetableview->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            codemodel,
            SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
    connect(codemodel,
            SIGNAL(selectionChanged(const addr_t& , const addr_t& )),
            tagmodel,
            SLOT(CodeSelectionChangedSlot(const addr_t& , const addr_t& )));

    // Create tableview for Value Ranges
    vrangemodel = new VRangeModel();
    ui->vrangeTableView->setModel(vrangemodel);
    ui->vrangeTableView->verticalHeader()->hide();
    ui->vrangeTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->vrangeTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    connect(tagmodel,
            SIGNAL(addVRange_signal(Tag*,Val*,Val*)),
            vrangemodel,
            SLOT(addVRange_slot(Tag*,Val*,Val*)));

    //rDelegate = new RDel(this);
    //ui->vrangeTableView->setItemDelegate(rDelegate);
}

MainWindow::~MainWindow()
{
    delete tagmodel;
    delete codemodel;
    delete vrangemodel;
    delete ui;
}

void MainWindow::startFuzz()
{
    // get selected code section
    addr_t from = codemodel->getSelRipFrom();
    addr_t to = codemodel->getSelRipTo();
    // get selected variables and accoring value ranges
    std::vector<const VRange*> vrange;
    vrangemodel->getVrange(vrange);
    // start fuzzing
    fuzzer->fuzz(from, to, vrange);
}
