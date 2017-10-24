/****************************************************************************
** Meta object code from reading C++ file 'QDisplayDataWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../ext/sofa/applications/sofa/gui/qt/QDisplayDataWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QDisplayDataWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sofa__gui__qt__QDisplayDataWidget_t {
    QByteArrayData data[9];
    char stringdata[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__QDisplayDataWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__QDisplayDataWidget_t qt_meta_stringdata_sofa__gui__qt__QDisplayDataWidget = {
    {
QT_MOC_LITERAL(0, 0, 33), // "sofa::gui::qt::QDisplayDataWi..."
QT_MOC_LITERAL(1, 34, 11), // "WidgetDirty"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 12), // "WidgetUpdate"
QT_MOC_LITERAL(4, 60, 10), // "DataUpdate"
QT_MOC_LITERAL(5, 71, 14), // "DataOwnerDirty"
QT_MOC_LITERAL(6, 86, 16), // "dataValueChanged"
QT_MOC_LITERAL(7, 103, 10), // "UpdateData"
QT_MOC_LITERAL(8, 114, 13) // "UpdateWidgets"

    },
    "sofa::gui::qt::QDisplayDataWidget\0"
    "WidgetDirty\0\0WidgetUpdate\0DataUpdate\0"
    "DataOwnerDirty\0dataValueChanged\0"
    "UpdateData\0UpdateWidgets"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__QDisplayDataWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       3,    0,   52,    2, 0x06 /* Public */,
       4,    0,   53,    2, 0x06 /* Public */,
       5,    1,   54,    2, 0x06 /* Public */,
       6,    1,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   60,    2, 0x0a /* Public */,
       8,    0,   61,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void sofa::gui::qt::QDisplayDataWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QDisplayDataWidget *_t = static_cast<QDisplayDataWidget *>(_o);
        switch (_id) {
        case 0: _t->WidgetDirty((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->WidgetUpdate(); break;
        case 2: _t->DataUpdate(); break;
        case 3: _t->DataOwnerDirty((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->dataValueChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->UpdateData(); break;
        case 6: _t->UpdateWidgets(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QDisplayDataWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QDisplayDataWidget::WidgetDirty)) {
                *result = 0;
            }
        }
        {
            typedef void (QDisplayDataWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QDisplayDataWidget::WidgetUpdate)) {
                *result = 1;
            }
        }
        {
            typedef void (QDisplayDataWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QDisplayDataWidget::DataUpdate)) {
                *result = 2;
            }
        }
        {
            typedef void (QDisplayDataWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QDisplayDataWidget::DataOwnerDirty)) {
                *result = 3;
            }
        }
        {
            typedef void (QDisplayDataWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QDisplayDataWidget::dataValueChanged)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject sofa::gui::qt::QDisplayDataWidget::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__QDisplayDataWidget.data,
      qt_meta_data_sofa__gui__qt__QDisplayDataWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::QDisplayDataWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::QDisplayDataWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__QDisplayDataWidget.stringdata))
        return static_cast<void*>(const_cast< QDisplayDataWidget*>(this));
    return QGroupBox::qt_metacast(_clname);
}

int sofa::gui::qt::QDisplayDataWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
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
void sofa::gui::qt::QDisplayDataWidget::WidgetDirty(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void sofa::gui::qt::QDisplayDataWidget::WidgetUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void sofa::gui::qt::QDisplayDataWidget::DataUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void sofa::gui::qt::QDisplayDataWidget::DataOwnerDirty(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void sofa::gui::qt::QDisplayDataWidget::dataValueChanged(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
struct qt_meta_stringdata_sofa__gui__qt__QDataSimpleEdit_t {
    QByteArrayData data[1];
    char stringdata[31];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__QDataSimpleEdit_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__QDataSimpleEdit_t qt_meta_stringdata_sofa__gui__qt__QDataSimpleEdit = {
    {
QT_MOC_LITERAL(0, 0, 30) // "sofa::gui::qt::QDataSimpleEdit"

    },
    "sofa::gui::qt::QDataSimpleEdit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__QDataSimpleEdit[] = {

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

void sofa::gui::qt::QDataSimpleEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject sofa::gui::qt::QDataSimpleEdit::staticMetaObject = {
    { &DataWidget::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__QDataSimpleEdit.data,
      qt_meta_data_sofa__gui__qt__QDataSimpleEdit,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::QDataSimpleEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::QDataSimpleEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__QDataSimpleEdit.stringdata))
        return static_cast<void*>(const_cast< QDataSimpleEdit*>(this));
    return DataWidget::qt_metacast(_clname);
}

int sofa::gui::qt::QDataSimpleEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DataWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_sofa__gui__qt__QPoissonRatioWidget_t {
    QByteArrayData data[4];
    char stringdata[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__QPoissonRatioWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__QPoissonRatioWidget_t qt_meta_stringdata_sofa__gui__qt__QPoissonRatioWidget = {
    {
QT_MOC_LITERAL(0, 0, 34), // "sofa::gui::qt::QPoissonRatioW..."
QT_MOC_LITERAL(1, 35, 19), // "changeLineEditValue"
QT_MOC_LITERAL(2, 55, 0), // ""
QT_MOC_LITERAL(3, 56, 17) // "changeSliderValue"

    },
    "sofa::gui::qt::QPoissonRatioWidget\0"
    "changeLineEditValue\0\0changeSliderValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__QPoissonRatioWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x09 /* Protected */,
       3,    0,   25,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void sofa::gui::qt::QPoissonRatioWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QPoissonRatioWidget *_t = static_cast<QPoissonRatioWidget *>(_o);
        switch (_id) {
        case 0: _t->changeLineEditValue(); break;
        case 1: _t->changeSliderValue(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject sofa::gui::qt::QPoissonRatioWidget::staticMetaObject = {
    { &TDataWidget<double>::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__QPoissonRatioWidget.data,
      qt_meta_data_sofa__gui__qt__QPoissonRatioWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::QPoissonRatioWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::QPoissonRatioWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__QPoissonRatioWidget.stringdata))
        return static_cast<void*>(const_cast< QPoissonRatioWidget*>(this));
    return TDataWidget<double>::qt_metacast(_clname);
}

int sofa::gui::qt::QPoissonRatioWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TDataWidget<double>::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
