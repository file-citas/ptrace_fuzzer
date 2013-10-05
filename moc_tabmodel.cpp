/****************************************************************************
** Meta object code from reading C++ file 'tabmodel.h'
**
** Created: Sat Oct 5 02:51:35 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "tabmodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tabmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_tabmodel[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      36,   10,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_tabmodel[] = {
    "tabmodel\0\0newSelection,oldSelection\0"
    "selectionChangedSlot(QItemSelection,QItemSelection)\0"
};

const QMetaObject tabmodel::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_tabmodel,
      qt_meta_data_tabmodel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &tabmodel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *tabmodel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *tabmodel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_tabmodel))
        return static_cast<void*>(const_cast< tabmodel*>(this));
    return QAbstractTableModel::qt_metacast(_clname);
}

int tabmodel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectionChangedSlot((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
