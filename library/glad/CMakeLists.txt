

# 指定源文件
set(gladLib_SOURCE_FILES src/glad.c)

# 生成共享库
add_library(glad SHARED ${gladLib_SOURCE_FILES})

# 将当前模块的头文件目录添加到项目的头文件目录中
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 将当前模块的共享库添加到主项目的链接库中

# target_include_directories(LearnDemo1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/glad)
target_compile_options(glad PRIVATE -g)
