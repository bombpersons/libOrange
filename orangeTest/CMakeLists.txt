cmake_minimum_required(VERSION 2.8.4)
project(orangeTest)

#Get all of our source files
file(GLOB_RECURSE ORANGETEST_SOURCES src/*.cpp src/*.c)

#The executable
add_executable(orangeTest ${ORANGETEST_SOURCES})

#Link to libOrange
target_link_libraries(orangeTest orange)

#Get all of our headers
target_include_directories(orangeTest PUBLIC "../libOrange/include")

#Enable c++11
set_target_properties(orangeTest PROPERTIES COMPILE_FLAGS "-std=c++11")
