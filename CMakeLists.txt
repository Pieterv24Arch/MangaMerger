CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(MangaMerger)

SET(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(Boost COMPONENTS regex system filesystem REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

INCLUDE_DIRECTORIES(libs)

FIND_LIBRARY(LHPDF_LIB
    NAMES
    hpdf
    PATHS
    /usr/local/lib)

SET(SOURCE_FILES main.cpp mangamerger.cpp mangamerger.h threadpool.cpp threadpool.h imagecompressor.cpp imagecompressor.h)
ADD_EXECUTABLE(MangaMerger ${SOURCE_FILES})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

TARGET_LINK_LIBRARIES( MangaMerger ${LHPDF_LIB})
TARGET_LINK_LIBRARIES( MangaMerger ${Boost_LIBRARIES})