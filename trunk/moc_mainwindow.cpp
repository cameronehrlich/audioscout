/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Fri Dec 10 15:40:03 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      30,   11,   11,   11, 0x08,
      46,   11,   11,   11, 0x08,
      54,   11,   11,   11, 0x08,
      63,   11,   11,   11, 0x08,
      83,   11,   11,   11, 0x08,
     104,   11,   11,   11, 0x08,
     118,   11,   11,   11, 0x08,
     137,  132,   11,   11, 0x08,
     168,  162,   11,   11, 0x08,
     204,  196,   11,   11, 0x08,
     244,  236,   11,   11, 0x08,
     270,   11,   11,   11, 0x08,
     299,  287,   11,   11, 0x08,
     318,   11,   11,   11, 0x08,
     341,   11,   11,   11, 0x08,
     354,   11,   11,   11, 0x08,
     378,  369,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0connectToServer()\0"
    "browseforfile()\0about()\0submit()\0"
    "changeToQueryMode()\0changeToSubmitMode()\0"
    "sampleAudio()\0readSamples()\0text\0"
    "appendPlainText(QString)\0level\0"
    "changeProgressBarLevel(int)\0min,max\0"
    "changeProgressBarRange(int,int)\0visible\0"
    "activateProgressBar(bool)\0finishWithTask()\0"
    "errorString\0postError(QString)\0"
    "changeSampleInterval()\0changeGain()\0"
    "clearDisplay()\0newState\0"
    "audioStateChanged(QAudio::State)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: connectToServer(); break;
        case 1: browseforfile(); break;
        case 2: about(); break;
        case 3: submit(); break;
        case 4: changeToQueryMode(); break;
        case 5: changeToSubmitMode(); break;
        case 6: sampleAudio(); break;
        case 7: readSamples(); break;
        case 8: appendPlainText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: changeProgressBarLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: changeProgressBarRange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: activateProgressBar((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: finishWithTask(); break;
        case 13: postError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: changeSampleInterval(); break;
        case 15: changeGain(); break;
        case 16: clearDisplay(); break;
        case 17: audioStateChanged((*reinterpret_cast< QAudio::State(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
