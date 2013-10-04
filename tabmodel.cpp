#include "tabmodel.h"
#include "access.h"
#include <stdio.h>

std::map<Tag*, std::vector<int> > tag_idx;
tabmodel::tabmodel(const std::set<Tag*>& t, QObject *parent) :
    QAbstractTableModel(parent)
{
    size = 0;
    double nTags = t.size();
    double hstep = 359/nTags;
    double hue = 0;

    std::map<Tag*, tagitem*> tti;
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

    for(std::map<Tag*, tagitem*>::iterator it=tti.begin();
        it!=tti.end(); ++it) {
        Tag* tag = it->first;
        std::map<addr_t, Trace*> tf = tag->tforw();
        for(std::map<addr_t, Trace*>::iterator jt = tf.begin();
            jt!=tf.end(); ++jt) {
            Tag* trtag = jt->second->tag;
            tagitem* tfitem = tti.find(trtag)->second;
            it->second->addTf(tfitem);
        }
    }
    /*for(std::set<Tag*>::iterator it=t.begin();
        it!=t.end(); ++it) {
        tags.insert(std::pair<int, Tag*>(
                        size,*it));
        QColor c;
        c.setHsv(hue, 100, 255);
        color.insert(std::pair<int, QColor>(size, c));
        hue += hstep;
        std::vector<int> rl;;
        for(int i=0; i<(*it)->len(); ++i)
            rl.push_back(size+i);
        tag_idx.insert(std::pair<Tag*, std::vector<int> >(
                           (*it), rl));
        size += (*it)->len();
    }*/
}

int tabmodel::rowCount(const QModelIndex &parent) const
{
    return size;
}
int tabmodel::columnCount(const QModelIndex &parent) const
{
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
        /*std::map<int,Tag*, greater>::const_iterator it =
                tags.lower_bound(row);
        if(it==tags.end()) return QVariant();
        int diff = row - it->first;
        if(col==0) {
            return QString("%1").arg(
                        it->second->loc()+diff,
                        8 , 16, QChar('0')).toUpper();
        } else {
            //const Access* a = it->second->lastAccess();
            //if(!a) return QVariant();
            const unsigned char* c =
                    it->second->init_val()->val();
            //        a->val()->val();
            if(col==1) {
                return QString("%1").arg(
                            c[diff],
                            2 , 16, QChar('0')).toUpper();
            } else if(col==2) {
                return QString(QChar(c[diff]));
            }
        }*/
        break;
    }
    case Qt::BackgroundRole:
    {
        /*std::map<int, QColor, greater>::const_iterator jt =
                color.lower_bound(row);
        if(jt==color.end()) return QVariant();*/
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
    QModelIndex index = newSelection.indexes()[0];
    int row = index.row();
    std::map<int,tagitem*, greater>::const_iterator it =
            ti.lower_bound(row);
    if(it==ti.end()) return;
    tagitem* ti = it->second;
    std::set<tagitem*>::iterator jt = ti->tf().begin();
    for(; jt!=ti->tf().end(); ++jt) {
        (*jt)->highlight();
    }

    /*std::map<int,Tag*, greater>::const_iterator it =
            tags.lower_bound(row);
    if(it==tags.end()) return;
    Tag* t = it->second;
    //std::vector<int> rl = tag_idx.find(t)->second;
    std::map<addr_t, Trace*> tf = t->tforw();
    std::map<addr_t, Trace*>::const_iterator jt =
            tf.begin();
    for( ; jt!=tf.end(); ++jt) {
        Tag* tt = jt->second->tag;
        std::vector<int> rl = tag_idx.find(tt)->second;
        int r = rl[0];
    }*/

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

void tagitem::addTf(tagitem *ti)
{
    tforw_.insert(ti);
}
