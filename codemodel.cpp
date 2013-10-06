#include "codemodel.h"
#include <stdio.h>
#include <QDebug>

Codemodel::Codemodel(const std::map<addr_t, _DInst*>& code, const _CodeInfo* ci, QObject *parent) :
    QAbstractTableModel(parent),
    codeToRip_(code), ci_(ci)
{
    size_ = codeToRip_.size();
    fprintf(stderr, "%d\n", size_);
    for(auto line : codeToRip_) {
        _DInst* inst = line.second;
        code_.push_back(inst);

        _DecodedInst di;
        distorm_format(ci_, inst, &di);
        strCode_.push_back(QString("%1 %2").arg((char*)di.mnemonic.p, (char*)di.operands.p));
        color_.push_back(QColor(Qt::white));
    }
}

int Codemodel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return size_;
}

int Codemodel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant Codemodel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    switch(role) {
    case Qt::DisplayRole:
    {
        if(col==0) {
            return strCode_[row];
        }
        break;
    }
    case Qt::BackgroundRole:
    {
        return color_[row];
        break;
    }
    }
    return QVariant();
}


QVariant Codemodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("Code");
            }
        }
    }
    return QVariant();
}


void Codemodel::selectionChangedSlot(const QItemSelection &newSelection,
                                     const QItemSelection &oldSelection)
{
    currentSelection.merge(oldSelection, QItemSelectionModel::Deselect);
    currentSelection.merge(newSelection, QItemSelectionModel::Select);
    int start_row = currentSelection.indexes().first().row();
    int stop_row = currentSelection.indexes().last().row();
    //qDebug() << start_row << " " << stop_row;
    emit selectionChanged(code_[start_row]->addr, code_[stop_row]->addr);
}

addr_t Codemodel::getSelRipFrom()  const
{
    int start_row = currentSelection.indexes().first().row();
    return code_[start_row]->addr;
}


addr_t Codemodel::getSelRipTo()  const
{
    int stop_row = currentSelection.indexes().last().row();
    return code_[stop_row]->addr;
}
