/****************************************************************************
** Meta object code from reading C++ file 'LinkWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../ext/sofa/applications/sofa/gui/qt/LinkWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LinkWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sofa__gui__qt__LinkWidget_t {
    QByteArrayData data[9];
    char stringdata[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__LinkWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__LinkWidget_t qt_meta_stringdata_sofa__gui__qt__LinkWidget = {
    {
QT_MOC_LITERAL(0, 0, 25), // "sofa::gui::qt::LinkWidget"
QT_MOC_LITERAL(1, 26, 11), // "WidgetDirty"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 14), // "LinkOwnerDirty"
QT_MOC_LITERAL(4, 54, 16), // "LinkBeingChanged"
QT_MOC_LITERAL(5, 71, 15), // "updateLinkValue"
QT_MOC_LITERAL(6, 87, 17), // "updateWidgetValue"
QT_MOC_LITERAL(7, 105, 14), // "setWidgetDirty"
QT_MOC_LITERAL(8, 120, 1) // "b"

    },
    "sofa::gui::qt::LinkWidget\0WidgetDirty\0"
    "\0LinkOwnerDirty\0LinkBeingChanged\0"
    "updateLinkValue\0updateWidgetValue\0"
    "setWidgetDirty\0b"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__LinkWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       3,    1,   52,    2, 0x06 /* Public */,
       4,    0,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   56,    2, 0x0a /* Public */,
       6,    0,   57,    2, 0x0a /* Public */,
       7,    0,   58,    2, 0x0a /* Public */,
       7,    1,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,

       0        // eod
};

void sofa::gui::qt::LinkWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LinkWidget *_t = static_cast<LinkWidget *>(_o);
        switch (_id) {
        case 0: _t->WidgetDirty((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->LinkOwnerDirty((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->LinkBeingChanged(); break;
        case 3: _t->updateLinkValue(); break;
        case 4: _t->updateWidgetValue(); break;
        case 5: _t->setWidgetDirty(); break;
        case 6: _t->setWidgetDirty((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LinkWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LinkWidget::WidgetDirty)) {
                *result = 0;
            }
        }
        {
            typedef void (LinkWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LinkWidget::LinkOwnerDirty)) {
                *result = 1;
            }
        }
        {
            typedef void (LinkWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LinkWidget::LinkBeingChanged)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject sofa::gui::qt::LinkWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__LinkWidget.data,
      qt_meta_data_sofa__gui__qt__LinkWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::LinkWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::LinkWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__LinkWidget.stringdata))
        return static_cast<void*>(const_cast< LinkWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int sofa::gui::qt::LinkWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void sofa::gui::qt::LinkWidget::WidgetDirty(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void sofa::gui::qt::LinkWidget::LinkOwnerDirty(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void sofa::gui::qt::LinkWidget::LinkBeingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
struct qt_meta_stringdata_sofa__gui__qt__QDisplayLinkInfoWidget_t {
    QByteArrayData data[3];
    char stringdata[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__QDisplayLinkInfoWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__QDisplayLinkInfoWidget_t qt_meta_stringdata_sofa__gui__qt__QDisplayLinkInfoWidget = {
    {
QT_MOC_LITERAL(0, 0, 37), // "sofa::gui::qt::QDisplayLinkIn..."
QT_MOC_LITERAL(1, 38, 11), // "getNumLines"
QT_MOC_LITERAL(2, 50, 0) // ""

    },
    "sofa::gui::qt::QDisplayLinkInfoWidget\0"
    "getNumLines\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__QDisplayLinkInfoWidget[] = {

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
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::UInt,

       0        // eod
};

void sofa::gui::qt::QDisplayLinkInfoWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QDisplayLinkInfoWidget *_t = static_cast<QDisplayLinkInfoWidget *>(_o);
        switch (_id) {
        case 0: { uint _r = _t->getNumLines();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject sofa::gui::qt::QDisplayLinkInfoWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__QDisplayLinkInfoWidget.data,
      qt_meta_data_sofa__gui__qt__QDisplayLinkInfoWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::QDisplayLinkInfoWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::QDisplayLinkInfoWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__QDisplayLinkInfoWidget.stringdata))
        return static_cast<void*>(const_cast< QDisplayLinkInfoWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int sofa::gui::qt::QDisplayLinkInfoWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
