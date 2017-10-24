/****************************************************************************
** Meta object code from reading C++ file 'DataWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../ext/sofa/applications/sofa/gui/qt/DataWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DataWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sofa__gui__qt__DataWidget_t {
    QByteArrayData data[10];
    char stringdata[138];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__DataWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__DataWidget_t qt_meta_stringdata_sofa__gui__qt__DataWidget = {
    {
QT_MOC_LITERAL(0, 0, 25), // "sofa::gui::qt::DataWidget"
QT_MOC_LITERAL(1, 26, 11), // "WidgetDirty"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 14), // "DataOwnerDirty"
QT_MOC_LITERAL(4, 54, 16), // "dataValueChanged"
QT_MOC_LITERAL(5, 71, 15), // "dataValueString"
QT_MOC_LITERAL(6, 87, 15), // "updateDataValue"
QT_MOC_LITERAL(7, 103, 17), // "updateWidgetValue"
QT_MOC_LITERAL(8, 121, 14), // "setWidgetDirty"
QT_MOC_LITERAL(9, 136, 1) // "b"

    },
    "sofa::gui::qt::DataWidget\0WidgetDirty\0"
    "\0DataOwnerDirty\0dataValueChanged\0"
    "dataValueString\0updateDataValue\0"
    "updateWidgetValue\0setWidgetDirty\0b"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__DataWidget[] = {

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
       4,    1,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   58,    2, 0x0a /* Public */,
       7,    0,   59,    2, 0x0a /* Public */,
       8,    1,   60,    2, 0x0a /* Public */,
       8,    0,   63,    2, 0x2a /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void,

       0        // eod
};

void sofa::gui::qt::DataWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DataWidget *_t = static_cast<DataWidget *>(_o);
        switch (_id) {
        case 0: _t->WidgetDirty((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->DataOwnerDirty((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->dataValueChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->updateDataValue(); break;
        case 4: _t->updateWidgetValue(); break;
        case 5: _t->setWidgetDirty((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setWidgetDirty(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DataWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataWidget::WidgetDirty)) {
                *result = 0;
            }
        }
        {
            typedef void (DataWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataWidget::DataOwnerDirty)) {
                *result = 1;
            }
        }
        {
            typedef void (DataWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataWidget::dataValueChanged)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject sofa::gui::qt::DataWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__DataWidget.data,
      qt_meta_data_sofa__gui__qt__DataWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::DataWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::DataWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__DataWidget.stringdata))
        return static_cast<void*>(const_cast< DataWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int sofa::gui::qt::DataWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void sofa::gui::qt::DataWidget::WidgetDirty(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void sofa::gui::qt::DataWidget::DataOwnerDirty(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void sofa::gui::qt::DataWidget::dataValueChanged(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_sofa__gui__qt__QPushButtonUpdater_t {
    QByteArrayData data[4];
    char stringdata[50];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__QPushButtonUpdater_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__QPushButtonUpdater_t qt_meta_stringdata_sofa__gui__qt__QPushButtonUpdater = {
    {
QT_MOC_LITERAL(0, 0, 33), // "sofa::gui::qt::QPushButtonUpd..."
QT_MOC_LITERAL(1, 34, 12), // "setDisplayed"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 1) // "b"

    },
    "sofa::gui::qt::QPushButtonUpdater\0"
    "setDisplayed\0\0b"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__QPushButtonUpdater[] = {

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
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void sofa::gui::qt::QPushButtonUpdater::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QPushButtonUpdater *_t = static_cast<QPushButtonUpdater *>(_o);
        switch (_id) {
        case 0: _t->setDisplayed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject sofa::gui::qt::QPushButtonUpdater::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__QPushButtonUpdater.data,
      qt_meta_data_sofa__gui__qt__QPushButtonUpdater,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::QPushButtonUpdater::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::QPushButtonUpdater::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__QPushButtonUpdater.stringdata))
        return static_cast<void*>(const_cast< QPushButtonUpdater*>(this));
    return QPushButton::qt_metacast(_clname);
}

int sofa::gui::qt::QPushButtonUpdater::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_sofa__gui__qt__QDisplayDataInfoWidget_t {
    QByteArrayData data[6];
    char stringdata[91];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__QDisplayDataInfoWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__QDisplayDataInfoWidget_t qt_meta_stringdata_sofa__gui__qt__QDisplayDataInfoWidget = {
    {
QT_MOC_LITERAL(0, 0, 37), // "sofa::gui::qt::QDisplayDataIn..."
QT_MOC_LITERAL(1, 38, 11), // "WidgetDirty"
QT_MOC_LITERAL(2, 50, 0), // ""
QT_MOC_LITERAL(3, 51, 16), // "linkModification"
QT_MOC_LITERAL(4, 68, 10), // "linkEdited"
QT_MOC_LITERAL(5, 79, 11) // "getNumLines"

    },
    "sofa::gui::qt::QDisplayDataInfoWidget\0"
    "WidgetDirty\0\0linkModification\0linkEdited\0"
    "getNumLines"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__QDisplayDataInfoWidget[] = {

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
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::UInt,

       0        // eod
};

void sofa::gui::qt::QDisplayDataInfoWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QDisplayDataInfoWidget *_t = static_cast<QDisplayDataInfoWidget *>(_o);
        switch (_id) {
        case 0: _t->WidgetDirty(); break;
        case 1: _t->linkModification(); break;
        case 2: _t->linkEdited(); break;
        case 3: { uint _r = _t->getNumLines();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QDisplayDataInfoWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QDisplayDataInfoWidget::WidgetDirty)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject sofa::gui::qt::QDisplayDataInfoWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__QDisplayDataInfoWidget.data,
      qt_meta_data_sofa__gui__qt__QDisplayDataInfoWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::QDisplayDataInfoWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::QDisplayDataInfoWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__QDisplayDataInfoWidget.stringdata))
        return static_cast<void*>(const_cast< QDisplayDataInfoWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int sofa::gui::qt::QDisplayDataInfoWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void sofa::gui::qt::QDisplayDataInfoWidget::WidgetDirty()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
