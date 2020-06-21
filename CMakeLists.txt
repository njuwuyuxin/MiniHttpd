cmake_minimum_required (VERSION 2.6)
add_definitions(-std=c++11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wno-unused-variable -pthread")
project (Minihttpd)

FILE(GLOB SOURCE_FILES "*.cpp" "*.h")
add_executable(Minihttpd ${SOURCE_FILES})