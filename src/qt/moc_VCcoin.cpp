/****************************************************************************
** Meta object code from reading C++ file 'VCcoin.h'
**
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qt/VCcoin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VCcoin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VccoinCore_t {
    QByteArrayData data[9];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VccoinCore_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VccoinCore_t qt_meta_stringdata_VccoinCore = {
    {
QT_MOC_LITERAL(0, 0, 10), // "VccoinCore"
QT_MOC_LITERAL(1, 11, 16), // "initializeResult"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 7), // "success"
QT_MOC_LITERAL(4, 37, 14), // "shutdownResult"
QT_MOC_LITERAL(5, 52, 16), // "runawayException"
QT_MOC_LITERAL(6, 69, 7), // "message"
QT_MOC_LITERAL(7, 77, 10), // "initialize"
QT_MOC_LITERAL(8, 88, 8) // "shutdown"

    },
    "VccoinCore\0initializeResult\0\0success\0"
    "shutdownResult\0runawayException\0message\0"
    "initialize\0shutdown"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VccoinCore[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    0,   42,    2, 0x06 /* Public */,
       5,    1,   43,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   46,    2, 0x0a /* Public */,
       8,    0,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VccoinCore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VccoinCore *_t = static_cast<VccoinCore *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->initializeResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->shutdownResult(); break;
        case 2: _t->runawayException((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->initialize(); break;
        case 4: _t->shutdown(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VccoinCore::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VccoinCore::initializeResult)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VccoinCore::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VccoinCore::shutdownResult)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VccoinCore::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VccoinCore::runawayException)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject VccoinCore::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_VccoinCore.data,
      qt_meta_data_VccoinCore,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VccoinCore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VccoinCore::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VccoinCore.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VccoinCore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void VccoinCore::initializeResult(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VccoinCore::shutdownResult()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void VccoinCore::runawayException(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_VccoinApplication_t {
    QByteArrayData data[13];
    char stringdata0[173];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VccoinApplication_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VccoinApplication_t qt_meta_stringdata_VccoinApplication = {
    {
QT_MOC_LITERAL(0, 0, 17), // "VccoinApplication"
QT_MOC_LITERAL(1, 18, 19), // "requestedInitialize"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 17), // "requestedShutdown"
QT_MOC_LITERAL(4, 57, 14), // "splashFinished"
QT_MOC_LITERAL(5, 72, 11), // "windowShown"
QT_MOC_LITERAL(6, 84, 10), // "VccoinGUI*"
QT_MOC_LITERAL(7, 95, 6), // "window"
QT_MOC_LITERAL(8, 102, 16), // "initializeResult"
QT_MOC_LITERAL(9, 119, 7), // "success"
QT_MOC_LITERAL(10, 127, 14), // "shutdownResult"
QT_MOC_LITERAL(11, 142, 22), // "handleRunawayException"
QT_MOC_LITERAL(12, 165, 7) // "message"

    },
    "VccoinApplication\0requestedInitialize\0"
    "\0requestedShutdown\0splashFinished\0"
    "windowShown\0VccoinGUI*\0window\0"
    "initializeResult\0success\0shutdownResult\0"
    "handleRunawayException\0message"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VccoinApplication[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,
       4,    0,   51,    2, 0x06 /* Public */,
       5,    1,   52,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   55,    2, 0x0a /* Public */,
      10,    0,   58,    2, 0x0a /* Public */,
      11,    1,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,

       0        // eod
};

void VccoinApplication::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VccoinApplication *_t = static_cast<VccoinApplication *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestedInitialize(); break;
        case 1: _t->requestedShutdown(); break;
        case 2: _t->splashFinished(); break;
        case 3: _t->windowShown((*reinterpret_cast< VccoinGUI*(*)>(_a[1]))); break;
        case 4: _t->initializeResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->shutdownResult(); break;
        case 6: _t->handleRunawayException((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VccoinApplication::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VccoinApplication::requestedInitialize)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VccoinApplication::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VccoinApplication::requestedShutdown)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VccoinApplication::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VccoinApplication::splashFinished)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (VccoinApplication::*_t)(VccoinGUI * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VccoinApplication::windowShown)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject VccoinApplication::staticMetaObject = {
    { &QApplication::staticMetaObject, qt_meta_stringdata_VccoinApplication.data,
      qt_meta_data_VccoinApplication,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VccoinApplication::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VccoinApplication::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VccoinApplication.stringdata0))
        return static_cast<void*>(this);
    return QApplication::qt_metacast(_clname);
}

int VccoinApplication::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QApplication::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void VccoinApplication::requestedInitialize()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void VccoinApplication::requestedShutdown()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void VccoinApplication::splashFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void VccoinApplication::windowShown(VccoinGUI * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
