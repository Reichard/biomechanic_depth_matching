/****************************************************************************
** Meta object code from reading C++ file 'DataFilenameWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../ext/sofa/applications/sofa/gui/qt/DataFilenameWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DataFilenameWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sofa__gui__qt__DataFileNameWidget_t {
    QByteArrayData data[3];
    char stringdata[47];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__DataFileNameWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__DataFileNameWidget_t qt_meta_stringdata_sofa__gui__qt__DataFileNameWidget = {
    {
QT_MOC_LITERAL(0, 0, 33), // "sofa::gui::qt::DataFileNameWi..."
QT_MOC_LITERAL(1, 34, 11), // "raiseDialog"
QT_MOC_LITERAL(2, 46, 0) // ""

    },
    "sofa::gui::qt::DataFileNameWidget\0"
    "raiseDialog\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__DataFileNameWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void sofa::gui::qt::DataFileNameWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DataFileNameWidget *_t = static_cast<DataFileNameWidget *>(_o);
        switch (_id) {
        case 0: _t->raiseDialog(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject sofa::gui::qt::DataFileNameWidget::staticMetaObject = {
    { &TDataWidget<std::string>::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__DataFileNameWidget.data,
      qt_meta_data_sofa__gui__qt__DataFileNameWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::DataFileNameWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::DataFileNameWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__DataFileNameWidget.stringdata))
        return static_cast<void*>(const_cast< DataFileNameWidget*>(this));
    return TDataWidget<std::string>::qt_metacast(_clname);
}

int sofa::gui::qt::DataFileNameWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TDataWidget<std::string>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
