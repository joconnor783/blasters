cmake_minimum_required (VERSION 2.6)

project (blasters)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/rapidxml-1.13/)

set(XML ${CMAKE_CURRENT_SOURCE_DIR}/rapidxml-1.13/rapidxml.hpp ${CMAKE_CURRENT_SOURCE_DIR}/rapidxml-1.13/rapidxml_utils.hpp)
set(SOURCES Log.h Log.cpp Loader.h Loader.cpp Packet.h Packet.cpp)
set(BLASTEE_SRC BlasteeLoader.h BlasteeLoader.cpp)
set(BLASTERS_SRC BlastersLoader.h BlastersLoader.cpp)

add_executable(blasters blasters.cpp ${SOURCES} ${XML} ${BLASTERS_SRC}) 
add_executable(blastee blastee.cpp ${SOURCES} ${XML} ${BLASTEE_SRC})
