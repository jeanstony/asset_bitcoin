/****************************************************************************
** Meta object code from reading C++ file 'VCcoingui.h'
**
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qt/VCcoingui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VCcoingui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VccoinGUI_t {
    QByteArrayData data[71];
    char stringdata0[955];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VccoinGUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VccoinGUI_t qt_meta_stringdata_VccoinGUI = {
    {
QT_MOC_LITERAL(0, 0, 9), // "VccoinGUI"
QT_MOC_LITERAL(1, 10, 11), // "receivedURI"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 3), // "uri"
QT_MOC_LITERAL(4, 27, 12), // "consoleShown"
QT_MOC_LITERAL(5, 40, 11), // "RPCConsole*"
QT_MOC_LITERAL(6, 52, 7), // "console"
QT_MOC_LITERAL(7, 60, 19), // "ModalOverlayActived"
QT_MOC_LITERAL(8, 80, 8), // "IsActive"
QT_MOC_LITERAL(9, 89, 16), // "CoinAssetChanged"
QT_MOC_LITERAL(10, 106, 9), // "CoinAsset"
QT_MOC_LITERAL(11, 116, 2), // "ca"
QT_MOC_LITERAL(12, 119, 9), // "IsRemoved"
QT_MOC_LITERAL(13, 129, 20), // "NotifyMainAssetFound"
QT_MOC_LITERAL(14, 150, 24), // "NotifyAssetStatusChanged"
QT_MOC_LITERAL(15, 175, 7), // "AssetNo"
QT_MOC_LITERAL(16, 183, 17), // "setNumConnections"
QT_MOC_LITERAL(17, 201, 5), // "count"
QT_MOC_LITERAL(18, 207, 16), // "setNetworkActive"
QT_MOC_LITERAL(19, 224, 13), // "networkActive"
QT_MOC_LITERAL(20, 238, 12), // "setNumBlocks"
QT_MOC_LITERAL(21, 251, 9), // "blockDate"
QT_MOC_LITERAL(22, 261, 21), // "nVerificationProgress"
QT_MOC_LITERAL(23, 283, 7), // "headers"
QT_MOC_LITERAL(24, 291, 7), // "message"
QT_MOC_LITERAL(25, 299, 5), // "title"
QT_MOC_LITERAL(26, 305, 5), // "style"
QT_MOC_LITERAL(27, 311, 5), // "bool*"
QT_MOC_LITERAL(28, 317, 3), // "ret"
QT_MOC_LITERAL(29, 321, 16), // "setCurrentWallet"
QT_MOC_LITERAL(30, 338, 12), // "WalletModel*"
QT_MOC_LITERAL(31, 351, 12), // "wallet_model"
QT_MOC_LITERAL(32, 364, 31), // "setCurrentWalletBySelectorIndex"
QT_MOC_LITERAL(33, 396, 5), // "index"
QT_MOC_LITERAL(34, 402, 18), // "updateWalletStatus"
QT_MOC_LITERAL(35, 421, 20), // "handlePaymentRequest"
QT_MOC_LITERAL(36, 442, 18), // "SendCoinsRecipient"
QT_MOC_LITERAL(37, 461, 9), // "recipient"
QT_MOC_LITERAL(38, 471, 19), // "incomingTransaction"
QT_MOC_LITERAL(39, 491, 4), // "date"
QT_MOC_LITERAL(40, 496, 4), // "unit"
QT_MOC_LITERAL(41, 501, 7), // "CAmount"
QT_MOC_LITERAL(42, 509, 6), // "amount"
QT_MOC_LITERAL(43, 516, 4), // "type"
QT_MOC_LITERAL(44, 521, 7), // "address"
QT_MOC_LITERAL(45, 529, 5), // "label"
QT_MOC_LITERAL(46, 535, 10), // "walletName"
QT_MOC_LITERAL(47, 546, 16), // "gotoOverviewPage"
QT_MOC_LITERAL(48, 563, 15), // "gotoHistoryPage"
QT_MOC_LITERAL(49, 579, 20), // "gotoReceiveCoinsPage"
QT_MOC_LITERAL(50, 600, 17), // "gotoSendCoinsPage"
QT_MOC_LITERAL(51, 618, 4), // "addr"
QT_MOC_LITERAL(52, 623, 18), // "gotoSignMessageTab"
QT_MOC_LITERAL(53, 642, 20), // "gotoVerifyMessageTab"
QT_MOC_LITERAL(54, 663, 11), // "openClicked"
QT_MOC_LITERAL(55, 675, 14), // "optionsClicked"
QT_MOC_LITERAL(56, 690, 12), // "aboutClicked"
QT_MOC_LITERAL(57, 703, 15), // "showDebugWindow"
QT_MOC_LITERAL(58, 719, 30), // "showDebugWindowActivateConsole"
QT_MOC_LITERAL(59, 750, 22), // "showHelpMessageClicked"
QT_MOC_LITERAL(60, 773, 17), // "trayIconActivated"
QT_MOC_LITERAL(61, 791, 33), // "QSystemTrayIcon::ActivationRe..."
QT_MOC_LITERAL(62, 825, 6), // "reason"
QT_MOC_LITERAL(63, 832, 21), // "showNormalIfMinimized"
QT_MOC_LITERAL(64, 854, 13), // "fToggleHidden"
QT_MOC_LITERAL(65, 868, 12), // "toggleHidden"
QT_MOC_LITERAL(66, 881, 14), // "detectShutdown"
QT_MOC_LITERAL(67, 896, 12), // "showProgress"
QT_MOC_LITERAL(68, 909, 9), // "nProgress"
QT_MOC_LITERAL(69, 919, 18), // "setTrayIconVisible"
QT_MOC_LITERAL(70, 938, 16) // "showModalOverlay"

    },
    "VccoinGUI\0receivedURI\0\0uri\0consoleShown\0"
    "RPCConsole*\0console\0ModalOverlayActived\0"
    "IsActive\0CoinAssetChanged\0CoinAsset\0"
    "ca\0IsRemoved\0NotifyMainAssetFound\0"
    "NotifyAssetStatusChanged\0AssetNo\0"
    "setNumConnections\0count\0setNetworkActive\0"
    "networkActive\0setNumBlocks\0blockDate\0"
    "nVerificationProgress\0headers\0message\0"
    "title\0style\0bool*\0ret\0setCurrentWallet\0"
    "WalletModel*\0wallet_model\0"
    "setCurrentWalletBySelectorIndex\0index\0"
    "updateWalletStatus\0handlePaymentRequest\0"
    "SendCoinsRecipient\0recipient\0"
    "incomingTransaction\0date\0unit\0CAmount\0"
    "amount\0type\0address\0label\0walletName\0"
    "gotoOverviewPage\0gotoHistoryPage\0"
    "gotoReceiveCoinsPage\0gotoSendCoinsPage\0"
    "addr\0gotoSignMessageTab\0gotoVerifyMessageTab\0"
    "openClicked\0optionsClicked\0aboutClicked\0"
    "showDebugWindow\0showDebugWindowActivateConsole\0"
    "showHelpMessageClicked\0trayIconActivated\0"
    "QSystemTrayIcon::ActivationReason\0"
    "reason\0showNormalIfMinimized\0fToggleHidden\0"
    "toggleHidden\0detectShutdown\0showProgress\0"
    "nProgress\0setTrayIconVisible\0"
    "showModalOverlay"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VccoinGUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      39,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  209,    2, 0x06 /* Public */,
       4,    1,  212,    2, 0x06 /* Public */,
       7,    1,  215,    2, 0x06 /* Public */,
       9,    2,  218,    2, 0x06 /* Public */,
      13,    0,  223,    2, 0x06 /* Public */,
      14,    1,  224,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    1,  227,    2, 0x0a /* Public */,
      18,    1,  230,    2, 0x0a /* Public */,
      20,    4,  233,    2, 0x0a /* Public */,
      24,    4,  242,    2, 0x0a /* Public */,
      24,    3,  251,    2, 0x2a /* Public | MethodCloned */,
      29,    1,  258,    2, 0x0a /* Public */,
      32,    1,  261,    2, 0x0a /* Public */,
      34,    0,  264,    2, 0x0a /* Public */,
      35,    1,  265,    2, 0x0a /* Public */,
      38,    8,  268,    2, 0x0a /* Public */,
      47,    0,  285,    2, 0x0a /* Public */,
      48,    0,  286,    2, 0x0a /* Public */,
      49,    0,  287,    2, 0x0a /* Public */,
      50,    1,  288,    2, 0x0a /* Public */,
      50,    0,  291,    2, 0x2a /* Public | MethodCloned */,
      52,    1,  292,    2, 0x0a /* Public */,
      52,    0,  295,    2, 0x2a /* Public | MethodCloned */,
      53,    1,  296,    2, 0x0a /* Public */,
      53,    0,  299,    2, 0x2a /* Public | MethodCloned */,
      54,    0,  300,    2, 0x0a /* Public */,
      55,    0,  301,    2, 0x0a /* Public */,
      56,    0,  302,    2, 0x0a /* Public */,
      57,    0,  303,    2, 0x0a /* Public */,
      58,    0,  304,    2, 0x0a /* Public */,
      59,    0,  305,    2, 0x0a /* Public */,
      60,    1,  306,    2, 0x0a /* Public */,
      63,    0,  309,    2, 0x0a /* Public */,
      63,    1,  310,    2, 0x0a /* Public */,
      65,    0,  313,    2, 0x0a /* Public */,
      66,    0,  314,    2, 0x0a /* Public */,
      67,    2,  315,    2, 0x0a /* Public */,
      69,    1,  320,    2, 0x0a /* Public */,
      70,    0,  323,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Bool,   11,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   15,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void, QMetaType::Int, QMetaType::QDateTime, QMetaType::Double, QMetaType::Bool,   17,   21,   22,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::UInt, 0x80000000 | 27,   25,   24,   26,   28,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::UInt,   25,   24,   26,
    QMetaType::Void, 0x80000000 | 30,   31,
    QMetaType::Void, QMetaType::Int,   33,
    QMetaType::Void,
    QMetaType::Bool, 0x80000000 | 36,   37,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, 0x80000000 | 41, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,   39,   40,   42,   43,   44,   45,   46,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   51,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   51,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   51,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 61,   62,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   64,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   25,   68,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,

       0        // eod
};

void VccoinGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VccoinGUI *_t = static_cast<VccoinGUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->receivedURI((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->consoleShown((*reinterpret_cast< RPCConsole*(*)>(_a[1]))); break;
        case 2: _t->ModalOverlayActived((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->CoinAssetChanged((*reinterpret_cast< const CoinAsset(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->NotifyMainAssetFound(); break;
        case 5: _t->NotifyAssetStatusChanged((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 6: _t->setNumConnections((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setNetworkActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->setNumBlocks((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QDateTime(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 9: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3])),(*reinterpret_cast< bool*(*)>(_a[4]))); break;
        case 10: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 11: _t->setCurrentWallet((*reinterpret_cast< WalletModel*(*)>(_a[1]))); break;
        case 12: _t->setCurrentWalletBySelectorIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->updateWalletStatus(); break;
        case 14: { bool _r = _t->handlePaymentRequest((*reinterpret_cast< const SendCoinsRecipient(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 15: _t->incomingTransaction((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const CAmount(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6])),(*reinterpret_cast< const QString(*)>(_a[7])),(*reinterpret_cast< const int(*)>(_a[8]))); break;
        case 16: _t->gotoOverviewPage(); break;
        case 17: _t->gotoHistoryPage(); break;
        case 18: _t->gotoReceiveCoinsPage(); break;
        case 19: _t->gotoSendCoinsPage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->gotoSendCoinsPage(); break;
        case 21: _t->gotoSignMessageTab((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 22: _t->gotoSignMessageTab(); break;
        case 23: _t->gotoVerifyMessageTab((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 24: _t->gotoVerifyMessageTab(); break;
        case 25: _t->openClicked(); break;
        case 26: _t->optionsClicked(); break;
        case 27: _t->aboutClicked(); break;
        case 28: _t->showDebugWindow(); break;
        case 29: _t->showDebugWindowActivateConsole(); break;
        case 30: _t->showHelpMessageClicked(); break;
        case 31: _t->trayIconActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 32: _t->showNormalIfMinimized(); break;
        case 33: _t->showNormalIfMinimized((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 34: _t->toggleHidden(); break;
        case 35: _t->detectShutdown(); break;
        case 36: _t->showProgress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 37: _t->setTrayIconVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 38: _t->showModalOverlay(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VccoinGUI::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VccoinGUI::receivedURI)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VccoinGUI::*_t)(RPCConsole * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VccoinGUI::consoleShown)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VccoinGUI::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VccoinGUI::ModalOverlayActived)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (VccoinGUI::*_t)(const CoinAsset , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VccoinGUI::CoinAssetChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (VccoinGUI::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VccoinGUI::NotifyMainAssetFound)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (VccoinGUI::*_t)(const int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VccoinGUI::NotifyAssetStatusChanged)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject VccoinGUI::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_VccoinGUI.data,
      qt_meta_data_VccoinGUI,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VccoinGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VccoinGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VccoinGUI.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int VccoinGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 39)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 39;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 39)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 39;
    }
    return _id;
}

// SIGNAL 0
void VccoinGUI::receivedURI(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VccoinGUI::consoleShown(RPCConsole * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VccoinGUI::ModalOverlayActived(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void VccoinGUI::CoinAssetChanged(const CoinAsset _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void VccoinGUI::NotifyMainAssetFound()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void VccoinGUI::NotifyAssetStatusChanged(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
struct qt_meta_stringdata_UnitDisplayStatusBarControl_t {
    QByteArrayData data[7];
    char stringdata0[88];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UnitDisplayStatusBarControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UnitDisplayStatusBarControl_t qt_meta_stringdata_UnitDisplayStatusBarControl = {
    {
QT_MOC_LITERAL(0, 0, 27), // "UnitDisplayStatusBarControl"
QT_MOC_LITERAL(1, 28, 17), // "updateDisplayUnit"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 8), // "newUnits"
QT_MOC_LITERAL(4, 56, 15), // "onMenuSelection"
QT_MOC_LITERAL(5, 72, 8), // "QAction*"
QT_MOC_LITERAL(6, 81, 6) // "action"

    },
    "UnitDisplayStatusBarControl\0"
    "updateDisplayUnit\0\0newUnits\0onMenuSelection\0"
    "QAction*\0action"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UnitDisplayStatusBarControl[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x08 /* Private */,
       4,    1,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void UnitDisplayStatusBarControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UnitDisplayStatusBarControl *_t = static_cast<UnitDisplayStatusBarControl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateDisplayUnit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onMenuSelection((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject UnitDisplayStatusBarControl::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_UnitDisplayStatusBarControl.data,
      qt_meta_data_UnitDisplayStatusBarControl,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *UnitDisplayStatusBarControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UnitDisplayStatusBarControl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UnitDisplayStatusBarControl.stringdata0))
        return static_cast<void*>(this);
    return QLabel::qt_metacast(_clname);
}

int UnitDisplayStatusBarControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
