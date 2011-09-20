#CMakeLists.txt for src-Folder

#define libraries which should be linked additionally
link_libraries(glfw GLEW IL assimp pthread cpptest)

#add include directories
include_directories(../include/)
include_directories(../external/tinyxml/)
include_directories(../test/)

#add definitions (for gcc)
add_definitions(-ggdb -std=c++0x -Wall -lpthread)

#define headers
file(GLOB HEADERS "../include/*.h")
file(GLOB HEADERS ${HEADERS} "../include/Graphics/*.h")
file(GLOB HEADERS ${HEADERS} "../include/Graphics/SceneObjects/*.h")
file(GLOB HEADERS ${HEADERS} "../include/Gamelogic/*.h")
file(GLOB HEADERS ${HEADERS} "../include/DummyGame/*.h")
#add test headers
file(GLOB HEADERS ${HEADERS} "../test/*.h")

#define sources
file(GLOB SOURCES "../src/*.cpp")
file(GLOB SOURCES ${SOURCES} "../src/Graphics/*.cpp")
file(GLOB SOURCES ${SOURCES} "../src/Graphics/SceneObjects/*.cpp")
file(GLOB SOURCES ${SOURCES} "../src/Gamelogic/*.cpp")
file(GLOB SOURCES ${SOURCES} "../src/DummyGame/*.cpp")
#add test sources
file(GLOB SOURCES ${SOURCES} "../test/*.cpp")


add_executable(project-cube ${SOURCES} ${HEADERS})

target_link_libraries(project-cube tinyxml)


