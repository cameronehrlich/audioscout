CONFIG += qt
QT += multimedia
LIBS += -L/usr/local/lib -L. -lzmq -lAudioData -lpHashAudio
HEADERS = mainwindow.h SendThread.h audiodata.h phash_audio.h MeterWidget.h
SOURCES = auscout-client-main.cpp mainwindow.cpp SendThread.cpp MeterWidget.cpp
TARGET = auscout

