/****************************************************************************
** Meta object code from reading C++ file 'QSofaRecorder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../ext/sofa/applications/sofa/gui/qt/QSofaRecorder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QSofaRecorder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sofa__gui__qt__QSofaRecorder_t {
    QByteArrayData data[17];
    char stringdata[251];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__QSofaRecorder_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__QSofaRecorder_t qt_meta_stringdata_sofa__gui__qt__QSofaRecorder = {
    {
QT_MOC_LITERAL(0, 0, 28), // "sofa::gui::qt::QSofaRecorder"
QT_MOC_LITERAL(1, 29, 16), // "RecordSimulation"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 7), // "NewTime"
QT_MOC_LITERAL(4, 55, 10), // "TimerStart"
QT_MOC_LITERAL(5, 66, 21), // "slot_recordSimulation"
QT_MOC_LITERAL(6, 88, 13), // "slot_backward"
QT_MOC_LITERAL(7, 102, 17), // "slot_stepbackward"
QT_MOC_LITERAL(8, 120, 16), // "slot_playforward"
QT_MOC_LITERAL(9, 137, 16), // "slot_stepforward"
QT_MOC_LITERAL(10, 154, 12), // "slot_forward"
QT_MOC_LITERAL(11, 167, 25), // "slot_loadrecord_timevalue"
QT_MOC_LITERAL(12, 193, 10), // "updateTime"
QT_MOC_LITERAL(13, 204, 16), // "slot_sliderValue"
QT_MOC_LITERAL(14, 221, 5), // "value"
QT_MOC_LITERAL(15, 227, 14), // "loadSimulation"
QT_MOC_LITERAL(16, 242, 8) // "one_step"

    },
    "sofa::gui::qt::QSofaRecorder\0"
    "RecordSimulation\0\0NewTime\0TimerStart\0"
    "slot_recordSimulation\0slot_backward\0"
    "slot_stepbackward\0slot_playforward\0"
    "slot_stepforward\0slot_forward\0"
    "slot_loadrecord_timevalue\0updateTime\0"
    "slot_sliderValue\0value\0loadSimulation\0"
    "one_step"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__QSofaRecorder[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       3,    0,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   93,    2, 0x0a /* Public */,
       5,    1,   96,    2, 0x09 /* Protected */,
       6,    0,   99,    2, 0x09 /* Protected */,
       7,    0,  100,    2, 0x09 /* Protected */,
       8,    0,  101,    2, 0x09 /* Protected */,
       9,    0,  102,    2, 0x09 /* Protected */,
      10,    0,  103,    2, 0x09 /* Protected */,
      11,    1,  104,    2, 0x09 /* Protected */,
      11,    0,  107,    2, 0x29 /* Protected | MethodCloned */,
      13,    2,  108,    2, 0x09 /* Protected */,
      13,    1,  113,    2, 0x29 /* Protected | MethodCloned */,
      15,    1,  116,    2, 0x09 /* Protected */,
      15,    0,  119,    2, 0x29 /* Protected | MethodCloned */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   14,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void,

       0        // eod
};

void sofa::gui::qt::QSofaRecorder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QSofaRecorder *_t = static_cast<QSofaRecorder *>(_o);
        switch (_id) {
        case 0: _t->RecordSimulation((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->NewTime(); break;
        case 2: _t->TimerStart((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->slot_recordSimulation((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->slot_backward(); break;
        case 5: _t->slot_stepbackward(); break;
        case 6: _t->slot_playforward(); break;
        case 7: _t->slot_stepforward(); break;
        case 8: _t->slot_forward(); break;
        case 9: _t->slot_loadrecord_timevalue((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->slot_loadrecord_timevalue(); break;
        case 11: _t->slot_sliderValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 12: _t->slot_sliderValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->loadSimulation((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->loadSimulation(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QSofaRecorder::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSofaRecorder::RecordSimulation)) {
                *result = 0;
            }
        }
        {
            typedef void (QSofaRecorder::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSofaRecorder::NewTime)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject sofa::gui::qt::QSofaRecorder::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__QSofaRecorder.data,
      qt_meta_data_sofa__gui__qt__QSofaRecorder,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::QSofaRecorder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::QSofaRecorder::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__QSofaRecorder.stringdata))
        return static_cast<void*>(const_cast< QSofaRecorder*>(this));
    return QWidget::qt_metacast(_clname);
}

int sofa::gui::qt::QSofaRecorder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void sofa::gui::qt::QSofaRecorder::RecordSimulation(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void sofa::gui::qt::QSofaRecorder::NewTime()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
