/****************************************************************************
** Meta object code from reading C++ file 'coinuint_treemodel.h'
**
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qt/coinuint_treemodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'coinuint_treemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CoinUintTreeModel_t {
    QByteArrayData data[12];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CoinUintTreeModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CoinUintTreeModel_t qt_meta_stringdata_CoinUintTreeModel = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CoinUintTreeModel"
QT_MOC_LITERAL(1, 18, 4), // "data"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 5), // "index"
QT_MOC_LITERAL(4, 30, 4), // "role"
QT_MOC_LITERAL(5, 35, 5), // "match"
QT_MOC_LITERAL(6, 41, 15), // "QModelIndexList"
QT_MOC_LITERAL(7, 57, 5), // "start"
QT_MOC_LITERAL(8, 63, 5), // "value"
QT_MOC_LITERAL(9, 69, 4), // "hits"
QT_MOC_LITERAL(10, 74, 14), // "Qt::MatchFlags"
QT_MOC_LITERAL(11, 89, 5) // "flags"

    },
    "CoinUintTreeModel\0data\0\0index\0role\0"
    "match\0QModelIndexList\0start\0value\0"
    "hits\0Qt::MatchFlags\0flags"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CoinUintTreeModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x02 /* Public */,
       5,    5,   39,    2, 0x02 /* Public */,
       5,    4,   50,    2, 0x22 /* Public | MethodCloned */,
       5,    3,   59,    2, 0x22 /* Public | MethodCloned */,

 // methods: parameters
    QMetaType::QVariant, QMetaType::QModelIndex, QMetaType::Int,    3,    4,
    0x80000000 | 6, QMetaType::QModelIndex, QMetaType::Int, QMetaType::QVariant, QMetaType::Int, 0x80000000 | 10,    7,    4,    8,    9,   11,
    0x80000000 | 6, QMetaType::QModelIndex, QMetaType::Int, QMetaType::QVariant, QMetaType::Int,    7,    4,    8,    9,
    0x80000000 | 6, QMetaType::QModelIndex, QMetaType::Int, QMetaType::QVariant,    7,    4,    8,

       0        // eod
};

void CoinUintTreeModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CoinUintTreeModel *_t = static_cast<CoinUintTreeModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { QVariant _r = _t->data((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = std::move(_r); }  break;
        case 1: { QModelIndexList _r = _t->match((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< Qt::MatchFlags(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< QModelIndexList*>(_a[0]) = std::move(_r); }  break;
        case 2: { QModelIndexList _r = _t->match((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QModelIndexList*>(_a[0]) = std::move(_r); }  break;
        case 3: { QModelIndexList _r = _t->match((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QModelIndexList*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject CoinUintTreeModel::staticMetaObject = {
    { &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_CoinUintTreeModel.data,
      qt_meta_data_CoinUintTreeModel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CoinUintTreeModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CoinUintTreeModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CoinUintTreeModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractItemModel::qt_metacast(_clname);
}

int CoinUintTreeModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
