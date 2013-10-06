#ifndef CODEMODEL_H
#define CODEMODEL_H

#include <QAbstractTableModel>
#include <QItemSelection>
#include <QColor>
#include <QBrush>
#include <map>
#include <vector>
#include "types.h"
#include "distorm.h"

class Codemodel : public QAbstractTableModel
{
    Q_OBJECT
public:
    Codemodel(const std::map<addr_t, _DInst*>& code,  const _CodeInfo* ci , QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:
    void selectionChanged(const addr_t& from, const addr_t& to);
    
public slots:

    void selectionChangedSlot(const QItemSelection &newSelection,
                              const QItemSelection &oldSelection);


private:
    std::vector<_DInst*> code_;
    std::vector<QString> strCode_;
    std::vector<QColor> color_;
    std::map<addr_t, _DInst*> codeToRip_;
    QItemSelection currentSelection;
    const _CodeInfo* ci_;
    int size_;
};

#endif // TABMODEL_H
