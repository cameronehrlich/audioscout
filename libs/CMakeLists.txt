cmake_minimum_required(VERSION 2.6)
project(pHashAudio)

set(pHashAudio_VERSION_MAJOR 1)
set(pHashAudio_VERSION_MINOR 0)

if (CMAKE_HOST_WIN32)

#  set windows variables here:
#  where to put include and static export libraries 

   set(MY_INSTALL_LOC "c:/Users/Public/local")
   set(MY_INCLUDE_LOC "c:/Users/Public/local/include")
   set(MY_LIB_LOC     "c:/Users/Public/local/lib")

   message("win32 system detected")
   set(BUILD_DLL 1)
   include_directories(${MY_INCLUDE_LOC})
   link_directories(${MY_LIB_LOC})
   set(CMAKE_INSTALL_PREFIX  ${MY_INSTALL_LOC})
   add_definitions(/D _MBCS)

endif(CMAKE_HOST_WIN32)

if (CMAKE_HOST_UNIX)

# set unix system variables

   message("unix system detected")
   add_definitions(-g -O3 -shared -fPIC)

endif(CMAKE_HOST_UNIX)

add_subdirectory(table-4.3.0phmodified)

configure_file(
  "${PROJECT_SOURCE_DIR}/pHashAudioConfig.h.in"
  "${PROJECT_BINARY_DIR}/pHashAudioConfig.h"
)

include_directories("${PROJECT_BINARY_DIR}")
link_directories("${PROJECT_BINARY_DIR}/table-4.3.0phmodified")

add_library(pHashAudio SHARED phash_audio.c fft.c phcomplex.c)
target_link_libraries(pHashAudio table)

add_library(AudioData SHARED audiodata.c)
target_link_libraries(AudioData  libsndfile-1 libsamplerate-0 zmq)

if (CMAKE_HOST_UNIX)

   add_executable(audioindex audio_index.c)
   target_link_libraries(audioindex AudioData pHashAudio m)
endif(CMAKE_HOST_UNIX)

if (CMAKE_HOST_WIN32)
# compile csharp bindings for windows
  add_subdirectory(bindings/csharp)
endif (CMAKE_HOST_WIN32)

#install files 

if(CMAKE_HOST_WIN32)
   install(FILES pHashAudio.dll AudioData.dll DESTINATION bin)
   install(FILES pHashAudio.dll.a AudioData.dll.a DESTINATION lib)
else(CMAKE_HOST_WIN32)
   install(TARGETS pHashAudio AudioData DESTINATION lib)
endif(CMAKE_HOST_WIN32)

install(FILES phash_audio.h DESTINATION include)
install(FILES audiodata.h DESTINATION include)

if (CMAKE_HOST_UNIX)
   install(TARGETS audioindex DESTINATION bin)
endif(CMAKE_HOST_UNIX)
