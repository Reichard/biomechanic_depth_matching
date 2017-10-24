/****************************************************************************
** Meta object code from reading C++ file 'GenGraphForm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../ext/sofa/applications/sofa/gui/qt/GenGraphForm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GenGraphForm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sofa__gui__qt__GenGraphForm_t {
    QByteArrayData data[10];
    char stringdata[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__GenGraphForm_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__GenGraphForm_t qt_meta_stringdata_sofa__gui__qt__GenGraphForm = {
    {
QT_MOC_LITERAL(0, 0, 27), // "sofa::gui::qt::GenGraphForm"
QT_MOC_LITERAL(1, 28, 6), // "change"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 8), // "doBrowse"
QT_MOC_LITERAL(4, 45, 8), // "doExport"
QT_MOC_LITERAL(5, 54, 9), // "doDisplay"
QT_MOC_LITERAL(6, 64, 7), // "doClose"
QT_MOC_LITERAL(7, 72, 12), // "taskFinished"
QT_MOC_LITERAL(8, 85, 12), // "changeFilter"
QT_MOC_LITERAL(9, 98, 9) // "setFilter"

    },
    "sofa::gui::qt::GenGraphForm\0change\0\0"
    "doBrowse\0doExport\0doDisplay\0doClose\0"
    "taskFinished\0changeFilter\0setFilter"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__GenGraphForm[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    0,   59,    2, 0x0a /* Public */,
       8,    0,   60,    2, 0x0a /* Public */,
       9,    0,   61,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void sofa::gui::qt::GenGraphForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GenGraphForm *_t = static_cast<GenGraphForm *>(_o);
        switch (_id) {
        case 0: _t->change(); break;
        case 1: _t->doBrowse(); break;
        case 2: _t->doExport(); break;
        case 3: _t->doDisplay(); break;
        case 4: _t->doClose(); break;
        case 5: _t->taskFinished(); break;
        case 6: _t->changeFilter(); break;
        case 7: _t->setFilter(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject sofa::gui::qt::GenGraphForm::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__GenGraphForm.data,
      qt_meta_data_sofa__gui__qt__GenGraphForm,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::GenGraphForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::GenGraphForm::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__GenGraphForm.stringdata))
        return static_cast<void*>(const_cast< GenGraphForm*>(this));
    if (!strcmp(_clname, "Ui_BaseGenGraphForm"))
        return static_cast< Ui_BaseGenGraphForm*>(const_cast< GenGraphForm*>(this));
    return QDialog::qt_metacast(_clname);
}

int sofa::gui::qt::GenGraphForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
