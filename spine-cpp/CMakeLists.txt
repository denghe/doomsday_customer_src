cmake_minimum_required(VERSION 3.8)

project(spine)

include_directories(
	include
)

set(SPINE_SRCS)
file(GLOB SPINE_SRCS
	include/spine/*.h
	src/spine/*.cpp
)

add_library(spine STATIC
    ${SPINE_SRCS}
)
