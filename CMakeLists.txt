cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
 
project(test)
include_directories(SYSTEM ./include/)
find_package(glfw3 REQUIRED)
 
file(GLOB project_file src/glad.c src/main.cpp src/stb_image.cpp src/testglm.cpp)
add_executable(${PROJECT_NAME} ${project_file})
 
target_link_libraries(${PROJECT_NAME} glfw)