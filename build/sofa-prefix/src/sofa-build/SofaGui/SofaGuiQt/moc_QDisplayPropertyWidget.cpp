/****************************************************************************
** Meta object code from reading C++ file 'QDisplayPropertyWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../ext/sofa/applications/sofa/gui/qt/QDisplayPropertyWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QDisplayPropertyWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sofa__gui__qt__QDisplayTreeItemWidget_t {
    QByteArrayData data[3];
    char stringdata[57];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__QDisplayTreeItemWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__QDisplayTreeItemWidget_t qt_meta_stringdata_sofa__gui__qt__QDisplayTreeItemWidget = {
    {
QT_MOC_LITERAL(0, 0, 37), // "sofa::gui::qt::QDisplayTreeIt..."
QT_MOC_LITERAL(1, 38, 17), // "updateDirtyWidget"
QT_MOC_LITERAL(2, 56, 0) // ""

    },
    "sofa::gui::qt::QDisplayTreeItemWidget\0"
    "updateDirtyWidget\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__QDisplayTreeItemWidget[] = {

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

void sofa::gui::qt::QDisplayTreeItemWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QDisplayTreeItemWidget *_t = static_cast<QDisplayTreeItemWidget *>(_o);
        switch (_id) {
        case 0: _t->updateDirtyWidget(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject sofa::gui::qt::QDisplayTreeItemWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__QDisplayTreeItemWidget.data,
      qt_meta_data_sofa__gui__qt__QDisplayTreeItemWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::QDisplayTreeItemWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::QDisplayTreeItemWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__QDisplayTreeItemWidget.stringdata))
        return static_cast<void*>(const_cast< QDisplayTreeItemWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int sofa::gui::qt::QDisplayTreeItemWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_sofa__gui__qt__QDisplayPropertyWidget_t {
    QByteArrayData data[5];
    char stringdata[101];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__QDisplayPropertyWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__QDisplayPropertyWidget_t qt_meta_stringdata_sofa__gui__qt__QDisplayPropertyWidget = {
    {
QT_MOC_LITERAL(0, 0, 37), // "sofa::gui::qt::QDisplayProper..."
QT_MOC_LITERAL(1, 38, 18), // "updateListViewItem"
QT_MOC_LITERAL(2, 57, 0), // ""
QT_MOC_LITERAL(3, 58, 20), // "clearComponentOutput"
QT_MOC_LITERAL(4, 79, 21) // "clearComponentWarning"

    },
    "sofa::gui::qt::QDisplayPropertyWidget\0"
    "updateListViewItem\0\0clearComponentOutput\0"
    "clearComponentWarning"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__QDisplayPropertyWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x09 /* Protected */,
       3,    0,   30,    2, 0x09 /* Protected */,
       4,    0,   31,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void sofa::gui::qt::QDisplayPropertyWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QDisplayPropertyWidget *_t = static_cast<QDisplayPropertyWidget *>(_o);
        switch (_id) {
        case 0: _t->updateListViewItem(); break;
        case 1: _t->clearComponentOutput(); break;
        case 2: _t->clearComponentWarning(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject sofa::gui::qt::QDisplayPropertyWidget::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__QDisplayPropertyWidget.data,
      qt_meta_data_sofa__gui__qt__QDisplayPropertyWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::QDisplayPropertyWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::QDisplayPropertyWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__QDisplayPropertyWidget.stringdata))
        return static_cast<void*>(const_cast< QDisplayPropertyWidget*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int sofa::gui::qt::QDisplayPropertyWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
