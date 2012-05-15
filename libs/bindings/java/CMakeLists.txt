find_package(JNI)

include_directories(${JNI_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

add_library(AudioScout-jni SHARED AudioScout-jni.cpp)

target_link_libraries(AudioScout-jni ${PROJECT_NAME})


install(FILES AudioScout-jni.h DESTINATION include)
