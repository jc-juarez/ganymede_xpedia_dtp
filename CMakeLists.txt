# *************************************
# Ganymede Xpedia
# Build
# 'CmakeLists.txt'
# Author: jcjuarez
# *************************************

cmake_minimum_required(VERSION 3.0)

project(AstraCachingServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILES
    src/main.cc
    src/gXThreadPool.cc
    src/gXDataTransmissionServer.cc
)

add_executable(gxtest ${SOURCE_FILES})

target_link_libraries(gxtest pthread)