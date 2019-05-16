cmake_minimum_required(VERSION 3.3.2)
project(frame_gen)

include_directories(include)
file(GLOB SOURCES "src/*.c")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(frame_gen ${SOURCES})
install(TARGETS frame_gen DESTINATION /usr/bin)
