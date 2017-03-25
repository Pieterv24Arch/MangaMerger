cmake_minimum_required(VERSION 3.6)
project(MangaMerger)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS regex system filesystem REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

INCLUDE_DIRECTORIES(libs)

set(SOURCE_FILES main.cpp mangamerger.cpp mangamerger.h threadpool.cpp threadpool.h imagecompressor.cpp imagecompressor.h)
add_executable(MangaMerger ${SOURCE_FILES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

TARGET_LINK_LIBRARIES( MangaMerger ${Boost_LIBRARIES})