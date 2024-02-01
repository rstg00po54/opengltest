

# 指定源文件
set(stb_imageLib_SOURCE_FILES stb_image.cpp)

# 生成共享库
add_library(stb_image SHARED ${stb_imageLib_SOURCE_FILES})

# 将当前模块的头文件目录添加到项目的头文件目录中
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 将当前模块的共享库添加到主项目的链接库中

target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(stb_image PRIVATE -g)
