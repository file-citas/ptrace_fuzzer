#ifndef TABMODEL_H
#define TABMODEL_H

#include <QAbstractTableModel>
#include <QItemSelection>
#include <QColor>
#include <QBrush>
#include <map>
#include <set>
#include "tag.h"

class greater {
    public:
        bool operator()(const int& x,const int& y) const {
            return (x-y)>0; }
};

class tagitem
{
public:
    tagitem(Tag* t, double hue, QModelIndexList indices);
    ~tagitem();
    const Tag* tag() const {return tag_;}
    const QColor* color() const { return color_;}
    void highlight() {highlight_ =true; color_->setHsv(hue_, 128, 255);}
    void lowlight() {highlight_ =false; color_->setHsv(hue_, 32, 255);}
    const QModelIndexList& indices() const { return indices_;}
    //std::set<tagitem*> tf() { return tforw_;}
    //void addTf(tagitem* ti);
    //void addTb(tagitem* tb);
private:
    Tag* tag_;
    //std::set<tagitem*> tforw_;
    //std::set<tagitem*> tbackw_;
    QColor* color_;
    QModelIndexList indices_;
    double hue_;
    bool highlight_;
};

class tabmodel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit tabmodel(const std::set<Tag*>& t, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:
    
public slots:

    void selectionChangedSlot(const QItemSelection &newSelection,
                              const QItemSelection &oldSelection);
    void CodeSelectionChangedSlot(const addr_t& , const addr_t&);


private:
    //std::map<int, Tag*, greater> tags;
    //std::map<int, QColor, greater> color;
    std::map<Tag*, tagitem*> tti;
    std::map<int, tagitem*, greater> ti;
    int size;
    addr_t rip;
};

#endif // TABMODEL_H
