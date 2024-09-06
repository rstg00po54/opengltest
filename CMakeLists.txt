cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_GENERATOR "MinGW Makefiles")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
 

# 设置可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# 设置共享库的输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# 设置静态库的输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
message(STATUS "Hello, this is a status message top.${CMAKE_CURRENT_BINARY_DIR}")
project(MainProject)

# 添加子项目 ProjectA
include_directories(SYSTEM ./library/imgui)
include_directories(SYSTEM ./library/imgui/backends)
include_directories(SYSTEM library/glad/include/glad)
include_directories(SYSTEM library/stb_image)
include_directories(SYSTEM library/shader)
# 添加子项目 ProjectB
# add_subdirectory(SDLDemo)
add_subdirectory(project/LearnDemo1)
add_subdirectory(project/LearnDemo2)
add_subdirectory(project/SDLDemo)
add_subdirectory(project/SDLPlane)
add_subdirectory(library/glad)
add_subdirectory(library/imgui)
add_subdirectory(library/stb_image)
add_subdirectory(library/shader)
add_subdirectory(project/Trangle)
add_subdirectory(project/imguiDemo)
add_subdirectory(project/SoftMini3D)
add_subdirectory(project/qt-renderer-master)
add_subdirectory(project/OpenCL)
add_subdirectory(project/SutherlandHodgman)


find_package(glfw3 REQUIRED)

target_link_libraries(IMGUI glfw glad stb_image shader)