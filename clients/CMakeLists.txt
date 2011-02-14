cmake_minimum_required(VERSION 2.6)
project(AudioScoutClient)

set(AudioScoutClient_MAJOR_VERSION 1)
set(AudioScoutClient_MINOR_VERSION 0)

add_definitions("-g -O3")

include_directories("${PROJECT_SOURCE_DIR}/../libs")
link_directories("${PROJECT_BINARY_DIR}/../libs")

add_executable(auscoutclient auscout-client.c)
target_link_libraries(auscoutclient AudioData pHashAudio zmq)


find_package(Qt4)
set(QT_USE_QTMULTIMEDIA TRUE)
include(${QT_USE_FILE})

set(QT_LIBRARIES "${QT_LIBRARIES};${QT_LIBRARY_DIR}/libQtMultimedia.so")
message(STATUS "QT_LIBRARIES - ${QT_LIBRARIES}")

set(SOURCES 
     mainwindow.h
     SendThread.h
     MeterWidget.h
)

QT4_WRAP_CPP(MOC1 mainwindow.h OPTIONS)
QT4_WRAP_CPP(MOC2 SendThread.h OPTIONS)
QT4_WRAP_CPP(MOC3 MeterWidget.h OPTIONS)

add_executable(auscout  auscout-client-main.cpp 
                        mainwindow.cpp ${MOC1} 
                        SendThread.cpp ${MOC2} 
                        MeterWidget.cpp ${MOC3})
target_link_libraries(auscout AudioData pHashAudio zmq ${QT_LIBRARIES})

install(TARGETS auscout DESTINATION bin)
install(TARGETS auscoutclient DESTINATION bin)

