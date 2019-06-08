cmake_minimum_required (VERSION 2.6)

project (blasters)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


set(SOURCES Log.h Log.cpp)
add_executable(blasters blasters.cpp ${SOURCES})
add_executable(blastee blastee.cpp ${SOURCES})
