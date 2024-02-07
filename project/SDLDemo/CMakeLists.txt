cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
 
project(SDLDemo)
include_directories(SYSTEM ../include/)
find_package(glfw3 REQUIRED)
 
add_executable(SDLDemo main.cpp)
 
# target_link_libraries(SDLDemo glfw SDL2 glew32)
target_link_libraries(SDLDemo PRIVATE opengl32 glfw SDL2 glew32 IMGUI)

project(imguiDemo)
include_directories(SYSTEM ../include/)
find_package(glfw3 REQUIRED)
 
add_executable(imguiDemo imgui.cpp)
 
# target_link_libraries(SDLDemo glfw SDL2 glew32)
target_link_libraries(imguiDemo PRIVATE opengl32 glfw SDL2 glew32 IMGUI)