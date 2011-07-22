#CMakeLists.txt for src-Folder

#define libraries which should be linked additionally
link_libraries(glfw GLEW IL)

#add include directories
include_directories(../include/)
include_directories(../external/tinyxml/)

#add definitions (for gcc)
add_definitions(-ggdb -std=c++0x)

#define headers
file(GLOB HEADERS "../include/*.h")

#define sources
file(GLOB SOURCES "../src/*.cpp")

add_executable(project-cube ${SOURCES} ${HEADERS})

target_link_libraries(project-cube tinyxml)

