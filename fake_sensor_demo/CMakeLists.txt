cmake_minimum_required(VERSION 3.1.0)
project (fake_sensor_demo)
add_executable(demo_sensor_server showCVImage.cpp CppTimer.cpp )
set (CMAKE_CXX_STANDARD 11)

find_package (Threads)
find_package(OpenCV REQUIRED) # add

include_directories(${OpenCV_INCLUDE_DIRS}) # add

TARGET_LINK_LIBRARIES(demo_sensor_server json-fastcgi rt ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(demo_sensor_server json-fastcgi rt ${OpenCV_LIBS}) # add

