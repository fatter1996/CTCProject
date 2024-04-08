/****************************************************************************
** Meta object code from reading C++ file 'sockettcp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../SocketObject/sockettcp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sockettcp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Socket__SocketTCP_t {
    QByteArrayData data[13];
    char stringdata0[142];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Socket__SocketTCP_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Socket__SocketTCP_t qt_meta_stringdata_Socket__SocketTCP = {
    {
QT_MOC_LITERAL(0, 0, 17), // "Socket::SocketTCP"
QT_MOC_LITERAL(1, 18, 8), // "recvData"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 9), // "dataArray"
QT_MOC_LITERAL(4, 38, 12), // "QHostAddress"
QT_MOC_LITERAL(5, 51, 8), // "hAddress"
QT_MOC_LITERAL(6, 60, 5), // "nPort"
QT_MOC_LITERAL(7, 66, 12), // "disconnected"
QT_MOC_LITERAL(8, 79, 9), // "connected"
QT_MOC_LITERAL(9, 89, 11), // "onConnected"
QT_MOC_LITERAL(10, 101, 14), // "onDisconnected"
QT_MOC_LITERAL(11, 116, 10), // "onRecvData"
QT_MOC_LITERAL(12, 127, 14) // "onSendDataSlot"

    },
    "Socket::SocketTCP\0recvData\0\0dataArray\0"
    "QHostAddress\0hAddress\0nPort\0disconnected\0"
    "connected\0onConnected\0onDisconnected\0"
    "onRecvData\0onSendDataSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Socket__SocketTCP[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   49,    2, 0x06 /* Public */,
       7,    2,   56,    2, 0x06 /* Public */,
       8,    2,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   66,    2, 0x0a /* Public */,
      10,    0,   67,    2, 0x0a /* Public */,
      11,    0,   68,    2, 0x0a /* Public */,
      12,    1,   69,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 4, QMetaType::UShort,    3,    5,    6,
    QMetaType::Void, 0x80000000 | 4, QMetaType::UShort,    5,    6,
    QMetaType::Void, 0x80000000 | 4, QMetaType::UShort,    5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    3,

       0        // eod
};

void Socket::SocketTCP::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SocketTCP *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->recvData((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QHostAddress(*)>(_a[2])),(*reinterpret_cast< const quint16(*)>(_a[3]))); break;
        case 1: _t->disconnected((*reinterpret_cast< const QHostAddress(*)>(_a[1])),(*reinterpret_cast< const quint16(*)>(_a[2]))); break;
        case 2: _t->connected((*reinterpret_cast< const QHostAddress(*)>(_a[1])),(*reinterpret_cast< const quint16(*)>(_a[2]))); break;
        case 3: _t->onConnected(); break;
        case 4: _t->onDisconnected(); break;
        case 5: _t->onRecvData(); break;
        case 6: _t->onSendDataSlot((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SocketTCP::*)(const QByteArray & , const QHostAddress & , const quint16 & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketTCP::recvData)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SocketTCP::*)(const QHostAddress & , const quint16 & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketTCP::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SocketTCP::*)(const QHostAddress & , const quint16 & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketTCP::connected)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Socket::SocketTCP::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Socket__SocketTCP.data,
    qt_meta_data_Socket__SocketTCP,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Socket::SocketTCP::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Socket::SocketTCP::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Socket__SocketTCP.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Socket::SocketTCP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void Socket::SocketTCP::recvData(const QByteArray & _t1, const QHostAddress & _t2, const quint16 & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Socket::SocketTCP::disconnected(const QHostAddress & _t1, const quint16 & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Socket::SocketTCP::connected(const QHostAddress & _t1, const quint16 & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
