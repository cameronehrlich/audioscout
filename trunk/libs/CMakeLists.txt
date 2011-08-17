cmake_minimum_required(VERSION 2.6)
project(pHashAudio)

set(pHashAudio_VERSION_MAJOR 1)
set(pHashAudio_VERSION_MINOR 0)

add_definitions("-g -O3 -shared -fPIC")

add_subdirectory(table-4.3.0phmodified)

find_library(HAVE_MPG123 mpg123)
find_library(HAVE_SNDFILE sndfile)
find_library(HAVE_SAMPLERATE samplerate)
find_library(HAVE_ZMQ zmq)

configure_file(
  "${PROJECT_SOURCE_DIR}/pHashAudioConfig.h.in"
  "${PROJECT_BINARY_DIR}/pHashAudioConfig.h"
)

include_directories("${PROJECT_BINARY_DIR}")

link_directories("${PROJECT_BINARY_DIR}/table-4.3.0phmodified")

if(HAVE_MPG123)
set(EXTRA_LIBS ${EXTRA_LIBS} mpg123)
endif(HAVE_MPG123)

add_library(pHashAudio SHARED phash_audio.c phash_audio.h fft.c fft.h complex.c complex.h)
target_link_libraries(pHashAudio table)

add_library(AudioData SHARED audiodata.c audiodata.h pHashAudioConfig.h)
target_link_libraries(AudioData sndfile samplerate ${EXTRA_LIBS})

add_executable(audioindex audio_index.c)
target_link_libraries(audioindex AudioData pHashAudio zmq sndfile samplerate m)

install(TARGETS pHashAudio DESTINATION lib)
install(FILES phash_audio.h DESTINATION include)

install(TARGETS AudioData  DESTINATION lib)
install(FILES audiodata.h DESTINATION include)

install(TARGETS audioindex DESTINATION bin)