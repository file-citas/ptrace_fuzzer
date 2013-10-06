#include "tabmodel.h"
#include "access.h"
#include <stdio.h>
#include <QDebug>
#include <QMenu>

std::map<Tag*, std::vector<int> > tag_idx;
tabmodel::tabmodel(const std::set<Tag*>& t, QObject *parent) :
    QAbstractTableModel(parent)
{
    size = 0;
    double nTags = t.size();
    double hstep = 359/nTags;
    double hue = 0;

    for(std::set<Tag*>::iterator it=t.begin();
        it!=t.end(); ++it) {
        QModelIndexList indices;
        for(int i=0; i<(*it)->len(); ++i)
            indices << createIndex(size+i,0,0);

        tagitem* item = new tagitem(*it, hue, indices);
        ti.insert(std::pair<int, tagitem*>(size, item));
        tti.insert(std::pair<Tag*, tagitem*>(
                       (*it), item));
        hue += hstep;
        size += (*it)->len();
    }

    setVRange = new QAction(tr("Set Value Range"), this);
}

int tabmodel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return size;
}

int tabmodel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant tabmodel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    std::map<int,tagitem*, greater>::const_iterator it =
            ti.lower_bound(row);
    if(it==ti.end()) return QVariant();
    switch(role) {
    case Qt::DisplayRole:
    {
        int diff = row - it->first;
        if(col==0) {
            return QString("%1").arg(
                        it->second->tag()->loc()+diff,
                        8 , 16, QChar('0')).toUpper();
        } else {
            const unsigned char* c =
                    it->second->tag()->init_val()->val();
            if(col==1) {
                return QString("%1").arg(
                            c[diff],
                            2 , 16, QChar('0')).toUpper();
            } else if(col==2) {
                return QString(QChar(c[diff]));
            }
        }
        break;
    }
    case Qt::BackgroundRole:
    {
        QBrush bg(*it->second->color());
        return bg;
        break;
    }
    }
    return QVariant();
}


QVariant tabmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("Address");
            case 1:
                return QString("H Value");
            case 2:
                return QString("C Value");
            }
        }
    }
    return QVariant();
}

void tabmodel::selectionChangedSlot(const QItemSelection &newSelection,
                                    const QItemSelection &oldSelection)
{
    currentSelection.merge(oldSelection, QItemSelectionModel::Deselect);
    currentSelection.merge(newSelection, QItemSelectionModel::Select);

    std::map<int,tagitem*, greater>::const_iterator kt = ti.begin();
    for( ; kt!=ti.end(); ++kt)
        kt->second->lowlight();
    emit dataChanged(createIndex(0, 0, 0),
                     createIndex(rowCount(), columnCount(), 0));

    if(currentSelection.indexes().isEmpty()) return;

    QModelIndex index = currentSelection.indexes()[0];
    int row = index.row();
    std::map<int,tagitem*, greater>::const_iterator it =
            ti.lower_bound(row);
    if(it==ti.end()) return;
    const Tag* tag = it->second->tag();
    std::map<addr_t, Trace*> tf = tag->tforw();
    std::map<addr_t, Trace*>::const_iterator jt = tf.begin();
    for( ; jt!=tf.end(); ++jt) {
        tagitem* item = tti.find(jt->second->tag)->second;
        item->highlight();
        emit dataChanged(createIndex(item->indices().first().row(), 0, 0),
                         createIndex(item->indices().last().row(), columnCount(), 0));
    }
}

void tabmodel::CodeSelectionChangedSlot(const addr_t& start, const addr_t& stop)
{
    std::map<int,tagitem*, greater>::const_iterator kt = ti.begin();
    for( ; kt!=ti.end(); ++kt)
        kt->second->lowlight();
    emit dataChanged(createIndex(0, 0, 0),
                     createIndex(rowCount(), columnCount(), 0));

    for(addr_t addr=start; addr<=stop; ++addr) {
        for( auto tagToItem : tti) {
            const Tag* tag = tagToItem.first;
            tagitem* tagi = tagToItem.second;
            if(tag->rip_access().find(addr) != tag->rip_access().end()) {
                tagi->highlight();
            }
        }
    }

}

void tabmodel::on_actionRightClick_triggered(const QPoint& p)
{
    QMenu menu;
    menu.addAction(setVRange);
    menu.exec(QCursor::pos());
}

void tabmodel::on_setVRange_triggered()
{
    //if(!setVRange->isChecked()) return;
    if(currentSelection.indexes().isEmpty()) return;

    QModelIndex index = currentSelection.indexes()[0];
    int row = index.row();
    std::map<int,tagitem*, greater>::const_iterator it =
            ti.lower_bound(row);
    if(it==ti.end()) return;
    const Tag* tag = it->second->tag();
    unsigned char* val = new unsigned char[tag->len()];
    memset(val, 0, tag->len());
    emit addVRange_signal(tag,
                          Val(tag->loc(), tag->len()),
                          Val(tag->loc(), tag->len()));

}

tagitem::tagitem(Tag* t, double hue, QModelIndexList indices) :
    tag_(t), indices_(indices), hue_(hue), highlight_(false)
{
    color_ = new QColor();
    color_->setHsv(hue_, 32, 255);
}
tagitem::~tagitem()
{
    // todo
}
