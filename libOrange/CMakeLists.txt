cmake_minimum_required(VERSION 2.8.4)
project(libOrange)

#Get all of our source files
file(GLOB_RECURSE LIBORANGE_SOURCES src/*.cpp src/*.c)

#Add the library
add_library(orange STATIC ${LIBORANGE_SOURCES})

#Add libraries to link
target_link_libraries(orange opengl32 winmm gdi32)

#Get all of our headers
target_include_directories(orange PUBLIC "include")

#Enable c++11
set_target_properties(orange PROPERTIES COMPILE_FLAGS "-std=c++11")
