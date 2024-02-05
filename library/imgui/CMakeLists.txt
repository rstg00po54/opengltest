

# 指定源文件
set(IMGUI_SOURCE_FILES 
./imgui.cpp 
./backends/imgui_impl_glfw.cpp 
./backends/imgui_impl_opengl3.cpp
./backends/imgui_impl_opengl2.cpp
./backends/imgui_impl_sdl2.cpp
./backends/imgui_impl_glut.cpp
./imgui_draw.cpp 
./imgui_tables.cpp 
./imgui_widgets.cpp
./imgui_demo.cpp
)
add_subdirectory(examples)
# 生成共享库
add_library(IMGUI SHARED ${IMGUI_SOURCE_FILES})

# 将当前模块的头文件目录添加到项目的头文件目录中
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 将当前模块的共享库添加到主项目的链接库中

target_include_directories(IMGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(IMGUI PRIVATE -g)
target_link_libraries(IMGUI SDL2 opengl32 freeglut)
