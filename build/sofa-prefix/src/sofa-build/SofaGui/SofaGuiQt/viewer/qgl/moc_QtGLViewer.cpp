/****************************************************************************
** Meta object code from reading C++ file 'QtGLViewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../../ext/sofa/applications/sofa/gui/qt/viewer/qgl/QtGLViewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtGLViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sofa__gui__qt__viewer__qgl__QtGLViewer_t {
    QByteArrayData data[25];
    char stringdata[329];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__viewer__qgl__QtGLViewer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__viewer__qgl__QtGLViewer_t qt_meta_stringdata_sofa__gui__qt__viewer__qgl__QtGLViewer = {
    {
QT_MOC_LITERAL(0, 0, 38), // "sofa::gui::qt::viewer::qgl::Q..."
QT_MOC_LITERAL(1, 39, 7), // "redrawn"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 7), // "resizeW"
QT_MOC_LITERAL(4, 56, 7), // "resizeH"
QT_MOC_LITERAL(5, 64, 4), // "quit"
QT_MOC_LITERAL(6, 69, 9), // "resetView"
QT_MOC_LITERAL(7, 79, 8), // "saveView"
QT_MOC_LITERAL(8, 88, 8), // "setSizeW"
QT_MOC_LITERAL(9, 97, 8), // "setSizeH"
QT_MOC_LITERAL(10, 106, 7), // "getView"
QT_MOC_LITERAL(11, 114, 19), // "defaulttype::Vec3d&"
QT_MOC_LITERAL(12, 134, 3), // "pos"
QT_MOC_LITERAL(13, 138, 18), // "defaulttype::Quat&"
QT_MOC_LITERAL(14, 157, 3), // "ori"
QT_MOC_LITERAL(15, 161, 7), // "setView"
QT_MOC_LITERAL(16, 169, 18), // "defaulttype::Vec3d"
QT_MOC_LITERAL(17, 188, 17), // "defaulttype::Quat"
QT_MOC_LITERAL(18, 206, 12), // "captureEvent"
QT_MOC_LITERAL(19, 219, 13), // "fitObjectBBox"
QT_MOC_LITERAL(20, 233, 36), // "sofa::core::objectmodel::Base..."
QT_MOC_LITERAL(21, 270, 6), // "object"
QT_MOC_LITERAL(22, 277, 11), // "fitNodeBBox"
QT_MOC_LITERAL(23, 289, 34), // "sofa::core::objectmodel::Base..."
QT_MOC_LITERAL(24, 324, 4) // "node"

    },
    "sofa::gui::qt::viewer::qgl::QtGLViewer\0"
    "redrawn\0\0resizeW\0resizeH\0quit\0resetView\0"
    "saveView\0setSizeW\0setSizeH\0getView\0"
    "defaulttype::Vec3d&\0pos\0defaulttype::Quat&\0"
    "ori\0setView\0defaulttype::Vec3d\0"
    "defaulttype::Quat\0captureEvent\0"
    "fitObjectBBox\0sofa::core::objectmodel::BaseObject*\0"
    "object\0fitNodeBBox\0"
    "sofa::core::objectmodel::BaseNode*\0"
    "node"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__viewer__qgl__QtGLViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    1,   80,    2, 0x06 /* Public */,
       4,    1,   83,    2, 0x06 /* Public */,
       5,    0,   86,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   87,    2, 0x0a /* Public */,
       7,    0,   88,    2, 0x0a /* Public */,
       8,    1,   89,    2, 0x0a /* Public */,
       9,    1,   92,    2, 0x0a /* Public */,
      10,    2,   95,    2, 0x0a /* Public */,
      15,    2,  100,    2, 0x0a /* Public */,
      18,    0,  105,    2, 0x0a /* Public */,
      19,    1,  106,    2, 0x0a /* Public */,
      22,    1,  109,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 13,   12,   14,
    QMetaType::Void, 0x80000000 | 16, 0x80000000 | 17,   12,   14,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, 0x80000000 | 23,   24,

       0        // eod
};

void sofa::gui::qt::viewer::qgl::QtGLViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QtGLViewer *_t = static_cast<QtGLViewer *>(_o);
        switch (_id) {
        case 0: _t->redrawn(); break;
        case 1: _t->resizeW((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->resizeH((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->quit(); break;
        case 4: _t->resetView(); break;
        case 5: _t->saveView(); break;
        case 6: _t->setSizeW((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setSizeH((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->getView((*reinterpret_cast< defaulttype::Vec3d(*)>(_a[1])),(*reinterpret_cast< defaulttype::Quat(*)>(_a[2]))); break;
        case 9: _t->setView((*reinterpret_cast< const defaulttype::Vec3d(*)>(_a[1])),(*reinterpret_cast< const defaulttype::Quat(*)>(_a[2]))); break;
        case 10: _t->captureEvent(); break;
        case 11: _t->fitObjectBBox((*reinterpret_cast< sofa::core::objectmodel::BaseObject*(*)>(_a[1]))); break;
        case 12: _t->fitNodeBBox((*reinterpret_cast< sofa::core::objectmodel::BaseNode*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QtGLViewer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QtGLViewer::redrawn)) {
                *result = 0;
            }
        }
        {
            typedef void (QtGLViewer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QtGLViewer::resizeW)) {
                *result = 1;
            }
        }
        {
            typedef void (QtGLViewer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QtGLViewer::resizeH)) {
                *result = 2;
            }
        }
        {
            typedef void (QtGLViewer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QtGLViewer::quit)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject sofa::gui::qt::viewer::qgl::QtGLViewer::staticMetaObject = {
    { &QGLViewer::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__viewer__qgl__QtGLViewer.data,
      qt_meta_data_sofa__gui__qt__viewer__qgl__QtGLViewer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::viewer::qgl::QtGLViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::viewer::qgl::QtGLViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__viewer__qgl__QtGLViewer.stringdata))
        return static_cast<void*>(const_cast< QtGLViewer*>(this));
    if (!strcmp(_clname, "sofa::gui::qt::viewer::OglModelSofaViewer"))
        return static_cast< sofa::gui::qt::viewer::OglModelSofaViewer*>(const_cast< QtGLViewer*>(this));
    return QGLViewer::qt_metacast(_clname);
}

int sofa::gui::qt::viewer::qgl::QtGLViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLViewer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void sofa::gui::qt::viewer::qgl::QtGLViewer::redrawn()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void sofa::gui::qt::viewer::qgl::QtGLViewer::resizeW(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void sofa::gui::qt::viewer::qgl::QtGLViewer::resizeH(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void sofa::gui::qt::viewer::qgl::QtGLViewer::quit()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
