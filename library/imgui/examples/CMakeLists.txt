cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
 
project(example_sdl2_opengl3)


include_directories(SYSTEM ../backends)
# include_directories(SYSTEM SDL2)

# include_directories(SYSTEM ../include/)
find_package(glfw3 REQUIRED)
 
add_executable(example_sdl2_opengl3 example_sdl2_opengl3/main.cpp)
add_executable(example_sdl2_opengl2 example_sdl2_opengl2/main.cpp)
add_executable(example_glut_opengl2 example_glut_opengl2/main.cpp)
 
# target_link_libraries(SDLDemo glfw SDL2 glew32)
# target_link_libraries(IMGUIDemo PRIVATE opengl32 glfw SDL2 glew32)
target_link_libraries(example_sdl2_opengl3 IMGUI SDL2 opengl32)
target_link_libraries(example_sdl2_opengl2 IMGUI SDL2 opengl32)
target_link_libraries(example_glut_opengl2 IMGUI SDL2 opengl32 freeglut)