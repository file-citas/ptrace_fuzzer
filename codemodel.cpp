#include "codemodel.h"
#include <stdio.h>

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
    int start_row = 0;
    int stop_row = 0;
    if(!newSelection.indexes().isEmpty()) {
        start_row = newSelection.indexes().first().row();
        stop_row = newSelection.indexes().last().row();
    }
    if(!oldSelection.indexes().isEmpty()) {
        start_row = qMin(start_row, oldSelection.indexes().first().row());
        stop_row = qMax(stop_row, oldSelection.indexes().last().row());
    }

    emit selectionChanged(code_[start_row]->addr, code_[stop_row]->addr);
}

