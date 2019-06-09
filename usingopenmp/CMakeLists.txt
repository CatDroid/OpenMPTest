# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

set(CMAKE_VERBOSE_MAKEFILE on)


add_library(
        native-lib
        SHARED
        src/main/cpp/native-lib.cpp)


find_library(log-lib log)
#find_library(gomp-lib gomp)

target_link_libraries(
        native-lib
        ${log-lib}
         )
# -lgomp 不要加上这个 会导致只有一个线程work??不支持什么原因