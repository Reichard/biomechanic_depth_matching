/****************************************************************************
** Meta object code from reading C++ file 'WDoubleLineEdit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../ext/sofa/applications/sofa/gui/qt/WDoubleLineEdit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WDoubleLineEdit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WDoubleLineEdit_t {
    QByteArrayData data[12];
    char stringdata[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WDoubleLineEdit_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WDoubleLineEdit_t qt_meta_stringdata_WDoubleLineEdit = {
    {
QT_MOC_LITERAL(0, 0, 15), // "WDoubleLineEdit"
QT_MOC_LITERAL(1, 16, 12), // "ValueChanged"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 19), // "valuePercentChanged"
QT_MOC_LITERAL(4, 50, 13), // "slotCalcValue"
QT_MOC_LITERAL(5, 64, 17), // "slotReturnPressed"
QT_MOC_LITERAL(6, 82, 15), // "setValuePercent"
QT_MOC_LITERAL(7, 98, 1), // "p"
QT_MOC_LITERAL(8, 100, 8), // "minValue"
QT_MOC_LITERAL(9, 109, 8), // "maxValue"
QT_MOC_LITERAL(10, 118, 5), // "Value"
QT_MOC_LITERAL(11, 124, 8) // "intValue"

    },
    "WDoubleLineEdit\0ValueChanged\0\0"
    "valuePercentChanged\0slotCalcValue\0"
    "slotReturnPressed\0setValuePercent\0p\0"
    "minValue\0maxValue\0Value\0intValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WDoubleLineEdit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       4,   60, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       3,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   50,    2, 0x09 /* Protected */,
       4,    1,   53,    2, 0x09 /* Protected */,
       5,    0,   56,    2, 0x09 /* Protected */,
       6,    1,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,

 // properties: name, type, flags
       8, QMetaType::Double, 0x00095103,
       9, QMetaType::Double, 0x00095103,
      10, QMetaType::Double, 0x00095103,
      11, QMetaType::Int, 0x00095103,

       0        // eod
};

void WDoubleLineEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WDoubleLineEdit *_t = static_cast<WDoubleLineEdit *>(_o);
        switch (_id) {
        case 0: _t->ValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->valuePercentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slotCalcValue((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->slotCalcValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->slotReturnPressed(); break;
        case 5: _t->setValuePercent((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (WDoubleLineEdit::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WDoubleLineEdit::ValueChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (WDoubleLineEdit::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WDoubleLineEdit::valuePercentChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject WDoubleLineEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_WDoubleLineEdit.data,
      qt_meta_data_WDoubleLineEdit,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WDoubleLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WDoubleLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WDoubleLineEdit.stringdata))
        return static_cast<void*>(const_cast< WDoubleLineEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int WDoubleLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = minValue(); break;
        case 1: *reinterpret_cast< double*>(_v) = maxValue(); break;
        case 2: *reinterpret_cast< double*>(_v) = Value(); break;
        case 3: *reinterpret_cast< int*>(_v) = intValue(); break;
        default: break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setMinValue(*reinterpret_cast< double*>(_v)); break;
        case 1: setMaxValue(*reinterpret_cast< double*>(_v)); break;
        case 2: setValue(*reinterpret_cast< double*>(_v)); break;
        case 3: setIntValue(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void WDoubleLineEdit::ValueChanged(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WDoubleLineEdit::valuePercentChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
