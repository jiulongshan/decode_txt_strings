#CMakeLists.txt 模板

cmake_minimum_required(VERSION 3.10)

project("ucar_vision_libs" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV 3.2.0 REQUIRED)

add_definitions("-Wall -g")

#include dirs
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ./include
    ./libs/include
)

#link dirs
link_directories(
    ${OpenCV_LIBRARY_DIRS}
    ./libs/libs/
)

#link libs
link_libraries(
    ${OpenCV_LIBS}
    -lpthread
)

#add_library(vision_infer SHARED
#    ./src/vision_recognition.cpp
#)

add_executable(video2frames main.cpp)
add_executable(ai_magic ai_magic.cpp)
add_executable(motion_detect opencv_motion_detect.cpp)
add_executable(motion_md opencv_md.cpp)
add_executable(sstest stringstream.cpp)
#单个文件命令编译：
# g++ `pkg-config --cflags --libs opencv` test.cpp -o test