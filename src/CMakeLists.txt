#CMakeLists.txt for src-Folder

#define libraries which should be linked additionally
if(WIN32)
	include_directories(../external/glm/)
	include_directories(../external/lua/include)
	include_directories(../external/luaponte/include)
	include_directories(../external/cpptest/include)
	include_directories(../external/glfw/include)
	include_directories(../external/glew/include)
	include_directories(../external/assimp/include)
	include_directories(../external/devil/include)
	include_directories(../external/boost/include)
else(WIN32)
	link_libraries(glfw GLEW IL assimp pthread cpptest lua luaponte GL)
endif(WIN32)

#compiler flags
if(WIN32)
	SET(CMAKE_CXX_FLAGS_DEBUG "/MTd /W3")
	SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG /FORCE:MULTIPLE")
else(WIN32)
	add_definitions(-ggdb -std=c++11 -Wall -lpthread -fpermissive)
endif(WIN32)

#add include directories for compiler
include_directories(/usr/include/lua/5.2/)
include_directories(../BambooEngine/include/)
include_directories(../include/)
include_directories(../)
include_directories(../include/tinyxml/)
include_directories(../test/)

#define headers, not for compile time, only for QtCreator
file(GLOB H_BAMBOO "../BambooEngine/include/*.h")
file(GLOB H_BAMBOO_RN "../BambooEngine/include/RenderNodes/*.h")
file(GLOB H_BAMBOO_SSN "../BambooEngine/include/SemanticSceneNodes/*.h")
file(GLOB H_BAMBOO_DNT "../BambooEngine/include/DeferredNodeTranslator/*.h")
file(GLOB H_GAMELOGIC "../include/Gamelogic/*.h")
file(GLOB H_GAMELOGIC_OBJ "../include/Gamelogic/Objects/*.h")
file(GLOB H_DUMMYGAME "../include/DummyGame/*.h")
#add test headers
file(GLOB H_TEST "../test/*.h")
#combine headers
file(GLOB HEADERS "../include/*.h"
				  ${H_BAMBOO} 
				  ${H_BAMBOO_RN} 
				  ${H_BAMBOO_SO} 
				  ${H_BAMBOO_STO} 
				  ${H_BAMBOO_DNT} 
				  ${H_BAMBOO_SSN} 
				  ${H_BAMBOO_RSN} 
				  ${H_GAMELOGIC} 
				  ${H_GAMELOGIC_OBJ}
				  ${H_DUMMYGAME} 
				  ${H_TEST})

#define sources
file(GLOB S_BAMBOO "../BambooEngine/src/*.cpp")
file(GLOB S_BAMBOO_RN "../BambooEngine/src/RenderNodes/*.cpp")
file(GLOB S_BAMBOO_SSN "../BambooEngine/src/SemanticSceneNodes/*.cpp")
file(GLOB S_BAMBOO_STO "../BambooEngine/src/StateObjects/*.cpp")
file(GLOB S_BAMBOO_DNT "../BambooEngine/src/DeferredNodeTranslator/*.cpp")
file(GLOB S_GAMELOGIC "../src/Gamelogic/*.cpp")
file(GLOB S_GAMELOGIC_OBJ "../src/Gamelogic/Objects/*.cpp")
file(GLOB S_DUMMYGAME "../src/DummyGame/*.cpp")
#add test sources
file(GLOB S_TEST "../test/*.cpp")
#combine sources
file(GLOB SOURCES "../src/*.cpp" 
				  ${S_BAMBOO}
				  ${S_BAMBOO_RN} 
				  ${S_BAMBOO_SO} 
				  ${S_BAMBOO_STO} 
				  ${S_BAMBOO_SSN} 
				  ${S_BAMBOO_DNT} 
				  ${S_GAMELOGIC} 
				  ${S_GAMELOGIC_OBJ} 
				  ${S_DUMMYGAME} 
				  ${S_TEST})


if(WIN32)
	SOURCE_GROUP("Header Files\\Gamelogic" FILES ${H_GAMELOGIC})
	SOURCE_GROUP("Header Files\\Gamelogic\\Objects" FILES ${H_GAMELOGIC_OBJ})
	SOURCE_GROUP("Header Files\\DummyGame" FILES ${H_DUMMYGAME})
	SOURCE_GROUP("Header Files\\BambooEngine" FILES ${H_BAMBOO})
	SOURCE_GROUP("Header Files\\BambooEngine\\RenderNodes" FILES ${H_BAMBOO_RN})
	SOURCE_GROUP("Header Files\\BambooEngine\\SemanticSceneNodes" FILES ${H_BAMBOO_SO})
	SOURCE_GROUP("Header Files\\BambooEngine\\SemanticSceneNodes" FILES ${H_BAMBOO_SSN})
	SOURCE_GROUP("Header Files\\BambooEngine\\RenderingSceneNodes" FILES ${H_BAMBOO_RSN})
	SOURCE_GROUP("Header Files\\BambooEngine\\StateObjects" FILES ${H_BAMBOO_STO})
	SOURCE_GROUP("Header Files\\BambooEngine\\DeferredNodeTranslator" FILES ${H_BAMBOO_DNT})
	SOURCE_GROUP("Header Files\\Test" FILES ${H_TEST})
	
	SOURCE_GROUP("Source Files\\Gamelogic" FILES ${S_GAMELOGIC})
	SOURCE_GROUP("Source Files\\Gamelogic\\Objects" FILES ${S_GAMELOGIC_OBJ})
	SOURCE_GROUP("Source Files\\DummyGame" FILES ${S_DUMMYGAME})
	SOURCE_GROUP("Source Files\\BambooEngine" FILES ${S_BAMBOO})
	SOURCE_GROUP("Source Files\\BambooEngine\\RenderNodes" FILES ${S_BAMBOO_RN})
	SOURCE_GROUP("Source Files\\BambooEngine\\SemanticSceneNodes" FILES ${S_BAMBOO_SO})
	SOURCE_GROUP("Source Files\\BambooEngine\\SemanticSceneNodes" FILES ${S_BAMBOO_SSN})
	SOURCE_GROUP("Source Files\\BambooEngine\\RenderingSceneNodes" FILES ${S_BAMBOO_RSN})
	SOURCE_GROUP("Source Files\\BambooEngine\\StateObjects" FILES ${S_BAMBOO_STO})
	SOURCE_GROUP("Source Files\\BambooEngine\\DeferredNodeTranslator" FILES ${S_BAMBOO_DNT})
	SOURCE_GROUP("Source Files\\Test" FILES ${S_TEST})
endif(WIN32)

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
	target_link_libraries(project-cube ../../Project-Cube/external/lua/msvc-10.0-x86/lua.debug)
	target_link_libraries(project-cube ../../Project-Cube/external/luaponte/msvc-10.0-x86/luaponte.debug)
	
endif(WIN32)
