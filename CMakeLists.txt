cmake_minimum_required(VERSION 3.3)
project(tigerhttpd)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

#head file path
include_directories(SYSTEM
    ${GLOG_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/prefix/include
    ${CMAKE_SOURCE_DIR}/src
)

#source directory
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

#set environment variable
set(SOURCE_FILES ${DIR_SRCS})

#add executable file
add_executable(tigerhttpd ${SOURCE_FILES})

#add link library
TARGET_LINK_LIBRARIES(tigerhttpd m)
