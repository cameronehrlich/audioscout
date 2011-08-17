cmake_minimum_required(VERSION 2.6)
project(AudioScoutClient)

set(AudioScoutClient_MAJOR_VERSION 1)
set(AudioScoutClient_MINOR_VERSION 0)

add_definitions("-g -O3")

include_directories("${PROJECT_SOURCE_DIR}/../libs")
link_directories("${PROJECT_BINARY_DIR}/../libs")

add_executable(auscoutclient auscout-client.c)

target_link_libraries(auscoutclient AudioData pHashAudio zmq)

find_package(Qt4 4.7.3 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(QT_USE_QTMULTIMEDIA TRUE)

set(QT_LIBRARIES "${QT_LIBRARIES};-lQtMultimediaKit")

set(AudioScoutClient_HEADERS 
     mainwindow.h
     SendThread.h
     MeterWidget.h
)

set(AudioScoutClient_SOURCES
     auscout-client-main.cpp
      mainwindow.cpp 
      SendThread.cpp
      MeterWidget.cpp
)

QT4_WRAP_CPP(AudioScoutClient_HEADERS_MOCS ${AudioScoutClient_HEADERS} OPTIONS)

configure_file(auscout-gui "${PROJECT_BINARY_DIR}/auscout-gui" COPY_ONLY)

add_executable(auscout  ${AudioScoutClient_SOURCES}
                        ${AudioScoutClient_HEADERS_MOCS}
		)

include_directories("${QT_INCLUDE_DIR}/../QtMobility")
include_directories("${QT_INCLUDE_DIR}/../QtMultimediaKit")

target_link_libraries(auscout AudioData pHashAudio zmq ${QT_LIBRARIES})

set(PERMS OWNER_READ OWNER_WRITE OWNER_EXECUTE 
          GROUP_READ GROUP_EXECUTE 
          WORLD_READ WORLD_EXECUTE)

install(TARGETS auscout DESTINATION bin)
install(FILES Microphone-icon.png DESTINATION bin)
install(FILES auscout-gui DESTINATION bin PERMISSIONS ${PERMS})
install(TARGETS auscoutclient DESTINATION bin)

