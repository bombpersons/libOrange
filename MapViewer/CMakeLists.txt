cmake_minimum_required(VERSION 2.8.4)
project(MapViewer)

#Get all of our source files
file(GLOB_RECURSE MAPVIEWER_SOURCES src/*.cpp src/*.c)

#The executable
add_executable(MapViewer ${MAPVIEWER_SOURCES})

#Link to libOrange
target_link_libraries(MapViewer orange)

#Get all of our headers
target_include_directories(MapViewer PUBLIC "../libOrange/include")
target_include_directories(MapViewer PUBLIC "include")

#Enable c++11
set_target_properties(MapViewer PROPERTIES COMPILE_FLAGS "-std=c++11")

#Copy data next to the exe
add_custom_command(TARGET MapViewer PRE_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_directory
				   ${PROJECT_SOURCE_DIR}/data $<TARGET_FILE_DIR:MapViewer>)
