cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
 
project(SoftMini3D)
# include_directories(SYSTEM ../include/)
include_directories(SYSTEM .)
find_package(glfw3 REQUIRED)
 
add_executable(SoftMini3D main.cpp Mini3DMath.cpp Mini3DTransform.cpp Mini3DGeometryCalc.cpp Mini3Dbmp.cpp)
 
# target_link_libraries(SDLDemo glfw SDL2 glew32)
# target_link_libraries(IMGUIDemo PRIVATE opengl32 glfw SDL2 glew32)
target_link_libraries(SoftMini3D glfw IMGUI SDL2 glad opengl32)