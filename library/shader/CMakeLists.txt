

# 指定源文件
set(shaderLib_SOURCE_FILES shader.cpp)

# 生成共享库
add_library(shader SHARED ${shaderLib_SOURCE_FILES})
include_directories(SYSTEM ../glad/include/glad)
find_package(glfw3 REQUIRED)
# 将当前模块的头文件目录添加到项目的头文件目录中
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# target_link_libraries(shader glfw glad stb_image shader)
target_link_libraries(shader glfw glad)
# 将当前模块的共享库添加到主项目的链接库中

# target_include_directories(LearnDemo1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/glad)
target_compile_options(shader PRIVATE -g)
