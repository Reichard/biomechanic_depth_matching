/****************************************************************************
** Meta object code from reading C++ file 'QtViewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../../ext/sofa/applications/sofa/gui/qt/viewer/qt/QtViewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sofa__gui__qt__viewer__qt__QtViewer_t {
    QByteArrayData data[30];
    char stringdata[396];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sofa__gui__qt__viewer__qt__QtViewer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sofa__gui__qt__viewer__qt__QtViewer_t qt_meta_stringdata_sofa__gui__qt__viewer__qt__QtViewer = {
    {
QT_MOC_LITERAL(0, 0, 35), // "sofa::gui::qt::viewer::qt::Qt..."
QT_MOC_LITERAL(1, 36, 7), // "redrawn"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 7), // "resizeW"
QT_MOC_LITERAL(4, 53, 7), // "resizeH"
QT_MOC_LITERAL(5, 61, 4), // "quit"
QT_MOC_LITERAL(6, 66, 9), // "resetView"
QT_MOC_LITERAL(7, 76, 8), // "saveView"
QT_MOC_LITERAL(8, 85, 8), // "setSizeW"
QT_MOC_LITERAL(9, 94, 8), // "setSizeH"
QT_MOC_LITERAL(10, 103, 7), // "getView"
QT_MOC_LITERAL(11, 111, 19), // "defaulttype::Vec3d&"
QT_MOC_LITERAL(12, 131, 3), // "pos"
QT_MOC_LITERAL(13, 135, 18), // "defaulttype::Quat&"
QT_MOC_LITERAL(14, 154, 3), // "ori"
QT_MOC_LITERAL(15, 158, 7), // "setView"
QT_MOC_LITERAL(16, 166, 18), // "defaulttype::Vec3d"
QT_MOC_LITERAL(17, 185, 17), // "defaulttype::Quat"
QT_MOC_LITERAL(18, 203, 7), // "newView"
QT_MOC_LITERAL(19, 211, 8), // "moveView"
QT_MOC_LITERAL(20, 220, 12), // "captureEvent"
QT_MOC_LITERAL(21, 233, 17), // "drawColourPicking"
QT_MOC_LITERAL(22, 251, 32), // "ColourPickingVisitor::ColourCode"
QT_MOC_LITERAL(23, 284, 4), // "code"
QT_MOC_LITERAL(24, 289, 11), // "fitNodeBBox"
QT_MOC_LITERAL(25, 301, 34), // "sofa::core::objectmodel::Base..."
QT_MOC_LITERAL(26, 336, 4), // "node"
QT_MOC_LITERAL(27, 341, 13), // "fitObjectBBox"
QT_MOC_LITERAL(28, 355, 36), // "sofa::core::objectmodel::Base..."
QT_MOC_LITERAL(29, 392, 3) // "obj"

    },
    "sofa::gui::qt::viewer::qt::QtViewer\0"
    "redrawn\0\0resizeW\0resizeH\0quit\0resetView\0"
    "saveView\0setSizeW\0setSizeH\0getView\0"
    "defaulttype::Vec3d&\0pos\0defaulttype::Quat&\0"
    "ori\0setView\0defaulttype::Vec3d\0"
    "defaulttype::Quat\0newView\0moveView\0"
    "captureEvent\0drawColourPicking\0"
    "ColourPickingVisitor::ColourCode\0code\0"
    "fitNodeBBox\0sofa::core::objectmodel::BaseNode*\0"
    "node\0fitObjectBBox\0"
    "sofa::core::objectmodel::BaseObject*\0"
    "obj"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sofa__gui__qt__viewer__qt__QtViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x06 /* Public */,
       3,    1,   95,    2, 0x06 /* Public */,
       4,    1,   98,    2, 0x06 /* Public */,
       5,    0,  101,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,  102,    2, 0x0a /* Public */,
       7,    0,  103,    2, 0x0a /* Public */,
       8,    1,  104,    2, 0x0a /* Public */,
       9,    1,  107,    2, 0x0a /* Public */,
      10,    2,  110,    2, 0x0a /* Public */,
      15,    2,  115,    2, 0x0a /* Public */,
      18,    0,  120,    2, 0x0a /* Public */,
      19,    2,  121,    2, 0x0a /* Public */,
      20,    0,  126,    2, 0x0a /* Public */,
      21,    1,  127,    2, 0x0a /* Public */,
      24,    1,  130,    2, 0x0a /* Public */,
      27,    1,  133,    2, 0x0a /* Public */,

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
    QMetaType::Void, 0x80000000 | 16, 0x80000000 | 17,   12,   14,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void, 0x80000000 | 28,   29,

       0        // eod
};

void sofa::gui::qt::viewer::qt::QtViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QtViewer *_t = static_cast<QtViewer *>(_o);
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
        case 10: _t->newView(); break;
        case 11: _t->moveView((*reinterpret_cast< const defaulttype::Vec3d(*)>(_a[1])),(*reinterpret_cast< const defaulttype::Quat(*)>(_a[2]))); break;
        case 12: _t->captureEvent(); break;
        case 13: _t->drawColourPicking((*reinterpret_cast< ColourPickingVisitor::ColourCode(*)>(_a[1]))); break;
        case 14: _t->fitNodeBBox((*reinterpret_cast< sofa::core::objectmodel::BaseNode*(*)>(_a[1]))); break;
        case 15: _t->fitObjectBBox((*reinterpret_cast< sofa::core::objectmodel::BaseObject*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QtViewer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QtViewer::redrawn)) {
                *result = 0;
            }
        }
        {
            typedef void (QtViewer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QtViewer::resizeW)) {
                *result = 1;
            }
        }
        {
            typedef void (QtViewer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QtViewer::resizeH)) {
                *result = 2;
            }
        }
        {
            typedef void (QtViewer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QtViewer::quit)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject sofa::gui::qt::viewer::qt::QtViewer::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_sofa__gui__qt__viewer__qt__QtViewer.data,
      qt_meta_data_sofa__gui__qt__viewer__qt__QtViewer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sofa::gui::qt::viewer::qt::QtViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sofa::gui::qt::viewer::qt::QtViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sofa__gui__qt__viewer__qt__QtViewer.stringdata))
        return static_cast<void*>(const_cast< QtViewer*>(this));
    if (!strcmp(_clname, "sofa::gui::qt::viewer::OglModelSofaViewer"))
        return static_cast< sofa::gui::qt::viewer::OglModelSofaViewer*>(const_cast< QtViewer*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int sofa::gui::qt::viewer::qt::QtViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void sofa::gui::qt::viewer::qt::QtViewer::redrawn()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void sofa::gui::qt::viewer::qt::QtViewer::resizeW(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void sofa::gui::qt::viewer::qt::QtViewer::resizeH(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void sofa::gui::qt::viewer::qt::QtViewer::quit()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
