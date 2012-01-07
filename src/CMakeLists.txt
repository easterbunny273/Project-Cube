#CMakeLists.txt for src-Folder

#define libraries which should be linked additionally
if(WIN32)
	include_directories(../external/glm/)
	include_directories(../external/lua/include)
	include_directories(../external/luabind)
	include_directories(../external/boost)
	include_directories(../external/cpptest/include)
	include_directories(../external/glfw/include)
	include_directories(../external/glew/include)
	include_directories(../external/assimp/include)
	include_directories(../external/devil/include)
else(WIN32)
	link_libraries(glfw GLEW IL assimp pthread cpptest lua luabind GL)
endif(WIN32)

#add include directories for compiler
include_directories(../BambooEngine/include)
include_directories(../include/)
include_directories(../include/tinyxml/)
include_directories(../test/)

#add definitions (for gcc)
add_definitions(-ggdb -std=c++11 -Wall -lpthread)

#define headers, not for compile time, only for QtCreator
file(GLOB HEADERS "../include/*.h")
file(GLOB HEADERS ${HEADERS} "../BambooEngine/include/*.h")
file(GLOB HEADERS ${HEADERS} "../BambooEngine/include/RenderNodes/*.h")
file(GLOB HEADERS ${HEADERS} "../BambooEngine/include/SceneObjects/*.h")
file(GLOB HEADERS ${HEADERS} "../include/Gamelogic/*.h")
file(GLOB HEADERS ${HEADERS} "../include/DummyGame/*.h")
#add test headers
file(GLOB HEADERS ${HEADERS} "../test/*.h")

#define sources
file(GLOB SOURCES "../src/*.cpp")
file(GLOB HEADERS ${HEADERS} "../BambooEngine/src/*.cpp")
file(GLOB HEADERS ${HEADERS} "../BambooEngine/src/RenderNodes/*.cpp")
file(GLOB HEADERS ${HEADERS} "../BambooEngine/src/SceneObjects/*.cpp")
file(GLOB SOURCES ${SOURCES} "../src/Gamelogic/*.cpp")
file(GLOB SOURCES ${SOURCES} "../src/DummyGame/*.cpp")
#add test sources
file(GLOB SOURCES ${SOURCES} "../test/*.cpp")


add_executable(project-cube ${SOURCES} ${HEADERS})

target_link_libraries(project-cube tinyxml)

#link external libraries when using VS2010
if(WIN32)
	target_link_libraries(project-cube opengl32)
	target_link_libraries(project-cube msvcrtd)
	target_link_libraries(project-cube libcmtd)
	
	target_link_libraries(project-cube ../../Project-Cube/external/assimp/lib/debug/assimp)
	target_link_libraries(project-cube ../../Project-Cube/external/cpptest/lib/cpptest)
	target_link_libraries(project-cube ../../Project-Cube/external/devil/lib/DevIL)
	target_link_libraries(project-cube ../../Project-Cube/external/devil/lib/ILU)
	target_link_libraries(project-cube ../../Project-Cube/external/devil/lib/ILUT)
	target_link_libraries(project-cube ../../Project-Cube/external/glew/lib/glew32)
	target_link_libraries(project-cube ../../Project-Cube/external/glfw/lib-msvc100/debug/GLFW)
	target_link_libraries(project-cube ../../Project-Cube/external/lua/lib/lua51)
	target_link_libraries(project-cube ../../Project-Cube/external/luabind/luabind)
	
endif(WIN32)
