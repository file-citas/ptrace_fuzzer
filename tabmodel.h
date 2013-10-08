#ifndef TABMODEL_H
#define TABMODEL_H

#include <QAbstractTableModel>
#include <QItemSelection>
#include <QColor>
#include <QBrush>
#include <QAction>
#include <QContextMenuEvent>
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
    Tag* tag() {return tag_;}
    const QColor* color() const { return color_;}
    void highlight() {highlight_ =true; color_->setHsv(hue_, 128, 255);}
    void lowlight() {highlight_ =false; color_->setHsv(hue_, 32, 255);}
    const QModelIndexList& indices() const { return indices_;}
private:
    Tag* tag_;
    QColor* color_;
    QModelIndexList indices_;
    double hue_;
    bool highlight_;
};

class TagModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TagModel(const std::set<Tag*>& t, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QAction* setVRange;

protected:

signals:
    void addVRange_signal(Tag* tag, Val* from, Val* to);
    
public slots:
    void doubleClicked_slot(QModelIndex);
    void selectionChangedSlot(const QItemSelection &newSelection,
                              const QItemSelection &oldSelection);
    void CodeSelectionChangedSlot(const addr_t& , const addr_t&);


private:
    std::map<Tag*, tagitem*> tagToTagitem_;
    std::map<int, tagitem*, greater> indexToTagitem_;
    int size_;
    QItemSelection currentSelection_;
};

#endif // TABMODEL_H
