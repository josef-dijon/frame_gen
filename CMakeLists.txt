cmake_minimum_required(VERSION 3.13.4)
project(frame_gen)
 
include_directories(include)
 
file(GLOB SOURCES "src/*.c")
 
add_executable(frame_gen ${SOURCES})
install(TARGETS frame_gen DESTINATION /usr/bin)
