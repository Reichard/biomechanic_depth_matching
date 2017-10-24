/****************************************************************************
** Meta object code from reading C++ file 'MaterialDataWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../ext/sofa/applications/sofa/gui/qt/MaterialDataWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MaterialDataWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sofa__gui__qt__RGBAColorPicker_t {
    QByteArrayData data[6];
    char stringdata[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__RGBAColorPicker_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__RGBAColorPicker_t qt_meta_stringdata_sofa__gui__qt__RGBAColorPicker = {
    {
QT_MOC_LITERAL(0, 0, 30), // "sofa::gui::qt::RGBAColorPicker"
QT_MOC_LITERAL(1, 31, 10), // "hasChanged"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 15), // "updateRGBAColor"
QT_MOC_LITERAL(4, 59, 17), // "redrawColorButton"
QT_MOC_LITERAL(5, 77, 17) // "raiseQColorDialog"

    },
    "sofa::gui::qt::RGBAColorPicker\0"
    "hasChanged\0\0updateRGBAColor\0"
    "redrawColorButton\0raiseQColorDialog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__RGBAColorPicker[] = {

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
       1,    0,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   35,    2, 0x09 /* Protected */,
       4,    0,   36,    2, 0x09 /* Protected */,
       5,    0,   37,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void sofa::gui::qt::RGBAColorPicker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RGBAColorPicker *_t = static_cast<RGBAColorPicker *>(_o);
        switch (_id) {
        case 0: _t->hasChanged(); break;
        case 1: _t->updateRGBAColor(); break;
        case 2: _t->redrawColorButton(); break;
        case 3: _t->raiseQColorDialog(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RGBAColorPicker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RGBAColorPicker::hasChanged)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject sofa::gui::qt::RGBAColorPicker::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__RGBAColorPicker.data,
      qt_meta_data_sofa__gui__qt__RGBAColorPicker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::RGBAColorPicker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::RGBAColorPicker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__RGBAColorPicker.stringdata))
        return static_cast<void*>(const_cast< RGBAColorPicker*>(this));
    return QWidget::qt_metacast(_clname);
}

int sofa::gui::qt::RGBAColorPicker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void sofa::gui::qt::RGBAColorPicker::hasChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_sofa__gui__qt__MaterialDataWidget_t {
    QByteArrayData data[1];
    char stringdata[34];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__MaterialDataWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__MaterialDataWidget_t qt_meta_stringdata_sofa__gui__qt__MaterialDataWidget = {
    {
QT_MOC_LITERAL(0, 0, 33) // "sofa::gui::qt::MaterialDataWi..."

    },
    "sofa::gui::qt::MaterialDataWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__MaterialDataWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void sofa::gui::qt::MaterialDataWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject sofa::gui::qt::MaterialDataWidget::staticMetaObject = {
    { &TDataWidget<sofa::core::loader::Material>::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__MaterialDataWidget.data,
      qt_meta_data_sofa__gui__qt__MaterialDataWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::MaterialDataWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::MaterialDataWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__MaterialDataWidget.stringdata))
        return static_cast<void*>(const_cast< MaterialDataWidget*>(this));
    return TDataWidget<sofa::core::loader::Material>::qt_metacast(_clname);
}

int sofa::gui::qt::MaterialDataWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TDataWidget<sofa::core::loader::Material>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_sofa__gui__qt__VectorMaterialDataWidget_t {
    QByteArrayData data[3];
    char stringdata[56];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__VectorMaterialDataWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__VectorMaterialDataWidget_t qt_meta_stringdata_sofa__gui__qt__VectorMaterialDataWidget = {
    {
QT_MOC_LITERAL(0, 0, 39), // "sofa::gui::qt::VectorMaterial..."
QT_MOC_LITERAL(1, 40, 14), // "changeMaterial"
QT_MOC_LITERAL(2, 55, 0) // ""

    },
    "sofa::gui::qt::VectorMaterialDataWidget\0"
    "changeMaterial\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__VectorMaterialDataWidget[] = {

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
       1,    1,   19,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void sofa::gui::qt::VectorMaterialDataWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VectorMaterialDataWidget *_t = static_cast<VectorMaterialDataWidget *>(_o);
        switch (_id) {
        case 0: _t->changeMaterial((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject sofa::gui::qt::VectorMaterialDataWidget::staticMetaObject = {
    { &TDataWidget<VectorMaterial>::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__VectorMaterialDataWidget.data,
      qt_meta_data_sofa__gui__qt__VectorMaterialDataWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::VectorMaterialDataWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::VectorMaterialDataWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__VectorMaterialDataWidget.stringdata))
        return static_cast<void*>(const_cast< VectorMaterialDataWidget*>(this));
    return TDataWidget<VectorMaterial>::qt_metacast(_clname);
}

int sofa::gui::qt::VectorMaterialDataWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TDataWidget<VectorMaterial>::qt_metacall(_c, _id, _a);
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
