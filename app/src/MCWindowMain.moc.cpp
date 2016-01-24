/****************************************************************************
** Meta object code from reading C++ file 'MCWindowMain.h'
**
** Created: Sun Aug 4 08:33:27 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MCWindowMain.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MCWindowMain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MC__WindowMain[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   31,   36,   36, 0x08,
      37,   86,   36,   36, 0x08,
      93,   36,   36,   36, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MC__WindowMain[] = {
    "MC::WindowMain\0grabInput(bool)\0grab\0"
    "\0iconActivated(QSystemTrayIcon::ActivationReason)\0"
    "reason\0actionTriggered()\0"
};

const QMetaObject MC::WindowMain::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MC__WindowMain,
      qt_meta_data_MC__WindowMain, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MC::WindowMain::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MC::WindowMain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MC::WindowMain::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MC__WindowMain))
        return static_cast<void*>(const_cast< WindowMain*>(this));
    return QWidget::qt_metacast(_clname);
}

int MC::WindowMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: grabInput((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: iconActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 2: actionTriggered(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
