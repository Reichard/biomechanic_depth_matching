/****************************************************************************
** Meta object code from reading C++ file 'AddObject.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../ext/sofa/applications/sofa/gui/qt/AddObject.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AddObject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sofa__gui__qt__AddObject_t {
    QByteArrayData data[8];
    char stringdata[88];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__AddObject_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__AddObject_t qt_meta_stringdata_sofa__gui__qt__AddObject = {
    {
QT_MOC_LITERAL(0, 0, 24), // "sofa::gui::qt::AddObject"
QT_MOC_LITERAL(1, 25, 10), // "loadObject"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 11), // "std::string"
QT_MOC_LITERAL(4, 49, 8), // "fileOpen"
QT_MOC_LITERAL(5, 58, 12), // "buttonUpdate"
QT_MOC_LITERAL(6, 71, 9), // "optionSet"
QT_MOC_LITERAL(7, 81, 6) // "accept"

    },
    "sofa::gui::qt::AddObject\0loadObject\0"
    "\0std::string\0fileOpen\0buttonUpdate\0"
    "optionSet\0accept"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__AddObject[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    8,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   51,    2, 0x0a /* Public */,
       5,    1,   52,    2, 0x0a /* Public */,
       7,    0,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,    2,    2,    2,    2,    2,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void,

       0        // eod
};

void sofa::gui::qt::AddObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AddObject *_t = static_cast<AddObject *>(_o);
        switch (_id) {
        case 0: _t->loadObject((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6])),(*reinterpret_cast< double(*)>(_a[7])),(*reinterpret_cast< double(*)>(_a[8]))); break;
        case 1: _t->fileOpen(); break;
        case 2: _t->buttonUpdate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->accept(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AddObject::*_t)(std::string , double , double , double , double , double , double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AddObject::loadObject)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject sofa::gui::qt::AddObject::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__AddObject.data,
      qt_meta_data_sofa__gui__qt__AddObject,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::AddObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::AddObject::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__AddObject.stringdata))
        return static_cast<void*>(const_cast< AddObject*>(this));
    if (!strcmp(_clname, "Ui_DialogAddObject"))
        return static_cast< Ui_DialogAddObject*>(const_cast< AddObject*>(this));
    return QDialog::qt_metacast(_clname);
}

int sofa::gui::qt::AddObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void sofa::gui::qt::AddObject::loadObject(std::string _t1, double _t2, double _t3, double _t4, double _t5, double _t6, double _t7, double _t8)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
