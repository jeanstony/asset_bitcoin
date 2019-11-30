/****************************************************************************
** Meta object code from reading C++ file 'assetmodel.h'
**
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qt/assetmodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'assetmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AssetModel_t {
    QByteArrayData data[29];
    char stringdata0[250];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AssetModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AssetModel_t qt_meta_stringdata_AssetModel = {
    {
QT_MOC_LITERAL(0, 0, 10), // "AssetModel"
QT_MOC_LITERAL(1, 11, 11), // "columnCount"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 6), // "parent"
QT_MOC_LITERAL(4, 31, 8), // "rowCount"
QT_MOC_LITERAL(5, 40, 3), // "get"
QT_MOC_LITERAL(6, 44, 3), // "row"
QT_MOC_LITERAL(7, 48, 6), // "append"
QT_MOC_LITERAL(8, 55, 7), // "AssetNo"
QT_MOC_LITERAL(9, 63, 4), // "name"
QT_MOC_LITERAL(10, 68, 7), // "balance"
QT_MOC_LITERAL(11, 76, 19), // "unconfirmed_balance"
QT_MOC_LITERAL(12, 96, 13), // "total_balance"
QT_MOC_LITERAL(13, 110, 8), // "IsLocked"
QT_MOC_LITERAL(14, 119, 12), // "IsMainWallet"
QT_MOC_LITERAL(15, 132, 3), // "set"
QT_MOC_LITERAL(16, 136, 6), // "remove"
QT_MOC_LITERAL(17, 143, 12), // "create_asset"
QT_MOC_LITERAL(18, 156, 4), // "desc"
QT_MOC_LITERAL(19, 161, 5), // "total"
QT_MOC_LITERAL(20, 167, 4), // "coin"
QT_MOC_LITERAL(21, 172, 10), // "lock_asset"
QT_MOC_LITERAL(22, 183, 6), // "IsLock"
QT_MOC_LITERAL(23, 190, 9), // "AssetRole"
QT_MOC_LITERAL(24, 200, 4), // "Name"
QT_MOC_LITERAL(25, 205, 2), // "no"
QT_MOC_LITERAL(26, 208, 7), // "Balance"
QT_MOC_LITERAL(27, 216, 19), // "Unconfirmed_balance"
QT_MOC_LITERAL(28, 236, 13) // "Total_balance"

    },
    "AssetModel\0columnCount\0\0parent\0rowCount\0"
    "get\0row\0append\0AssetNo\0name\0balance\0"
    "unconfirmed_balance\0total_balance\0"
    "IsLocked\0IsMainWallet\0set\0remove\0"
    "create_asset\0desc\0total\0coin\0lock_asset\0"
    "IsLock\0AssetRole\0Name\0no\0Balance\0"
    "Unconfirmed_balance\0Total_balance"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AssetModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       1,  124, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x02 /* Public */,
       1,    0,   67,    2, 0x22 /* Public | MethodCloned */,
       4,    1,   68,    2, 0x02 /* Public */,
       4,    0,   71,    2, 0x22 /* Public | MethodCloned */,
       5,    1,   72,    2, 0x02 /* Public */,
       7,    7,   75,    2, 0x02 /* Public */,
      15,    8,   90,    2, 0x02 /* Public */,
      16,    1,  107,    2, 0x02 /* Public */,
      17,    4,  110,    2, 0x02 /* Public */,
      21,    2,  119,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Int, QMetaType::QModelIndex,    3,
    QMetaType::Int,
    QMetaType::Int, QMetaType::QModelIndex,    2,
    QMetaType::Int,
    QMetaType::QVariantMap, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Bool,    8,    9,   10,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Bool,    6,    8,    9,   10,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    9,   18,   19,   20,
    QMetaType::Int, QMetaType::Int, QMetaType::Bool,    8,   22,

 // enums: name, flags, count, data
      23, 0x0,    7,  128,

 // enum data: key, value
      24, uint(AssetModel::Name),
      25, uint(AssetModel::no),
      26, uint(AssetModel::Balance),
      27, uint(AssetModel::Unconfirmed_balance),
      28, uint(AssetModel::Total_balance),
      22, uint(AssetModel::IsLock),
      14, uint(AssetModel::IsMainWallet),

       0        // eod
};

void AssetModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AssetModel *_t = static_cast<AssetModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->columnCount((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 1: { int _r = _t->columnCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 2: { int _r = _t->rowCount((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 3: { int _r = _t->rowCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 4: { QVariantMap _r = _t->get((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->append((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const bool(*)>(_a[6])),(*reinterpret_cast< const bool(*)>(_a[7]))); break;
        case 6: _t->set((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6])),(*reinterpret_cast< const bool(*)>(_a[7])),(*reinterpret_cast< const bool(*)>(_a[8]))); break;
        case 7: _t->remove((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: { int _r = _t->create_asset((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: { int _r = _t->lock_asset((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject AssetModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_AssetModel.data,
      qt_meta_data_AssetModel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AssetModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AssetModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AssetModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int AssetModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
