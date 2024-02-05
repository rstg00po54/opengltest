cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
 
project(Trangle)
include_directories(SYSTEM ./)
include_directories(SYSTEM ./EGE20.08/include)
find_package(glfw3 REQUIRED)
 
add_executable(Trangle  main.cpp)
 
# link_directories(./EGE20.08/lib/mingw64)
target_link_libraries(Trangle ${CMAKE_CURRENT_SOURCE_DIR}/EGE20.08/lib/mingw64/libgraphics64.a)

# target_link_libraries(SDLDemo glfw SDL2 glew32)
# target_link_libraries(Trangle PRIVATE opengl32 glfw SDL2 glew32)
target_link_libraries(Trangle opengl32 SDL2)