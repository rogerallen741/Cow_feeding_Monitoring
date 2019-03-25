/****************************************************************************
** Meta object code from reading C++ file 'cow_feeding_monitoring.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "cow_feeding_monitoring.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cow_feeding_monitoring.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Cow_Feeding_Monitoring_t {
    QByteArrayData data[15];
    char stringdata0[232];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Cow_Feeding_Monitoring_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Cow_Feeding_Monitoring_t qt_meta_stringdata_Cow_Feeding_Monitoring = {
    {
QT_MOC_LITERAL(0, 0, 22), // "Cow_Feeding_Monitoring"
QT_MOC_LITERAL(1, 23, 8), // "showTime"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 22), // "on_Capture_Btn_clicked"
QT_MOC_LITERAL(4, 56, 27), // "on_Fence_par_ok_Btn_clicked"
QT_MOC_LITERAL(5, 84, 22), // "on_StartButton_clicked"
QT_MOC_LITERAL(6, 107, 21), // "on_StopButton_clicked"
QT_MOC_LITERAL(7, 129, 23), // "on_Test_rec_pos_clicked"
QT_MOC_LITERAL(8, 153, 17), // "show_eating_image"
QT_MOC_LITERAL(9, 171, 3), // "str"
QT_MOC_LITERAL(10, 175, 18), // "show_leaving_image"
QT_MOC_LITERAL(11, 194, 12), // "show_console"
QT_MOC_LITERAL(12, 207, 10), // "show_image"
QT_MOC_LITERAL(13, 218, 8), // "img_path"
QT_MOC_LITERAL(14, 227, 4) // "type"

    },
    "Cow_Feeding_Monitoring\0showTime\0\0"
    "on_Capture_Btn_clicked\0"
    "on_Fence_par_ok_Btn_clicked\0"
    "on_StartButton_clicked\0on_StopButton_clicked\0"
    "on_Test_rec_pos_clicked\0show_eating_image\0"
    "str\0show_leaving_image\0show_console\0"
    "show_image\0img_path\0type"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Cow_Feeding_Monitoring[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    1,   70,    2, 0x0a /* Public */,
      10,    1,   73,    2, 0x0a /* Public */,
      11,    1,   76,    2, 0x0a /* Public */,
      12,    2,   79,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   14,

       0        // eod
};

void Cow_Feeding_Monitoring::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Cow_Feeding_Monitoring *_t = static_cast<Cow_Feeding_Monitoring *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showTime(); break;
        case 1: _t->on_Capture_Btn_clicked(); break;
        case 2: _t->on_Fence_par_ok_Btn_clicked(); break;
        case 3: _t->on_StartButton_clicked(); break;
        case 4: _t->on_StopButton_clicked(); break;
        case 5: _t->on_Test_rec_pos_clicked(); break;
        case 6: _t->show_eating_image((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->show_leaving_image((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->show_console((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->show_image((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Cow_Feeding_Monitoring::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Cow_Feeding_Monitoring.data,
      qt_meta_data_Cow_Feeding_Monitoring,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Cow_Feeding_Monitoring::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Cow_Feeding_Monitoring::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Cow_Feeding_Monitoring.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Cow_Feeding_Monitoring::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
