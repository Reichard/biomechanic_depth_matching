/****************************************************************************
** Meta object code from reading C++ file 'DisplayFlagsDataWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../ext/sofa/applications/sofa/gui/qt/DisplayFlagsDataWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DisplayFlagsDataWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sofa__gui__qt__DisplayFlagWidget_t {
    QByteArrayData data[4];
    char stringdata[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__DisplayFlagWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__DisplayFlagWidget_t qt_meta_stringdata_sofa__gui__qt__DisplayFlagWidget = {
    {
QT_MOC_LITERAL(0, 0, 32), // "sofa::gui::qt::DisplayFlagWidget"
QT_MOC_LITERAL(1, 33, 6), // "change"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 7) // "clicked"

    },
    "sofa::gui::qt::DisplayFlagWidget\0"
    "change\0\0clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__DisplayFlagWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,
       3,    0,   29,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    2,    2,
    QMetaType::Void,

       0        // eod
};

void sofa::gui::qt::DisplayFlagWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DisplayFlagWidget *_t = static_cast<DisplayFlagWidget *>(_o);
        switch (_id) {
        case 0: _t->change((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DisplayFlagWidget::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DisplayFlagWidget::change)) {
                *result = 0;
            }
        }
        {
            typedef void (DisplayFlagWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DisplayFlagWidget::clicked)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject sofa::gui::qt::DisplayFlagWidget::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__DisplayFlagWidget.data,
      qt_meta_data_sofa__gui__qt__DisplayFlagWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::DisplayFlagWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::DisplayFlagWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__DisplayFlagWidget.stringdata))
        return static_cast<void*>(const_cast< DisplayFlagWidget*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int sofa::gui::qt::DisplayFlagWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void sofa::gui::qt::DisplayFlagWidget::change(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void sofa::gui::qt::DisplayFlagWidget::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
struct qt_meta_stringdata_sofa__gui__qt__DisplayFlagsDataWidget_t {
    QByteArrayData data[1];
    char stringdata[38];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__DisplayFlagsDataWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__DisplayFlagsDataWidget_t qt_meta_stringdata_sofa__gui__qt__DisplayFlagsDataWidget = {
    {
QT_MOC_LITERAL(0, 0, 37) // "sofa::gui::qt::DisplayFlagsDa..."

    },
    "sofa::gui::qt::DisplayFlagsDataWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__DisplayFlagsDataWidget[] = {

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

void sofa::gui::qt::DisplayFlagsDataWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject sofa::gui::qt::DisplayFlagsDataWidget::staticMetaObject = {
    { &TDataWidget<sofa::core::visual::DisplayFlags>::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__DisplayFlagsDataWidget.data,
      qt_meta_data_sofa__gui__qt__DisplayFlagsDataWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::DisplayFlagsDataWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::DisplayFlagsDataWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__DisplayFlagsDataWidget.stringdata))
        return static_cast<void*>(const_cast< DisplayFlagsDataWidget*>(this));
    return TDataWidget<sofa::core::visual::DisplayFlags>::qt_metacast(_clname);
}

int sofa::gui::qt::DisplayFlagsDataWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TDataWidget<sofa::core::visual::DisplayFlags>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
