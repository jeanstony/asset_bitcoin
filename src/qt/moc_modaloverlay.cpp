/****************************************************************************
** Meta object code from reading C++ file 'modaloverlay.h'
**
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qt/modaloverlay.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modaloverlay.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ModalOverlay_t {
    QByteArrayData data[15];
    char stringdata0[177];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModalOverlay_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModalOverlay_t qt_meta_stringdata_ModalOverlay = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ModalOverlay"
QT_MOC_LITERAL(1, 13, 13), // "NotifyActived"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 8), // "IsActive"
QT_MOC_LITERAL(4, 37, 9), // "tipUpdate"
QT_MOC_LITERAL(5, 47, 5), // "count"
QT_MOC_LITERAL(6, 53, 9), // "blockDate"
QT_MOC_LITERAL(7, 63, 21), // "nVerificationProgress"
QT_MOC_LITERAL(8, 85, 18), // "setKnownBestHeight"
QT_MOC_LITERAL(9, 104, 16), // "toggleVisibility"
QT_MOC_LITERAL(10, 121, 8), // "showHide"
QT_MOC_LITERAL(11, 130, 4), // "hide"
QT_MOC_LITERAL(12, 135, 13), // "userRequested"
QT_MOC_LITERAL(13, 149, 12), // "closeClicked"
QT_MOC_LITERAL(14, 162, 14) // "isLayerVisible"

    },
    "ModalOverlay\0NotifyActived\0\0IsActive\0"
    "tipUpdate\0count\0blockDate\0"
    "nVerificationProgress\0setKnownBestHeight\0"
    "toggleVisibility\0showHide\0hide\0"
    "userRequested\0closeClicked\0isLayerVisible"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModalOverlay[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    3,   62,    2, 0x0a /* Public */,
       8,    2,   69,    2, 0x0a /* Public */,
       9,    0,   74,    2, 0x0a /* Public */,
      10,    2,   75,    2, 0x0a /* Public */,
      10,    1,   80,    2, 0x2a /* Public | MethodCloned */,
      10,    0,   83,    2, 0x2a /* Public | MethodCloned */,
      13,    0,   84,    2, 0x0a /* Public */,
      14,    0,   85,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QDateTime, QMetaType::Double,    5,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::QDateTime,    5,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,   11,   12,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,

       0        // eod
};

void ModalOverlay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ModalOverlay *_t = static_cast<ModalOverlay *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->NotifyActived((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->tipUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QDateTime(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 2: _t->setKnownBestHeight((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QDateTime(*)>(_a[2]))); break;
        case 3: _t->toggleVisibility(); break;
        case 4: _t->showHide((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->showHide((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->showHide(); break;
        case 7: _t->closeClicked(); break;
        case 8: { bool _r = _t->isLayerVisible();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (ModalOverlay::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModalOverlay::NotifyActived)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ModalOverlay::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ModalOverlay.data,
      qt_meta_data_ModalOverlay,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ModalOverlay::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModalOverlay::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModalOverlay.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ModalOverlay::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ModalOverlay::NotifyActived(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
