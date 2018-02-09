/****************************************************************************
** Meta object code from reading C++ file 'nemofacerecogwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "nemofacerecogwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'nemofacerecogwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NemoFaceRecogWidget_t {
    QByteArrayData data[7];
    char stringdata0[83];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NemoFaceRecogWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NemoFaceRecogWidget_t qt_meta_stringdata_NemoFaceRecogWidget = {
    {
QT_MOC_LITERAL(0, 0, 19), // "NemoFaceRecogWidget"
QT_MOC_LITERAL(1, 20, 21), // "updateFaceRecogResult"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 3), // "img"
QT_MOC_LITERAL(4, 47, 9), // "faceIndex"
QT_MOC_LITERAL(5, 57, 5), // "score"
QT_MOC_LITERAL(6, 63, 19) // "ShowFaceRecogResult"

    },
    "NemoFaceRecogWidget\0updateFaceRecogResult\0"
    "\0img\0faceIndex\0score\0ShowFaceRecogResult"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NemoFaceRecogWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    3,   31,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage, QMetaType::Int, QMetaType::Float,    3,    4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::QImage, QMetaType::Int, QMetaType::Float,    3,    4,    5,

       0        // eod
};

void NemoFaceRecogWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NemoFaceRecogWidget *_t = static_cast<NemoFaceRecogWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateFaceRecogResult((*reinterpret_cast< QImage(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 1: _t->ShowFaceRecogResult((*reinterpret_cast< QImage(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NemoFaceRecogWidget::*_t)(QImage , int , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NemoFaceRecogWidget::updateFaceRecogResult)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject NemoFaceRecogWidget::staticMetaObject = {
    { &QTableWidget::staticMetaObject, qt_meta_stringdata_NemoFaceRecogWidget.data,
      qt_meta_data_NemoFaceRecogWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NemoFaceRecogWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NemoFaceRecogWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NemoFaceRecogWidget.stringdata0))
        return static_cast<void*>(const_cast< NemoFaceRecogWidget*>(this));
    return QTableWidget::qt_metacast(_clname);
}

int NemoFaceRecogWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
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
void NemoFaceRecogWidget::updateFaceRecogResult(QImage _t1, int _t2, float _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
