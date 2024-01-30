cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
 
project(LearnDemo1)
include_directories(SYSTEM ../include/)
include_directories(SYSTEM ../imgui/)

find_package(glfw3 REQUIRED)
 
file(GLOB project_file ../src/glad.c src/main.cpp ../src/stb_image.cpp src/testglm.cpp src/mglm.cpp)
add_executable(LearnDemo1 ${project_file})
 
target_link_libraries(LearnDemo1 glfw IMGUI)
# target_link_libraries(LearnDemo1 glfw)