cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_GENERATOR "MinGW Makefiles")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
 

# 设置可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 设置共享库的输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# 设置静态库的输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

project(MainProject)

# 添加子项目 ProjectA

# 添加子项目 ProjectB
# add_subdirectory(SDLDemo)
add_subdirectory(LearnDemo1)

include_directories(SYSTEM ./imgui)
include_directories(SYSTEM ./imgui/backends)
find_package(glfw3 REQUIRED)
add_library(IMGUI SHARED 
    ./imgui/imgui.cpp 
    ./imgui/backends/imgui_impl_glfw.cpp 
    ./imgui/backends/imgui_impl_opengl3.cpp
    ./imgui/imgui_draw.cpp 
    ./imgui/imgui_tables.cpp 
    ./imgui/imgui_widgets.cpp
    ./imgui/imgui_demo.cpp
)
target_link_libraries(IMGUI glfw glew32)