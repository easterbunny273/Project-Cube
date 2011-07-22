#CMakeLists.txt for tinyxml

file(GLOB_RECURSE
	source_files
	*.h
	*.cpp
)

add_definitions(-DTIXML_USE_STL)

add_library(tinyxml ${source_files})