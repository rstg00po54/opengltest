cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
 
project(SDLDemo)
include_directories(SYSTEM ../include/)
find_package(glfw3 REQUIRED)
 
add_executable(SDLDemo ../src/glad.c main.cpp)
 
target_link_libraries(SDLDemo glfw SDL2)