#CMakeLists.txt for src-Folder

include_directories(../include/tinyxml/)

set(SOURCES Main.cpp Settings.cpp Settings.h)

add_executable("Project Cube" ${SOURCES})

target_link_libraries("Project Cube" tinyxml)