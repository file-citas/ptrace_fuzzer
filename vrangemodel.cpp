#include "vrangemodel.h"
#include <QDebug>

VRangeModel::VRangeModel(QObject *parent) :
	QAbstractTableModel(parent)
{
}

int VRangeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
	return 3;
}

int VRangeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
	return items_.size();
}

QVariant VRangeModel::data(const QModelIndex &index, int role) const
{
	int row = index.row();
	int col = index.column();
	vrangeitem* item = items_[row];
	switch(role) {
		case Qt::DisplayRole:
			{
				if(col==0) {
					return QString("%1-%2").arg(item->addrFrom(),
							8 , 16, QChar('0')).arg(item->addrTo(),
							8 , 16, QChar('0')).toUpper();
				} else if(col==1){
					const unsigned char* val = item->valFrom()->val();
					int len = item->valFrom()->len();
					QString ret;
					for(int i=0; i<len; ++i)
						ret += QString("%1 ").arg(val[i], 2, 16, QChar('0'));
					return ret;
				} else if(col==2) {
					const unsigned char* val = item->valTo()->val();
					int len = item->valTo()->len();
					QString ret;
					for(int i=0; i<len; ++i)
						ret += QString("%1 ").arg(val[i], 2, 16, QChar('0'));
					return ret;
				}
				break;
			}
	}
	return QVariant();
}
Qt::ItemFlags VRangeModel::flags(const QModelIndex & index) const
{
    Q_UNUSED(index);
	return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}
bool VRangeModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (role == Qt::EditRole)
	{
		int row = index.row();
		int col = index.column();
		vrangeitem* item = items_[row];
		// TODO
		QByteArray ba = value.toByteArray();

		if(ba.length() > item->tag()->len())
			item->tag()->len(ba.length());
		if(col==1) {
			item->valFrom()->val(
					(unsigned char*)ba.data(),
					ba.length());
			emit dataChanged(createIndex(row, col, 0),
					createIndex(row, col, 0));
		} else if(col==2) {
			item->valTo()->val(
					(unsigned char*)ba.data(),
					ba.length());
			emit dataChanged(createIndex(row, col, 0),
					createIndex(row, col, 0));
		}
	}
	return true;
}


QVariant VRangeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) {
			switch (section)
			{
				case 0:
					return QString("Address");
				case 1:
					return QString("From");
				case 2:
					return QString("To");
			}
		}
	}
	return QVariant();
}

void VRangeModel::getVrange(std::vector<const VRange*>& vrange) const
{
	for(auto item : items_)
		vrange.push_back(item->vrange());
}

void VRangeModel::addVRange_slot(Tag *tag, Val *from, Val *to)
{
	int row = items_.size();
	beginInsertRows( QModelIndex(), row, row );
	items_.append(new vrangeitem(tag, from, to));
	endInsertRows();
}


vrangeitem::vrangeitem(Tag *tag, Val *from, Val *to) :
	tag_(tag), valFrom_(from), valTo_(to)
{
	vrange_ = new VRange(tag, from, to);
	addrFrom_ = tag->loc();
	addrTo_ = tag->loc()+tag->len();
}
