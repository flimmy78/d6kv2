/****************************************************************************
** Meta object code from reading C++ file 'datacurveview.h'
**
** Created: Fri Apr 7 09:20:15 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../datacurveview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datacurveview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DataCurveView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      35,   15,   14,   14, 0x09,
      89,   72,   14,   14, 0x09,
     129,   14,   14,   14, 0x0a,
     141,   14,   14,   14, 0x0a,
     155,  149,   14,   14, 0x0a,
     198,  181,   14,   14, 0x0a,
     235,  181,   14,   14, 0x0a,
     279,  271,   14,   14, 0x0a,
     315,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DataCurveView[] = {
    "DataCurveView\0\0topLeft,bottomRight\0"
    "dataChanged(QModelIndex,QModelIndex)\0"
    "current,previous\0"
    "currentChanged(QModelIndex,QModelIndex)\0"
    "selectAll()\0reset()\0index\0"
    "setRootIndex(QModelIndex)\0parent,start,end\0"
    "columnsInserted(QModelIndex,int,int)\0"
    "columnsRemoved(QModelIndex,int,int)\0"
    "selpara\0selectorChanged(selectorcurvepara&)\0"
    "mouseMoved(QPoint)\0"
};

void DataCurveView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DataCurveView *_t = static_cast<DataCurveView *>(_o);
        switch (_id) {
        case 0: _t->dataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 1: _t->currentChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 2: _t->selectAll(); break;
        case 3: _t->reset(); break;
        case 4: _t->setRootIndex((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: _t->columnsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: _t->columnsRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: _t->selectorChanged((*reinterpret_cast< selectorcurvepara(*)>(_a[1]))); break;
        case 8: _t->mouseMoved((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DataCurveView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DataCurveView::staticMetaObject = {
    { &QAbstractItemView::staticMetaObject, qt_meta_stringdata_DataCurveView,
      qt_meta_data_DataCurveView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataCurveView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataCurveView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataCurveView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataCurveView))
        return static_cast<void*>(const_cast< DataCurveView*>(this));
    return QAbstractItemView::qt_metacast(_clname);
}

int DataCurveView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
