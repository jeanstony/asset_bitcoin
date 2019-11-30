/****************************************************************************
** Meta object code from reading C++ file 'walletframe.h'
**
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qt/walletframe.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'walletframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WalletFrame_t {
    QByteArrayData data[21];
    char stringdata0[342];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WalletFrame_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WalletFrame_t qt_meta_stringdata_WalletFrame = {
    {
QT_MOC_LITERAL(0, 0, 11), // "WalletFrame"
QT_MOC_LITERAL(1, 12, 24), // "requestedSyncWarningInfo"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 25), // "NotifyModalOverlayActived"
QT_MOC_LITERAL(4, 64, 8), // "IsActive"
QT_MOC_LITERAL(5, 73, 16), // "gotoOverviewPage"
QT_MOC_LITERAL(6, 90, 15), // "gotoHistoryPage"
QT_MOC_LITERAL(7, 106, 20), // "gotoReceiveCoinsPage"
QT_MOC_LITERAL(8, 127, 17), // "gotoSendCoinsPage"
QT_MOC_LITERAL(9, 145, 4), // "addr"
QT_MOC_LITERAL(10, 150, 18), // "gotoSignMessageTab"
QT_MOC_LITERAL(11, 169, 20), // "gotoVerifyMessageTab"
QT_MOC_LITERAL(12, 190, 13), // "encryptWallet"
QT_MOC_LITERAL(13, 204, 6), // "status"
QT_MOC_LITERAL(14, 211, 12), // "backupWallet"
QT_MOC_LITERAL(15, 224, 16), // "changePassphrase"
QT_MOC_LITERAL(16, 241, 12), // "unlockWallet"
QT_MOC_LITERAL(17, 254, 20), // "usedSendingAddresses"
QT_MOC_LITERAL(18, 275, 22), // "usedReceivingAddresses"
QT_MOC_LITERAL(19, 298, 23), // "outOfSyncWarningClicked"
QT_MOC_LITERAL(20, 322, 19) // "ModalOverlayActived"

    },
    "WalletFrame\0requestedSyncWarningInfo\0"
    "\0NotifyModalOverlayActived\0IsActive\0"
    "gotoOverviewPage\0gotoHistoryPage\0"
    "gotoReceiveCoinsPage\0gotoSendCoinsPage\0"
    "addr\0gotoSignMessageTab\0gotoVerifyMessageTab\0"
    "encryptWallet\0status\0backupWallet\0"
    "changePassphrase\0unlockWallet\0"
    "usedSendingAddresses\0usedReceivingAddresses\0"
    "outOfSyncWarningClicked\0ModalOverlayActived"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WalletFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x06 /* Public */,
       3,    1,  110,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,  113,    2, 0x0a /* Public */,
       6,    0,  114,    2, 0x0a /* Public */,
       7,    0,  115,    2, 0x0a /* Public */,
       8,    1,  116,    2, 0x0a /* Public */,
       8,    0,  119,    2, 0x2a /* Public | MethodCloned */,
      10,    1,  120,    2, 0x0a /* Public */,
      10,    0,  123,    2, 0x2a /* Public | MethodCloned */,
      11,    1,  124,    2, 0x0a /* Public */,
      11,    0,  127,    2, 0x2a /* Public | MethodCloned */,
      12,    1,  128,    2, 0x0a /* Public */,
      14,    0,  131,    2, 0x0a /* Public */,
      15,    0,  132,    2, 0x0a /* Public */,
      16,    0,  133,    2, 0x0a /* Public */,
      17,    0,  134,    2, 0x0a /* Public */,
      18,    0,  135,    2, 0x0a /* Public */,
      19,    0,  136,    2, 0x0a /* Public */,
      20,    1,  137,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,

       0        // eod
};

void WalletFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WalletFrame *_t = static_cast<WalletFrame *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestedSyncWarningInfo(); break;
        case 1: _t->NotifyModalOverlayActived((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->gotoOverviewPage(); break;
        case 3: _t->gotoHistoryPage(); break;
        case 4: _t->gotoReceiveCoinsPage(); break;
        case 5: _t->gotoSendCoinsPage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->gotoSendCoinsPage(); break;
        case 7: _t->gotoSignMessageTab((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->gotoSignMessageTab(); break;
        case 9: _t->gotoVerifyMessageTab((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->gotoVerifyMessageTab(); break;
        case 11: _t->encryptWallet((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->backupWallet(); break;
        case 13: _t->changePassphrase(); break;
        case 14: _t->unlockWallet(); break;
        case 15: _t->usedSendingAddresses(); break;
        case 16: _t->usedReceivingAddresses(); break;
        case 17: _t->outOfSyncWarningClicked(); break;
        case 18: _t->ModalOverlayActived((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (WalletFrame::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WalletFrame::requestedSyncWarningInfo)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (WalletFrame::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WalletFrame::NotifyModalOverlayActived)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject WalletFrame::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_WalletFrame.data,
      qt_meta_data_WalletFrame,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *WalletFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WalletFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WalletFrame.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int WalletFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void WalletFrame::requestedSyncWarningInfo()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void WalletFrame::NotifyModalOverlayActived(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
