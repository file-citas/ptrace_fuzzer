#ifndef VRANGEVIEW_H
#define VRANGEVIEW_H

#include <QAbstractTableModel>
#include <QList>
#include <vector>
#include "tag.h"
#include "val.h"
#include "vrange.h"

class vrangeitem {
public:
    vrangeitem(const Tag *tag, const Val &from, const Val &to);
    // TODO destructor
    const VRange* vrange() const { return vrange_;}
    addr_t addrFrom() const { return addrFrom_;}
    addr_t addrTo() const { return addrTo_;}
    Val* valFrom() { return &valFrom_; }
    Val* valTo() { return &valTo_; }
private:
    const VRange* vrange_;
    const Tag* tag_;
    Val valFrom_;
    Val valTo_;
    addr_t addrFrom_;
    addr_t addrTo_;
};

class vrangemodel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit vrangemodel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const ;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void getVrange(std::vector<const VRange*>&) const;

protected:

signals:
    //void editCompleted(const QString &);

public slots:
    void addVRange_slot(const Tag* tag, const Val& from, const Val& to);

private:
    QList<vrangeitem*> items_;
};


#endif // VRANGEVIEW_H
